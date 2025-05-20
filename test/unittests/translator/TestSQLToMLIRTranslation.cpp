#include "lingodb/compiler/Dialect/DB/IR/DBDialect.h"
#include "lingodb/compiler/Dialect/RelAlg/IR/RelAlgDialect.h"
#include "lingodb/compiler/Dialect/SubOperator/SubOperatorDialect.h"
#include "lingodb/compiler/Dialect/SubOperator/SubOperatorOps.h"
#include "lingodb/compiler/Dialect/TupleStream/TupleStreamDialect.h"
#include "lingodb/compiler/Dialect/util/UtilDialect.h"
#include "lingodb/compiler/frontend/SQL/Parser.h"
#include "lingodb/compiler/frontend/analyzer/sql_analyzer.h"
#include "lingodb/compiler/frontend/sql-parser/driver.h"
#include "lingodb/compiler/frontend/translator/sql_mlir_translator.h"
#include "lingodb/runtime/Session.h"

#include "mlir/IR/BuiltinDialect.h"
#include <catch2/catch_test_macros.hpp>

namespace {
using namespace lingodb;
using namespace lingodb::utility;
using namespace lingodb::catalog;
auto createTableEntry() {
   CreateTableDef createTableDef;
   createTableDef.name = "testtable";
   createTableDef.columns = {Column("col1", Type::int8(), true), Column("col2", Type::stringType(), false)};
   createTableDef.primaryKey = {"col1"};
   return LingoDBTableCatalogEntry::createFromCreateTable(createTableDef);
}
auto createIndexEntry() {
   return LingoDBHashIndexEntry::createForPrimaryKey("testtable", {"col1"});
}

auto makeSQLAnalyzer(std::shared_ptr<Catalog> catalog) {
   analyzer::SQLQueryAnalyzer sqlAnalyzer{catalog};
   auto sqlContext = std::make_shared<lingodb::analyzer::SQLContext>();
   sqlContext->catalog = catalog;
   return sqlAnalyzer;
}

std::pair<std::shared_ptr<analyzer::SQLContext>, std::shared_ptr<ast::QueryNode>> analyze(std::string sql, std::shared_ptr<Catalog> catalog) {
   auto analyzer = makeSQLAnalyzer(catalog);
   driver drv;
   REQUIRE(drv.parse(":" + sql) == 0);
   analyzer.analyze(drv.result);

   return std::make_pair(analyzer.context, drv.result);
}

auto translateNew(std::string sql, std::shared_ptr<Catalog> catalog) {
   auto result = analyze(sql, catalog);
   mlir::MLIRContext context;
   mlir::DialectRegistry registry;
   registry.insert<mlir::BuiltinDialect>();
   registry.insert<lingodb::compiler::dialect::relalg::RelAlgDialect>();
   registry.insert<lingodb::compiler::dialect::subop::SubOperatorDialect>();
   registry.insert<lingodb::compiler::dialect::tuples::TupleStreamDialect>();
   registry.insert<lingodb::compiler::dialect::db::DBDialect>();
   registry.insert<mlir::func::FuncDialect>();
   registry.insert<mlir::arith::ArithDialect>();

   registry.insert<mlir::memref::MemRefDialect>();
   registry.insert<lingodb::compiler::dialect::util::UtilDialect>();
   registry.insert<mlir::scf::SCFDialect>();
   registry.insert<mlir::LLVM::LLVMDialect>();
   context.appendDialectRegistry(registry);
   context.loadAllAvailableDialects();
   context.loadDialect<lingodb::compiler::dialect::relalg::RelAlgDialect>();
   mlir::OpBuilder builder(&context);
   mlir::ModuleOp moduleOp = builder.create<mlir::ModuleOp>(builder.getUnknownLoc());
   builder.setInsertionPointToStart(moduleOp.getBody());
   lingodb::translator::SQLMlirTranslator translator{moduleOp, catalog};
   auto* queryBlock = new mlir::Block;
   {
      mlir::OpBuilder::InsertionGuard guard(builder);
      builder.setInsertionPointToStart(queryBlock);
      auto val = translator.translate(builder, result.second, result.first);
      if (val.has_value()) {
         builder.create<lingodb::compiler::dialect::subop::SetResultOp>(builder.getUnknownLoc(), 0, val.value());
      }
      builder.create<mlir::func::ReturnOp>(builder.getUnknownLoc());
   }
   mlir::func::FuncOp funcOp = builder.create<mlir::func::FuncOp>(builder.getUnknownLoc(), "main", builder.getFunctionType({}, {}));
   funcOp.getBody().push_back(queryBlock);

   mlir::OpPrintingFlags flags;
   flags.assumeVerified();
   std::string output;
   llvm::raw_string_ostream os(output);
   moduleOp->print(os, flags);
   os.flush();
   moduleOp.erase();

   return output;
}

auto translateOld(std::string sql, std::shared_ptr<Catalog> catalog) {
   mlir::MLIRContext context;
   mlir::DialectRegistry registry;
   registry.insert<mlir::BuiltinDialect>();
   registry.insert<lingodb::compiler::dialect::relalg::RelAlgDialect>();
   registry.insert<lingodb::compiler::dialect::subop::SubOperatorDialect>();
   registry.insert<lingodb::compiler::dialect::tuples::TupleStreamDialect>();
   registry.insert<lingodb::compiler::dialect::db::DBDialect>();
   registry.insert<mlir::func::FuncDialect>();
   registry.insert<mlir::arith::ArithDialect>();

   registry.insert<mlir::memref::MemRefDialect>();
   registry.insert<lingodb::compiler::dialect::util::UtilDialect>();
   registry.insert<mlir::scf::SCFDialect>();
   registry.insert<mlir::LLVM::LLVMDialect>();
   context.appendDialectRegistry(registry);
   context.loadAllAvailableDialects();
   context.loadDialect<lingodb::compiler::dialect::relalg::RelAlgDialect>();
   mlir::OpBuilder builder(&context);
   mlir::ModuleOp moduleOp = builder.create<mlir::ModuleOp>(builder.getUnknownLoc());
   lingodb::compiler::frontend::sql::Parser translator(sql, *catalog, moduleOp);

   builder.setInsertionPointToStart(moduleOp.getBody());
   auto* queryBlock = new mlir::Block;
   {
      mlir::OpBuilder::InsertionGuard guard(builder);
      builder.setInsertionPointToStart(queryBlock);
      auto val = translator.translate(builder);
      if (val.has_value()) {
         builder.create<compiler::dialect::subop::SetResultOp>(builder.getUnknownLoc(), 0, val.value());
      }
      builder.create<mlir::func::ReturnOp>(builder.getUnknownLoc());
   }
   mlir::func::FuncOp funcOp = builder.create<mlir::func::FuncOp>(builder.getUnknownLoc(), "main", builder.getFunctionType({}, {}));
   funcOp.getBody().push_back(queryBlock);

   mlir::OpPrintingFlags flags;
   flags.assumeVerified();
   std::string output;
   llvm::raw_string_ostream os(output);
   moduleOp->print(os, flags);
   os.flush();
   moduleOp.erase();

   return output;
}

TEST_CASE("Translation:simpleSelect") {
   auto catalog = Catalog::createEmpty();
   auto tableEntry = createTableEntry();
   catalog->insertEntry(tableEntry);
   auto indexEntry = createIndexEntry();
   catalog->insertEntry(indexEntry);
   std::string sql = "select col1,col2 from testtable";
   auto result = translateNew(sql, catalog);
   auto expected = translateOld(sql, catalog);
   REQUIRE(result == expected);
}

TEST_CASE("Translation:simpleSelect2") {
   auto catalog = Catalog::createEmpty();
   auto tableEntry = createTableEntry();
   catalog->insertEntry(tableEntry);
   auto indexEntry = createIndexEntry();
   catalog->insertEntry(indexEntry);
   std::string sql = "select * from testtable";
   auto result = translateNew(sql, catalog);
   auto expected = translateOld(sql, catalog);
   REQUIRE(result == expected);
}

TEST_CASE("Translation:simpleSelect3") {
   auto catalog = Catalog::createEmpty();
   auto tableEntry = createTableEntry();
   catalog->insertEntry(tableEntry);
   auto indexEntry = createIndexEntry();
   catalog->insertEntry(indexEntry);
   std::string sql = "select t.* from testtable t";
   std::string expected{"module {"
                        "func.func @main() {"
                        "%0 = relalg.query (){"
                        "%1 = relalg.basetable  {table_identifier = \"testtable\"} columns: {col1 => @t::@col1({type = !db.nullable<i8>}), col2 => @t::@col2({type = !db.string})}"
                        "%2 = relalg.materialize %1 [@t::@col1,@t::@col2] => [\"col1\", \"col2\"] : !subop.local_table<[col1$0 : !db.nullable<i8>, col2$0 : !db.string], [\"col1\", \"col2\"]>"
                        "relalg.query_return %2 : !subop.local_table<[col1$0 : !db.nullable<i8>, col2$0 : !db.string], [\"col1\", \"col2\"]>"
                        "} -> !subop.local_table<[col1$0 : !db.nullable<i8>, col2$0 : !db.string], [\"col1\", \"col2\"]>"
                        "subop.set_result 0 %0 : !subop.local_table<[col1$0 : !db.nullable<i8>, col2$0 : !db.string], [\"col1\", \"col2\"]>"
                        "return"
                        "}"
                        "}"};

   auto result = translateNew(sql, catalog);
   std::erase(expected, ' ');

   std::erase(result, '\n');
   std::erase(result, ' ');
   REQUIRE(result == expected);
}

TEST_CASE("Translation:simpleSelectAlias") {
   auto catalog = Catalog::createEmpty();
   auto tableEntry = createTableEntry();
   catalog->insertEntry(tableEntry);
   auto indexEntry = createIndexEntry();
   catalog->insertEntry(indexEntry);
   std::string sql = "select col1 as c from testtable";
   auto result = translateNew(sql, catalog);
   auto expected = translateOld(sql, catalog);
   REQUIRE(result == expected);
}
TEST_CASE("Translation:simpleSelectAlias2") {
   auto catalog = Catalog::createEmpty();
   auto tableEntry = createTableEntry();
   catalog->insertEntry(tableEntry);
   auto indexEntry = createIndexEntry();
   catalog->insertEntry(indexEntry);
   std::string sql = "select t.col1 as c from testtable as t";
   auto result = translateNew(sql, catalog);
   auto expected = translateOld(sql, catalog);
   REQUIRE(result == expected);
}

TEST_CASE("Translation:simpleSelectConstantWhere") {
   auto catalog = Catalog::createEmpty();
   auto tableEntry = createTableEntry();
   catalog->insertEntry(tableEntry);
   auto indexEntry = createIndexEntry();
   catalog->insertEntry(indexEntry);
   std::string sql = "select t.col1 as c from testtable as t where 1";
   auto result = translateNew(sql, catalog);
   auto expected = translateOld(sql, catalog);
   REQUIRE(result == expected);
}

TEST_CASE("Translation:simpleSelectWhereSingleExpression") {
   auto catalog = Catalog::createEmpty();
   auto tableEntry = createTableEntry();
   catalog->insertEntry(tableEntry);
   auto indexEntry = createIndexEntry();
   catalog->insertEntry(indexEntry);
   std::string sql = "select t.col1 from testtable as t where t.col1=1 and t.col2='dfssdsff'";
   auto result = translateNew(sql, catalog);
   auto expected = translateOld(sql, catalog);
   REQUIRE(result == expected);
}

TEST_CASE("Translation:simplePipeSQL") {
   auto catalog = Catalog::createEmpty();
   auto tableEntry = createTableEntry();
   catalog->insertEntry(tableEntry);
   auto indexEntry = createIndexEntry();
   catalog->insertEntry(indexEntry);
   std::string sql = "from testtable |> select col1";
   auto result = translateNew(sql, catalog);
   std::string expected{"module {"
                        "func.func @main() {"
                        "%0 = relalg.query (){"
                        "%1 = relalg.basetable  {table_identifier = \"testtable\"} columns: {col1 => @testtable::@col1({type = !db.nullable<i8>}), col2 => @testtable::@col2({type = !db.string})}"
                        "%2 = relalg.materialize %1 [@testtable::@col1] => [\"col1\"] : !subop.local_table<[col1$0 : !db.nullable<i8>], [\"col1\"]>"
                        "relalg.query_return %2 : !subop.local_table<[col1$0 : !db.nullable<i8>], [\"col1\"]>"
                        "} -> !subop.local_table<[col1$0 : !db.nullable<i8>], [\"col1\"]>"
                        "subop.set_result 0 %0 : !subop.local_table<[col1$0 : !db.nullable<i8>], [\"col1\"]>"
                        "return"
                        "}"
                        "}"};
   std::erase(expected, ' ');

   std::erase(result, '\n');
   std::erase(result, ' ');
   REQUIRE(result == expected);
}

TEST_CASE("Translation:simplePipeSQL2") {
   auto catalog = Catalog::createEmpty();
   auto tableEntry = createTableEntry();
   catalog->insertEntry(tableEntry);
   auto indexEntry = createIndexEntry();
   catalog->insertEntry(indexEntry);
   std::string sql = "from testtable |> select col1 |> where col1<1 and col1>=2 |> where col2='sdf' |> select *";
   auto result = translateNew(sql, catalog);
   std::string expected{"module{func.func@main(){%0=relalg.query(){%1=relalg.basetable{table_identifier=\"testtable\"}columns:{col1=>@testtable::@col1({type=!db.nullable<i8>}),col2=>@testtable::@col2({type=!db.string})}%2=relalg.selection%1(%arg0:!tuples.tuple){%5=tuples.getcol%arg0@testtable::@col1:!db.nullable<i8>%6=db.constant(1:i32):i32%7=db.cast%5:!db.nullable<i8>->!db.nullable<i32>%8=db.comparelt%7:!db.nullable<i32>,%6:i32%9=tuples.getcol%arg0@testtable::@col1:!db.nullable<i8>%10=db.constant(2:i32):i32%11=db.cast%9:!db.nullable<i8>->!db.nullable<i32>%12=db.comparegte%11:!db.nullable<i32>,%10:i32%13=db.and%8,%12:!db.nullable<i1>,!db.nullable<i1>tuples.return%13:!db.nullable<i1>}%3=relalg.selection%2(%arg0:!tuples.tuple){%5=tuples.getcol%arg0@testtable::@col2:!db.string%6=db.constant(\"sdf\"):!db.string%7=db.compareeq%5:!db.string,%6:!db.stringtuples.return%7:i1}%4=relalg.materialize%3[@testtable::@col1,@testtable::@col2]=>[\"col1\",\"col2\"]:!subop.local_table<[col1$0:!db.nullable<i8>,col2$0:!db.string],[\"col1\",\"col2\"]>relalg.query_return%4:!subop.local_table<[col1$0:!db.nullable<i8>,col2$0:!db.string],[\"col1\",\"col2\"]>}->!subop.local_table<[col1$0:!db.nullable<i8>,col2$0:!db.string],[\"col1\",\"col2\"]>subop.set_result0%0:!subop.local_table<[col1$0:!db.nullable<i8>,col2$0:!db.string],[\"col1\",\"col2\"]>return}}"};
   std::erase(expected, ' ');

   std::erase(result, '\n');
   std::erase(result, ' ');
   REQUIRE(result == expected);
}

} // namespace