#include "lingodb/compiler/Dialect/SubOperator/SubOperatorDialect.h"
#include "lingodb/compiler/Dialect/SubOperator/SubOperatorOps.h"
#include "lingodb/compiler/Dialect/SubOperator/Transforms/ColumnCreationAnalysis.h"
#include "lingodb/compiler/Dialect/SubOperator/Transforms/ColumnUsageAnalysis.h"
#include "lingodb/compiler/Dialect/SubOperator/Transforms/Passes.h"
#include "lingodb/compiler/Dialect/SubOperator/Transforms/SubOpDependencyAnalysis.h"
#include "lingodb/compiler/Dialect/TupleStream/TupleStreamDialect.h"
#include "lingodb/compiler/Dialect/TupleStream/TupleStreamOps.h"
#include "lingodb/compiler/helper.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/raw_ostream.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"

#include <llvm/Support/Debug.h>
#define DEBUG_TYPE "subop-sip"
namespace {
using namespace lingodb::compiler::dialect;

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

      getOperation()->walk([&](subop::ExecutionGroupOp eg) {
         llvm::SmallVector<subop::GetExternalOp> tables;
         llvm::SmallVector<subop::ScanRefsOp> scans;
         llvm::SmallVector<subop::LookupOp> lookups;
         llvm::SmallVector<subop::CreateHashIndexedView> hashViews;

         eg.walk([&](subop::GetExternalOp op) { tables.push_back(op); });
         eg.walk([&](subop::ScanRefsOp op) { scans.push_back(op); });
         eg.walk([&](subop::LookupOp op) { lookups.push_back(op); });
         eg.walk([&](subop::CreateHashIndexedView op) { hashViews.push_back(op); });
         llvm::dbgs() << "[SIP] ExecutionGroup at " << eg.getLoc() << "\n";

         // Tables accessed
         for (auto t : tables) {
            // GetExternalOp stores its description in the 'descr' property
            llvm::dbgs() << "  table: descr=" << t.getDescr() << " type=" << t.getResult().getType() << "\n";
         }

         // Scan outputs (potential build/probe sources)
         for (auto scan : scans) {
            llvm::dbgs() << "  scan_refs " << scan.getOperation() << " creates: ";
            for (auto* col : created.getCreatedColumns(scan.getOperation())) {
               llvm::dbgs() << col->type << " ";
            }
            llvm::dbgs() << "\n";
         }

         // Hash table layouts (what gets stored)
         for (auto hv : hashViews) {
            // CreateHashIndexedView has a source operand and attributes for hash/link members.
            llvm::dbgs() << "  hash_view source=" << hv.getSource() << " hashMember=" << hv.getHashMember().getMember() << " linkMember=" << hv.getLinkMember().getMember() << " layout=" << hv.getResult().getType() << "\n";
         }

         // Lookup keys and their producers (join keys)
         for (auto lookup : lookups) {
            llvm::dbgs() << "  lookup " << lookup.getOperation() << " uses columns: ";
            for (auto* col : used.getUsedColumns(lookup.getOperation())) {
               mlir::Operation* producer = created.getColumnCreator(col);
               if (producer)
                  llvm::dbgs() << col->type << " (producer=" << producer->getName() << ") ";
               else
                  llvm::dbgs() << col->type << " (producer=null) ";
            }
            llvm::dbgs() << "\n";
         }

         // Structured summary: for each hash view, print linked lookups and probe producers
         for (auto hv : hashViews) {
            llvm::dbgs() << "  [SIP-SUMMARY] view=" << hv << " source=" << hv.getSource() << " hashMember=" << hv.getHashMember().getMember() << " valueMembers=[";
            auto written = hv.getWrittenMembers();
            auto& memberManager = hv.getContext()->getOrLoadDialect<subop::SubOperatorDialect>()->getMemberManager();
            for (size_t i = 0; i < written.size(); ++i) {
               llvm::dbgs() << memberManager.getName(written[i]);
               if (i + 1 < written.size()) llvm::dbgs() << ",";
            }
            llvm::dbgs() << "]\n";

            // find lookups that reference this view (operand match)
            for (auto lk : lookups) {
               // lookup operand 1 is the view
               if (lk.getOperand(1) == hv.getResult()) {
                  llvm::dbgs() << "    lookup=" << lk << " uses probe columns: ";
                  for (auto* col : used.getUsedColumns(lk.getOperation())) {
                     mlir::Operation* producer = created.getColumnCreator(col);
                     if (producer)
                        llvm::dbgs() << col->type << " (producer=" << producer->getName() << ") ";
                     else
                        llvm::dbgs() << col->type << " (producer=null) ";
                  }
                  llvm::dbgs() << "\n";
               }
            }
         }
      });
   }
};

} // namespace
std::unique_ptr<mlir::Pass>
subop::createSIPPass() { return std::make_unique<SIPPass>(); }