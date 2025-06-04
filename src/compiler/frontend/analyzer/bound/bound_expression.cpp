#include "lingodb/compiler/frontend/analyzer/bound/bound_expression.h"
namespace lingodb::ast {
/*
 * BoundColumnRefExpression
*/
BoundColumnRefExpression::BoundColumnRefExpression(std::string scope, catalog::NullableType resultType, std::shared_ptr<NamedResult> namedResult) : BoundExpression(TYPE, ExpressionType::BOUND_COLUMN_REF, resultType), scope(scope), namedResult(std::move(namedResult)) {
}
std::string BoundColumnRefExpression::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   return "";
}

/*
 * BoundComparisonExpression
*/
BoundComparisonExpression::BoundComparisonExpression(ExpressionType type) : BoundExpression(TYPE, type), left(nullptr), right(nullptr) {
}
BoundComparisonExpression::BoundComparisonExpression(ExpressionType type, std::shared_ptr<BoundExpression> left, std::shared_ptr<BoundExpression> right) : BoundExpression(TYPE, type, catalog::Type::boolean()), left(std::move(left)), right(std::move(right)) {
}
std::string BoundComparisonExpression::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   return "";
}

/*
 * BoundConjunctionExpression
*/
BoundConjunctionExpression::BoundConjunctionExpression(ExpressionType type, std::shared_ptr<BoundExpression> left, std::shared_ptr<BoundExpression> right) : BoundExpression(TYPE, type, catalog::Type::boolean()), children(std::vector{left, right}) {
   if (type != ExpressionType::CONJUNCTION_AND && type != ExpressionType::CONJUNCTION_OR) {
      throw std::runtime_error("Invalid type for BoundConjunctionExpression");
   }
}
BoundConjunctionExpression::BoundConjunctionExpression(ExpressionType type, std::vector<std::shared_ptr<BoundExpression>> children) : BoundExpression(TYPE, type, catalog::Type::boolean()), children(std::move(children)) {
}
std::string BoundConjunctionExpression::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   return "";
}

/*
 * BoundConstantExpression
*/
BoundConstantExpression::BoundConstantExpression(catalog::Type resultType, std::shared_ptr<Value> value) : BoundExpression(TYPE, ExpressionType::VALUE_CONSTANT, resultType), value(std::move(value)) {
}
std::string BoundConstantExpression::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   return "";
}

/*
 * BoundTargetsExpression
*/
BoundTargetsExpression::BoundTargetsExpression(std::vector<std::shared_ptr<BoundExpression>> targets, std::vector<std::shared_ptr<NamedResult>> targetColumns) : BoundExpression(TYPE, ExpressionType::BOUND_TARGETS), targets(std::move(targets)), targetColumns(std::move(targetColumns)) {
   for (const auto& target : this->targets) {
      if (target->type == ExpressionType::AGGREGATE && target->exprClass == ExpressionClass::FUNCTION) {
         //TODO handle aggregation
      }
   }
}
std::string BoundTargetsExpression::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   return "";
}

/*
 * BoundFunctionExpression
*/
BoundFunctionExpression::BoundFunctionExpression(ExpressionType type, catalog::Type resultType, std::string functionName, std::string scope, std::string aliasOrUniqueIdentifier, std::vector<std::shared_ptr<BoundExpression>> arguments, std::shared_ptr<FunctionInfo> functionInfo) : BoundExpression(TYPE, type, resultType), functionName(functionName), scope(scope), aliasOrUniqueIdentifier(aliasOrUniqueIdentifier), arguments(arguments), functionInfo(functionInfo) {
}
std::string BoundFunctionExpression::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   return "";
}

/*
 * BoundStarExpression
*/
BoundStarExpression::BoundStarExpression(std::string scope, std::vector<std::pair<std::string, catalog::Column>> columns) : BoundExpression(TYPE, ExpressionType::STAR), scope(scope), columns(std::move(columns)) {
}
std::string BoundStarExpression::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
}

/*
 * BoundComparisonExpression
*/
BoundOperatorExpression::BoundOperatorExpression(ExpressionType type, catalog::Type resultType, std::vector<std::shared_ptr<BoundExpression>> children) : BoundExpression(TYPE, type, resultType), children(children) {
}
BoundOperatorExpression::BoundOperatorExpression(ExpressionType type, catalog::Type resultType, std::shared_ptr<BoundExpression> left, std::shared_ptr<BoundExpression> right) : BoundExpression(TYPE, type, resultType), children({left, right}) {
}
std::string BoundOperatorExpression::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   return "";
}

/*
 * BoundCastExpression
*/
BoundCastExpression::BoundCastExpression(catalog::Type resultType, std::shared_ptr<BoundExpression> child, LogicalType logicalType, std::optional<TypeMods> typeMods) : BoundExpression(TYPE, ExpressionType::CAST, resultType), child(std::move(child)), logicalType(logicalType), typeMods(typeMods) {
}
std::string BoundCastExpression::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   return "";
}

/*
 * BoundBetweenExpression
 */
BoundBetweenExpression::BoundBetweenExpression(ExpressionType type, catalog::Type resultType, std::shared_ptr<BoundExpression> input, std::shared_ptr<BoundExpression> lower, std::shared_ptr<BoundExpression> upper) : BoundExpression(TYPE, type, resultType), input(std::move(input)), lower(std::move(lower)), upper(std::move(upper)) {
}
std::string BoundBetweenExpression::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   return "";
}
} // namespace lingodb::ast