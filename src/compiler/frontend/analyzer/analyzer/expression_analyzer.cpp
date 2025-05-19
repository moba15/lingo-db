#include "lingodb/compiler/frontend/analyzer/analyzer/expression_analyzer.h"
namespace lingodb::analyzer {
void ExpressionAnalyzer::analyze(std::shared_ptr<ast::AstNode> rootNode, std::shared_ptr<SQLContext> context) {
   if (rootNode->nodeType == ast::NodeType::EXPRESSION) {
      auto expr = std::static_pointer_cast<ast::ParsedExpression>(rootNode);
      if (expr->exprClass == ast::ExpressionClass::COMPARISON) {
         auto comp = std::static_pointer_cast<ast::ComparisonExpression>(expr);
         analyzeComparisonExpression(comp, context);
      } else if (expr->exprClass == ast::ExpressionClass::CONSTANT) {
         auto constExpr = std::static_pointer_cast<ast::ConstantExpression>(expr);
         analyzeConstExpression(constExpr, context);
      } else if (expr->exprClass == ast::ExpressionClass::COLUMN_REF) {
         auto columnRef = std::static_pointer_cast<ast::ColumnRefExpression>(expr);
         analyzeColumnRefExpression(columnRef, context);
      } else if (expr->exprClass == ast::ExpressionClass::CONJUNCTION) {
         auto conjunction = std::static_pointer_cast<ast::ConjunctionExpression>(expr);
         analyzeConjunctionExpression(conjunction, context);
      } else if (expr->exprClass == ast::ExpressionClass::STAR) {
         auto star = std::static_pointer_cast<ast::StarExpression>(expr);
         analyzeStarRefExpression(star, context);
      } else if (expr->exprClass == ast::ExpressionClass::FUNCTION) {
         if (expr->type == ast::ExpressionType::AGGREGATE) {
            analyzeAggregationFunctionExpression(std::static_pointer_cast<ast::FunctionExpression>(expr), context);

         } else {
            throw std::runtime_error("Not implemented");
         }
      } else {
         throw std::runtime_error("Not implemented");
      }
   }
}

void ExpressionAnalyzer::analyzeComparisonExpression(std::shared_ptr<ast::ComparisonExpression> comparison, std::shared_ptr<SQLContext> context) {
   analyze(comparison->left, context);
   analyze(comparison->right, context);
   if (!comparison->left->resultType.has_value()) {
      error("Left side of comparison is not a valid expression", comparison->left->loc);
      return;
   }
   if (!comparison->right->resultType.has_value()) {
      error("Right side of comparison is not a valid expression", comparison->right->loc);
      return;
   }
   if (comparison->left->resultType.value().getTypeId() != comparison->right->resultType.value().getTypeId()) {
      error("Comparison is not possible between " + comparison->left->resultType.value().toString() + " and " + comparison->right->resultType.value().toString(), comparison->loc);
      return;
   }
   comparison->resultType = catalog::Type::boolean();
}

void ExpressionAnalyzer::analyzeColumnRefExpression(std::shared_ptr<ast::ColumnRefExpression> columnRef, std::shared_ptr<SQLContext> context) {
   auto columnName = columnRef->column_names.size() == 1 ? columnRef->column_names[0] : columnRef->column_names[1];
   std::vector<catalog::Column> columns;
   std::string scope;
   if (columnRef->column_names.size() == 2) {
      auto foundColumn = context->findColumn(columnRef->column_names[1], columnRef->column_names[0]);
      columns = foundColumn.second;
      scope = foundColumn.first;
   } else if (columnRef->column_names.size() == 1) {
      auto foundColumn = context->findColumn(columnRef->column_names[0]);
      columns = foundColumn.second;
      scope = foundColumn.first;
   } else {
      throw std::runtime_error("Not implemented");
   }

   if (columns.empty()) {
      //TODO check function
      auto function = context->findFunction(columnRef->column_names[0]);
      if (function.second) {
         columnRef->scope = function.second->scope;
         //TODO type!
         columnRef->resultType = catalog::Type::int64();
         columnRef->refsAggregationFunction = true;
         columnRef->fName = function.second->name;
         return;
      }
      if (columnRef->column_names.size() == 1)
         error("No column found with name " + columnName, columnRef->loc);
      else
         error("No column found with name " + columnRef->column_names[0] + "." + columnName, columnRef->loc);
      return;
   }
   if (columns.size() > 1) {
      error(columnName + " is ambiguous", columnRef->loc);
      return;
   }
   columnRef->columns = columns;
   columnRef->resultType = columns.at(0).getLogicalType();
   columnRef->scope = std::move(scope);
}

void ExpressionAnalyzer::analyzeStarRefExpression(const std::shared_ptr<ast::StarExpression> star, const std::shared_ptr<SQLContext> context) {
   std::vector<catalog::Catalog> catalogs;
   std::string scope;
   if (star->relationName.empty()) {
      star->columns = std::move(context->getColumns());
   } else {
      star->columns = std::move(context->getColumns(star->relationName));
   }
}

void ExpressionAnalyzer::analyzeConstExpression(std::shared_ptr<ast::ConstantExpression> constExpr, std::shared_ptr<SQLContext> context) {
   //TODO set correct type
   if (!constExpr->value) {
      throw std::runtime_error("Value of constExpr is empty");
   }
   switch (constExpr->value->type) {
      case ast::ConstantType::INT:
         constExpr->resultType = catalog::Type::int64();
         break;
      case ast::ConstantType::STRING:
         constExpr->resultType = catalog::Type::stringType();
         break;
      default:
         throw std::runtime_error("Not implemented");
   }
}

void ExpressionAnalyzer::analyzeConjunctionExpression(std::shared_ptr<ast::ConjunctionExpression> conjunction, std::shared_ptr<SQLContext> context) {
   for (auto expr : conjunction->children) {
      analyze(expr, context);
      if (!expr->resultType.has_value() || expr->resultType.value().getTypeId() != catalog::Type::boolean().getTypeId()) {
         error("Conjunction is not possible with children of type boolean", expr->loc);
         return;
      }
   }
   conjunction->resultType = catalog::Type::boolean();
}

void ExpressionAnalyzer::analyzeAggregationFunctionExpression(std::shared_ptr<ast::FunctionExpression> function, std::shared_ptr<SQLContext> context) {
   //TODO Better
   if (!function->scope.empty()) {
      return;
   }
   for (auto arg : function->arguments) {
      analyze(arg, context);
   }
   function->scope = createTmpScope();
   auto fName = function->alias.empty() ? function->functionName : function->alias;
   context->currentScope->functionsEntry.emplace(fName, std::make_shared<ast::FunctionInfo>(function->scope, fName));
}

void ExpressionAnalyzer::error(std::string message, lingodb::location loc) {
   std::ostringstream s{};
   s << message << " at " << loc << std::endl;
   throw std::runtime_error(s.str());
}
} // namespace lingodb::analyzer