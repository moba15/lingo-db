//
// Created by mor on 06.01.25.
//

#include "lingodb/server/parser/ParaParser.h"
#include "lingodb/execution/Frontend.h"
#include "mlir/Dialect/Async/IR/Async.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/InitAllExtensions.h"
#include <llvm/Support/SourceMgr.h>
#include <arrow/table.h>
#include <lingodb/runtime/ExecutionContext.h>
namespace server {

ParaParser::ParaParser(const std::shared_ptr<lingodb::runtime::Session>& session) : session(session) {}

std::shared_ptr<StatementInformation> ParaParser::getStatementInformation(std::string sql) {
   mlir::MLIRContext context;
   lingodb::execution::initializeContext(context);
   mlir::OpBuilder builder(&context);

   mlir::ModuleOp moduleOp = builder.create<mlir::ModuleOp>(builder.getUnknownLoc());
   lingodb::compiler::frontend::sql::Parser translator(sql, *session->getCatalog(), moduleOp);
   auto type = getStatementType(translator);
   auto statementInformation = std::make_shared<StatementInformation>(type);
   if (type == StatementType::AD_HOC_QUERY) {
      //Get used relations
      auto* statement = static_cast<Node*>(translator.result.tree->head->data.ptr_value);
      auto relations = getUsedRelationsFromSelectStatement(reinterpret_cast<SelectStmt*>(statement));
      statementInformation->relations = std::move(relations);
   }

   return std::move(statementInformation);
}

std::unique_ptr<std::vector<std::shared_ptr<arrow::Field>>> ParaParser::findParasInSelectStatement(SelectStmt* selectStatement) {
   if (selectStatement->op_ != SETOP_NONE) {
      throw std::runtime_error("ParaParser::getParas(): invalid node op_ type");
   }

   if (selectStatement->where_clause_) {
      Node* current = selectStatement->where_clause_;
      auto result = findParas(selectStatement->where_clause_);
      return std::move(result);
   }
   return std::make_unique<std::vector<std::shared_ptr<arrow::Field>>>();
}
std::unique_ptr<std::vector<std::shared_ptr<arrow::Field>>> ParaParser::getParas(std::string sql) {
   mlir::MLIRContext context;
   lingodb::execution::initializeContext(context);
   mlir::OpBuilder builder(&context);

   mlir::ModuleOp moduleOp = builder.create<mlir::ModuleOp>(builder.getUnknownLoc());
   lingodb::compiler::frontend::sql::Parser translator(sql, *session->getCatalog(), moduleOp);
   auto* statement = static_cast<Node*>(translator.result.tree->head->data.ptr_value);
   if (statement->type != T_SelectStmt) { return {}; }
   return std::move(findParasInSelectStatement(reinterpret_cast<SelectStmt*>(statement)));
}
server::StatementType ParaParser::getStatementType(lingodb::compiler::frontend::sql::Parser& translator) {
   auto* statement = static_cast<Node*>(translator.result.tree->head->data.ptr_value);
   if (statement->type == T_SelectStmt) { return StatementType::AD_HOC_QUERY; }
   if (statement->type == T_UpdateStmt) {
      return StatementType::AD_HOC_UPDATE;
   }
   if (statement->type == T_InsertStmt) {
      return StatementType::AD_HOC_UPDATE;
   }
   throw std::runtime_error("ParaParser::getParas(): invalid node type");
}
std::unique_ptr<std::vector<std::shared_ptr<lingodb::runtime::Relation>>> ParaParser::getUsedRelationsFromSelectStatement(SelectStmt* selectStatement) {
   auto result = std::make_unique<std::vector<std::shared_ptr<lingodb::runtime::Relation>>>();
   auto fromClause = selectStatement->from_clause_;
   for (auto* cell = fromClause->head; cell != nullptr; cell = cell->next) {
      auto* node = reinterpret_cast<Node*>(cell->data.ptr_value);
      switch (node->type) {
         case T_RangeVar: {
            auto* range = reinterpret_cast<RangeVar*>(node);
            auto* relName = range->relname_;

            result->emplace_back(this->session->getCatalog()->findRelation(relName));

            break;
         }
         case T_RangeSubselect: {
            auto* range = reinterpret_cast<RangeSubselect*>(node);
            auto r = getUsedRelationsFromSelectStatement(reinterpret_cast<SelectStmt*>(range->subquery_));
            for (auto relation : *r) {
               result->emplace_back(relation);
            }
            break;
         }
         case T_JoinExpr: {
            auto* joinExpr = reinterpret_cast<JoinExpr*>(node);

            break;
         }
         default: {
            throw std::runtime_error("ParaParser::getRelNamesFromSelectStatement(): invalid node type");
         }
      }
   }
   return result;
}

std::unique_ptr<std::vector<std::shared_ptr<arrow::Field>>> ParaParser::findParas(Node* currentNode) {
   auto fieldsVector = std::make_unique<std::vector<std::shared_ptr<arrow::Field>>>();
   if (currentNode->type == T_A_Expr) {
      auto* expr = reinterpret_cast<A_Expr*>(currentNode);
      if (expr->kind_ == AEXPR_OP) {
         Node* left = expr->lexpr_;
         Node* right = expr->rexpr_;
         //Right expr should include para
         if (right->type == T_ParamRef) {
            if (left->type == T_ColumnRef) {
               auto* colRef = reinterpret_cast<ColumnRef*>(left);
               auto fields = colRef->fields_;
               auto name = fieldsToString(fields);
               std::cout << fieldsToString(fields) << std::endl;
               auto field = getFieldOfColumn(name);
               fieldsVector->push_back(field);
            }
         } else if (right->type == T_SubLink) {
            auto* subLink = reinterpret_cast<SubLink*>(right);
            std::vector<std::shared_ptr<arrow::Field>> value1;
            auto f = findParasInSelectStatement(reinterpret_cast<SelectStmt*>(subLink->subselect_));
            for (auto fcurrent : value1) { fieldsVector->push_back(fcurrent); }
         }

         //Left should include column
      }

   } else if (currentNode->type == T_BoolExpr) {
      auto* boolExpr = reinterpret_cast<BoolExpr*>(currentNode);
      std::vector<mlir::Value> values;
      for (auto* cell = boolExpr->args_->head; cell != nullptr; cell = cell->next) {
         auto* node = reinterpret_cast<Node*>(cell->data.ptr_value);
         auto f = findParas(node);
         for (auto fcurrent : *f) { fieldsVector->push_back(fcurrent); }
      }
   }
   return std::move(fieldsVector);
}

std::string ParaParser::fieldsToString(List* fields) {
   auto* node = reinterpret_cast<Node*>(fields->head->data.ptr_value);
   std::string colName;
   std::string tableName;
   if (fields->length == 1) {
      colName = reinterpret_cast<value*>(node)->val_.str_;
      tableName = "";
   } else {
      auto* nextNode = reinterpret_cast<Node*>(fields->head->next->data.ptr_value);
      if (nextNode->type == T_A_Star) {
         throw std::runtime_error("unexpected *");
      } else {
         colName = reinterpret_cast<value*>(nextNode)->val_.str_;
      }

      tableName = reinterpret_cast<value*>(node)->val_.str_;
   }
   return tableName.empty() ? colName : tableName + "." + colName;
}
std::shared_ptr<arrow::Field> ParaParser::getFieldOfColumn(std::string columnName) {
   std::vector<std::shared_ptr<lingodb::runtime::Relation>> relations{session->getCatalog()->findRelation("hoeren"), session->getCatalog()->findRelation("vorlesungen")};
   for (auto& relation : relations) {
      auto fields = relation->getArrowSchema()->fields();
      for (auto field : fields) {
         if (field->name() == columnName) { return field; }
      }
   }

   throw std::runtime_error("could not find field " + columnName);
}
}