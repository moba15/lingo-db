#include "lingodb/compiler/Dialect/DB/IR/DBDialect.h"
#include "lingodb/compiler/Dialect/DB/IR/DBOps.h"
#include "lingodb/compiler/Dialect/SubOperator/SubOperatorDialect.h"
#include "lingodb/compiler/Dialect/SubOperator/SubOperatorOps.h"
#include "lingodb/compiler/Dialect/SubOperator/Transforms/ColumnCreationAnalysis.h"
#include "lingodb/compiler/Dialect/SubOperator/Transforms/ColumnUsageAnalysis.h"
#include "lingodb/compiler/Dialect/SubOperator/Transforms/Passes.h"
#include "lingodb/compiler/Dialect/SubOperator/Transforms/StateUsageTransformer.h"
#include "lingodb/compiler/Dialect/SubOperator/Transforms/SubOpDependencyAnalysis.h"
#include "lingodb/compiler/Dialect/SubOperator/Utils.h"
#include "lingodb/compiler/Dialect/TupleStream/TupleStreamDialect.h"
#include "lingodb/compiler/Dialect/TupleStream/TupleStreamOps.h"
#include "lingodb/compiler/Dialect/util/UtilDialect.h"
#include "lingodb/compiler/Dialect/util/UtilOps.h"
#include "lingodb/compiler/helper.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/raw_ostream.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"

#include <llvm/Support/Debug.h>
#include <algorithm>
#include <lingodb/compiler/Dialect/DB/IR/DBOps.h.inc>
#include <string>
#include <mlir/Dialect/Arith/IR/Arith.h>

#define DEBUG_TYPE "subop-sip"
namespace {
using namespace lingodb::compiler::dialect;

static inline std::string trim(const std::string& s) {
   size_t a = s.find_first_not_of(" \t\n\r");
   if (a == std::string::npos) return "";
   size_t b = s.find_last_not_of(" \t\n\r");
   return s.substr(a, b - a + 1);
}

class SIPPass : public mlir::PassWrapper<SIPPass, mlir::OperationPass<mlir::ModuleOp>> {
   public:
   MLIR_DEFINE_EXPLICIT_INTERNAL_INLINE_TYPE_ID(SIPPass)
   virtual llvm::StringRef getArgument() const override { return "subop-sip-pass"; }

   // Generic helper to identify build/probe sides of a hash join
   struct HashJoinInfo {
      mlir::Value buildSideRoot; // Root scan/table operation for build side
      mlir::Value probeSideRoot; // Root scan/table operation for probe side
      subop::CreateHashIndexedView hashView; // The hash indexed view
      subop::LookupOp lookupOp; // The lookup operation
      llvm::SmallVector<tuples::ColumnRefAttr> buildKeyColumns; // Columns used for hash key on build side
      llvm::SmallVector<tuples::ColumnRefAttr> probeKeyColumns; // Columns used for hash key on probe side

      subop::GetExternalOp externalOp;
   };

   mlir::Operation* walkToFindSourceScan(mlir::Operation* op) {
      mlir::Operation* current = op;
      while (current->getNumOperands() > 0) {
         if (auto scan = mlir::dyn_cast_or_null<subop::ScanOp>(current)) {
            return scan;
         } else {
            current = current->getOperand(0).getDefiningOp();
         }
      }
   }

   mlir::Operation* findSourceScanFromCreateHashIndexedView(subop::CreateHashIndexedView createHashIndexedView) {
      auto users = createHashIndexedView.getOperand().getUsers();
      for (auto* user : users) {
         if (auto mat = mlir::dyn_cast_or_null<subop::MaterializeOp>(user)) {
            return walkToFindSourceScan(mat);
         }
      }

      return nullptr;
   }

