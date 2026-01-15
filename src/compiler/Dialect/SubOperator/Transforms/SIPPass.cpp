#include "json.h"
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
      subop::GetExternalOp externalProbeOp;
      bool isPositiveJoin; // True for IN/all_true joins, false for NOT IN/none_true joins
   };

   mlir::Operation* walkToFindSourceScan(mlir::Operation* op, bool debug = false) {
      mlir::Operation* current = op;
      while (current->getNumOperands() > 0) {
         if (debug) {
            if (debug) {
               std::cerr << "Walking op: " << current->getName().getStringRef().str() << std::endl;
               current->dump();
            }
         }
         if (auto scan = mlir::dyn_cast_or_null<subop::ScanOp>(current)) {
            return scan;
         } else if (auto mat = mlir::dyn_cast_or_null<subop::MaterializeOp>(current)) {
            current = mat->getOperand(0).getDefiningOp();
         } else {
            if (current->getNumOperands() > 1) {
               if (debug) {
                  std::cerr << "should not happen" << std::endl;
                  std::cerr << "No scan for found last current: " << std::endl;
                  current->dump();
               }


            }
            current = current->getOperand(0).getDefiningOp();
         }
      }
      return nullptr;
   }

   mlir::Operation* findSourceScanFromCreateHashIndexedView(subop::CreateHashIndexedView createHashIndexedView, bool debug) {
      auto users = createHashIndexedView.getOperand().getUsers();
      for (auto* user : users) {
         if (auto mat = mlir::dyn_cast_or_null<subop::MaterializeOp>(user)) {
            return walkToFindSourceScan(mat, debug);
         }
      }

      return nullptr;
   }

   std::optional<HashJoinInfo> identifyHashJoinTables(subop::LookupOp lookupOp, bool debug = false) {
      mlir::Value stateValue = lookupOp.getState();
      auto* stateDefOp = stateValue.getDefiningOp();
      auto createHashView = mlir::dyn_cast_or_null<subop::CreateHashIndexedView>(stateDefOp);
      if (!createHashView) return std::nullopt;
      //First get Build/Input side
      auto buildScan = findSourceScanFromCreateHashIndexedView(createHashView, false);
      if (!buildScan) {
         return std::nullopt;
      }
      //Get probe side
      if (debug) {
         lookupOp.getStream().getDefiningOp()->dump();
      }
      auto probeScan = walkToFindSourceScan(lookupOp.getStream().getDefiningOp(), debug);
      if (debug) {
         probeScan->dump();
      }
      if (!probeScan) {
         return std::nullopt;
      }

      //Get get_external op from probe scan
      subop::GetExternalOp externalOp;
      if (auto scanOp = mlir::dyn_cast_or_null<subop::ScanOp>(probeScan)) {
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
               // Extract the input columns attribute from the map operation
               auto inputAttr = mapOp.getInputColsAttr();
               for (auto input : inputAttr) {
                  if (auto colRef = mlir::dyn_cast<tuples::ColumnRefAttr>(input)) {
                     buildKeyColumns.push_back(colRef);
                  }
               }
            }
         }
      }

      // Extract key columns from probe side: find the MapOp before lookup
      llvm::SmallVector<tuples::ColumnRefAttr> probeKeyColumns;
      mlir::Operation* probeStream = lookupOp.getStream().getDefiningOp();
      if (auto probeMapOp = mlir::dyn_cast_or_null<subop::MapOp>(probeStream)) {
         auto inputAttr = probeMapOp.getInputColsAttr();
         for (auto input : inputAttr) {
            if (auto colRef = mlir::dyn_cast<tuples::ColumnRefAttr>(input)) {
               probeKeyColumns.push_back(colRef);
            }
         }
      }
      // Detect if this is a positive (IN/all_true) or negative (NOT IN/none_true) join
      //TODO detection may be hardly simplified
      //TODO einmal selber machen
      bool isPositiveJoin = true;
      for (auto* user : lookupOp->getUsers()) {
         if (auto nestedMap = mlir::dyn_cast_or_null<subop::NestedMapOp>(user)) {
            nestedMap.walk([&](subop::FilterOp filter) {
               if (filter.getFilterSemantic() == subop::FilterSemantic::none_true) {
                  isPositiveJoin = false;
               }
            });
         }
      }

      return HashJoinInfo{.buildSideRoot = buildScan->getResult(0),
                          .probeSideRoot = probeScan->getResult(0),
                          .hashView = createHashView,
                          .lookupOp = lookupOp,
                          .buildKeyColumns = buildKeyColumns,
                          .probeKeyColumns = probeKeyColumns,
                          .externalProbeOp = externalOp,
                          .isPositiveJoin = isPositiveJoin};
   }
   std::string gen_random(const int len) {
      static const char alphanum[] =
         "0123456789"
         "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
         "abcdefghijklmnopqrstuvwxyz";
      std::string tmp_s;
      tmp_s.reserve(len);

      for (int i = 0; i < len; ++i) {
         tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
      }

      return tmp_s;
   }
   void runOnOperation() override {
      auto& created = getAnalysis<subop::ColumnCreationAnalysis>();
      auto& used = getAnalysis<subop::ColumnUsageAnalysis>();
      std::optional<HashJoinInfo> joinInfo;
      getOperation()->walk([&](subop::LookupOp lookupOp) {
         joinInfo = identifyHashJoinTables(lookupOp);
         if (joinInfo) {
#if 0
            llvm::dbgs() << "Hash Join Found:\n";
            llvm::dbgs() << "  Build Side Root: " << joinInfo->buildSideRoot << "\n";
            llvm::dbgs() << "  Build Side get external" << joinInfo->externalProbeOp << "\n";
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
#endif
            if (joinInfo->probeKeyColumns.size() != 1 || joinInfo->buildKeyColumns.size() != 1) {
               return;
            }
            static int count = 0;

            count++;
            if (count!=5) {
               return;
            }

            identifyHashJoinTables(lookupOp, true);

            //Do SIP
            auto module = getOperation();
            mlir::Location loc = joinInfo->hashView->getLoc();
            mlir::OpBuilder b(joinInfo->hashView);
            b.setInsertionPointAfter(joinInfo->hashView);
            // Create a SIP filter named "test" for testing purposes.
            std::string jsonRaw = joinInfo->externalProbeOp.getDescr().str();
            nlohmann::json descr = nlohmann::json::parse(jsonRaw);
            std::string sipName = gen_random(10);
            auto probeColRef = joinInfo->probeKeyColumns[0];
            auto buildColRef = joinInfo->buildKeyColumns[0];
            if (descr.contains("restrictions")) {
               //Add restrictions to JSON
               nlohmann::json restrictionJson;
               // Get probe-side key column name (use root::leaf if root is present)

               if (auto sym = probeColRef.getName()) {
                  auto root = sym.getRootReference();
                  auto leaf = sym.getLeafReference();
                  std::string colName;

                  colName = leaf.str();
                  restrictionJson["column"] = colName;
               } else {
                  assert(false);
                  restrictionJson["column"] = "";
               }
               restrictionJson["cmp"] = joinInfo->isPositiveJoin ? "sip" : "n_sip";
               restrictionJson["value"] = sipName;
               descr["restrictions"].push_back(restrictionJson);
            }
            {
               if (auto buildSym = buildColRef.getName()) {
                  auto buildroot = buildSym.getRootReference();
                  auto buildleaf = buildSym.getLeafReference();
                  if (auto probeSym = probeColRef.getName()) {
                     auto proberoot = probeSym.getRootReference();
                     auto probeleaf = probeSym.getLeafReference();
                     if (std::getenv("LINGODB_SIP_DEBUG")) {
                        std::cerr << "Adding SIP for column from(build) " << buildroot.str() << ":" << buildleaf.str() << " to(probe)  " << proberoot.str() << ":" << probeleaf.str() << " for sipId: " << sipName << " positive: " << joinInfo->isPositiveJoin << std::endl;
                        std::cerr << "Probe: " << std::endl;
                        joinInfo->externalProbeOp->dump();
                     }

                  } else {
                     assert(false);
                  }
               }
            }

            //descr to string
            std::string updatedDescr = to_string(descr);
            joinInfo->externalProbeOp.setDescr(b.getStringAttr(updatedDescr));
            b.create<subop::CreateSIPFilterOp>(loc, joinInfo->hashView.getResult(), b.getStringAttr(sipName));
         }
      });
   }
};

} // namespace
std::unique_ptr<mlir::Pass>
subop::createSIPPass() { return std::make_unique<SIPPass>(); }
