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
   void runOnOperation() override {
      // SIP-oriented walk: per execution group, collect
      //  - tables touched
      //  - scan outputs (build/probe candidates)
      //  - hash join use: lookup keys, producing ops, and hash-table layout
      auto& created = getAnalysis<subop::ColumnCreationAnalysis>();
      auto& used = getAnalysis<subop::ColumnUsageAnalysis>();

      getOperation()->walk([&](mlir::Operation* op) {
         if (auto scan = mlir::dyn_cast_or_null<subop::ScanOp>(op)) {
            llvm::dbgs() << "Found scan op: " << scan << "\n";
            llvm::dbgs() << "  State: " << scan.getState() << " of type " << scan.getState().getType() << "\n";
            llvm::dbgs() << "  Mapping: " << scan.getMapping() << "\n";
            auto usedCols = used.getUsedColumnsForOp(scan);
         }
         if (auto map = mlir::dyn_cast_or_null<subop::MapOp>(op)) {
            map.dump();
            auto usedCols = used.getUsedColumnsForOp(map);
            llvm::dbgs() << "  Used columns:\n";
            for (auto* col : usedCols) {
               llvm::dbgs() << col->type << "\n";
            }
            map.walk([&](mlir::Operation* op) {
               if (auto hash = mlir::dyn_cast_or_null<db::Hash>(op)) {
                  llvm::dbgs() << "  Hash: " << "\n";
                  hash->dumpPretty();
                  hash.getOperands()[0].dump();
               }
            });
         }
         if (auto genericCreateOp = mlir::dyn_cast_or_null<subop::GenericCreateOp>(op)) {
            if (genericCreateOp.getRes().getType().isa<subop::BufferType>()) {
               auto type = genericCreateOp.getRes().getType();

               auto buffer = mlir::dyn_cast_or_null<subop::BufferType>(type);
               assert(buffer);
               ;


            }

         }
         if (auto createHashIndexView = mlir::dyn_cast_or_null<subop::CreateHashIndexedView>(op)) {
            createHashIndexView.dump();
         }




      });
   }
};

} // namespace
std::unique_ptr<mlir::Pass>
subop::createSIPPass() { return std::make_unique<SIPPass>(); }