   std::optional<HashJoinInfo> identifyHashJoinTables(subop::LookupOp lookupOp) {
      mlir::Value stateValue = lookupOp.getState();
      auto* stateDefOp = stateValue.getDefiningOp();
      auto createHashView = mlir::dyn_cast_or_null<subop::CreateHashIndexedView>(stateDefOp);
      if (!createHashView) return std::nullopt;
      //First get Build/Input side
      auto buildScan = findSourceScanFromCreateHashIndexedView(createHashView);
      if (!buildScan) {
         return std::nullopt;
      }
      //Get probe side
      auto probeScan = walkToFindSourceScan(lookupOp.getStream().getDefiningOp());
      if (!probeScan) {
         return std::nullopt;
      }

      //Get get_external op from scan
      subop::GetExternalOp externalOp;
      if (auto scanOp = mlir::dyn_cast_or_null<subop::ScanOp>(probeScan)) {
         std::cerr << "Test" << std::endl;
         externalOp = mlir::dyn_cast_or_null<subop::GetExternalOp>(scanOp.getState().getDefiningOp());
         if (!externalOp) return std::nullopt;
      }

      // Extract key columns from build side: find the MapOp that feeds into materialize
      llvm::SmallVector<tuples::ColumnRefAttr> buildKeyColumns;
      auto bufferUsers = createHashView.getOperand().getUsers();
      for (auto* user : bufferUsers) {
         if (auto mat = mlir::dyn_cast_or_null<subop::MaterializeOp>(user)) {
            mlir::Value streamBeforeMat = mat->getOperand(0);
            if (auto mapOp = mlir::dyn_cast_or_null<subop::MapOp>(streamBeforeMat.getDefiningOp())) {
               mapOp.dump();
               // Extract the input columns attribute from the map operation
               auto inputAttr = mapOp.getInputColsAttr();
               for (auto input : inputAttr) {
                  if (auto colRef = mlir::dyn_cast<tuples::ColumnRefAttr>(input)) {
                     buildKeyColumns.push_back(colRef);
                     colRef.dump();
                  }
               }
            }
         }
      }
      std::cerr << buildKeyColumns.size() << std::endl;

      // Extract key columns from probe side: find the MapOp before lookup
      llvm::SmallVector<tuples::ColumnRefAttr> probeKeyColumns;
      mlir::Operation* probeStream = lookupOp.getStream().getDefiningOp();
      if (auto probeMapOp = mlir::dyn_cast_or_null<subop::MapOp>(probeStream)) {
         auto inputAttr = probeMapOp.getInputColsAttr();
         for (auto input : inputAttr) {
            if (auto colRef = mlir::dyn_cast<tuples::ColumnRefAttr>(input)) {
               probeKeyColumns.push_back(colRef);

               colRef.dump();
            }
         }
      }

      return HashJoinInfo{.buildSideRoot = buildScan->getResult(0),
                          .probeSideRoot = probeScan->getResult(0),
                          .hashView = createHashView,
                          .lookupOp = lookupOp,
                          .buildKeyColumns = buildKeyColumns,
                          .probeKeyColumns = probeKeyColumns,
                          .externalOp = externalOp};
   }

   void runOnOperation() override {
      auto& created = getAnalysis<subop::ColumnCreationAnalysis>();
      auto& used = getAnalysis<subop::ColumnUsageAnalysis>();
      std::optional<HashJoinInfo> joinInfo;
      getOperation()->walk([&](subop::LookupOp lookupOp) {
         joinInfo = identifyHashJoinTables(lookupOp);
         if (joinInfo) {
            llvm::dbgs() << "Hash Join Found:\n";
            llvm::dbgs() << "  Build Side Root: " << joinInfo->buildSideRoot << "\n";
            llvm::dbgs() << "  Build Side get external" << joinInfo->externalOp << "\n";
            llvm::dbgs() << "  Probe Side Root: " << joinInfo->probeSideRoot << "\n";
            llvm::dbgs() << "  Hash View: " << joinInfo->probeKeyColumns.size() << "\n";
            llvm::dbgs() << "Build side key inputs: \n";
            for (auto& in : joinInfo->buildKeyColumns) {
               llvm::dbgs() << " - ";
               in.dump();
            }
            llvm::dbgs() << "Probe side key inputs: \n";
            for (auto& in : joinInfo->probeKeyColumns) {
               llvm::dbgs() << " - ";
               in.dump();
            }

            if (joinInfo->probeKeyColumns.size() != 1) {
               return;
            }
            //Do SIP
            auto module = getOperation();
            mlir::Location loc = joinInfo->hashView->getLoc();
            mlir::OpBuilder b(joinInfo->hashView);
            b.setInsertionPointAfter(joinInfo->hashView);
            b.create<subop::CreateSIPFilterOp>(loc, joinInfo->hashView.getResult(), joinInfo->externalOp.getResult());
         }
      });
   }
};

} // namespace
std::unique_ptr<mlir::Pass>
subop::createSIPPass() { return std::make_unique<SIPPass>(); }
