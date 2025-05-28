#pragma once
#include "lingodb/compiler/frontend/sql-parser/ast_node.h"
#include "lingodb/compiler/frontend/sql-parser/parsed_expression.h"
namespace lingodb::ast {
class BoundExpression : public AstNode {
   public:
   //TODO make enums sense
   BoundExpression(ExpressionClass exprClass, ExpressionType type) : AstNode(NodeType::BOUND_EXPRESSION), exprClass(exprClass), type(type) {}
   BoundExpression(ExpressionClass exprClass, ExpressionType type, catalog::Type resultType) : AstNode(NodeType::BOUND_EXPRESSION), exprClass(exprClass), type(type), resultType(catalog::NullableType(resultType)) {}
   BoundExpression(ExpressionClass exprClass, ExpressionType type, catalog::NullableType resultType) : AstNode(NodeType::BOUND_EXPRESSION), exprClass(exprClass), type(type), resultType(resultType) {}

   ExpressionClass exprClass;
   ExpressionType type;
   //! The alias of the expression
   std::string alias;

   std::optional<catalog::NullableType> resultType = std::nullopt;
};

class BoundColumnRefExpression : public BoundExpression {
   public:
   static constexpr ExpressionClass TYPE = ExpressionClass::BOUND_COLUMN_REF;

   //! Specify both the column and table name
   BoundColumnRefExpression(std::string scope, catalog::NullableType resultType, catalog::Column boundColumn);

   //TODO semenatic
   std::string scope;
   catalog::Column boundColumn;
   //TODO type etc

   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;
};

class BoundComparisonExpression : public BoundExpression {
   public:
   static constexpr const ExpressionClass TYPE = ExpressionClass::BOUND_COMPARISON;

   explicit BoundComparisonExpression(ExpressionType type);
   BoundComparisonExpression(ExpressionType type, std::shared_ptr<BoundExpression> left, std::shared_ptr<BoundExpression> right);

   std::shared_ptr<BoundExpression> left;
   std::shared_ptr<BoundExpression> right;

   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;
};

class BoundConjunctionExpression : public BoundExpression {
   public:
   static constexpr const ExpressionClass TYPE = ExpressionClass::BOUND_CONJUNCTION;
   BoundConjunctionExpression(ExpressionType type, std::shared_ptr<BoundExpression> left, std::shared_ptr<BoundExpression> right);
   BoundConjunctionExpression(ExpressionType type, std::vector<std::shared_ptr<BoundExpression>> children);

   std::vector<std::shared_ptr<BoundExpression>> children;

   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;

   private:
   std::string typeToAscii(ExpressionType type) const;
};

class BoundConstantExpression : public BoundExpression {
   public:
   static constexpr ExpressionClass TYPE = ExpressionClass::BOUND_CONSTANT;
   BoundConstantExpression(catalog::Type resultType, std::shared_ptr<Value> value);

   std::shared_ptr<Value> value;

   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;
};

//TODO other

class BoundTargetsExpression : public BoundExpression {
   public:
   static constexpr ExpressionClass TYPE = ExpressionClass::BOUND_TARGETS;
   BoundTargetsExpression(std::vector<std::shared_ptr<BoundExpression>> targets, std::vector<std::pair<std::string, catalog::Column>> targetColumns);

   std::vector<std::shared_ptr<BoundExpression>> targets;
   std::vector<std::pair<std::string, catalog::Column>> targetColumns;

   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;
};

class BoundFunctionExpression : public BoundExpression {
   public:
   static constexpr const ExpressionClass TYPE = ExpressionClass::BOUND_FUNCTION;
   BoundFunctionExpression(ExpressionType type, catalog::Type resultType, std::string functionName, std::string scope, std::string alias, std::vector<std::shared_ptr<BoundExpression>> arguments);

   std::string functionName;
   std::string scope;
   std::string alias;
   std::vector<std::shared_ptr<BoundExpression>> arguments;

   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;
};

class BoundStarExpression : public BoundExpression {
   public:
   static constexpr const ExpressionClass TYPE = ExpressionClass::STAR;
   explicit BoundStarExpression(std::string scope, std::vector<std::pair<std::string, catalog::Column>> columns);

   std::string scope;
   std::vector<std::pair<std::string, catalog::Column>> columns{};

   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;
};

class BoundOperatorExpression : public BoundExpression {
   public:
   static constexpr const ExpressionClass TYPE = ExpressionClass::BOUND_OPERATOR;
   BoundOperatorExpression(ExpressionType type, catalog::Type resultType, std::vector<std::shared_ptr<BoundExpression>> children);
   BoundOperatorExpression(ExpressionType type, catalog::Type resultType, std::shared_ptr<BoundExpression> left, std::shared_ptr<BoundExpression> right);

   std::vector<std::shared_ptr<BoundExpression>> children;
   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;
};

class BoundCastExpression : public BoundExpression {
   public:
   static constexpr const ExpressionClass TYPE = ExpressionClass::BOUND_CAST;
   BoundCastExpression(catalog::Type resultType, std::shared_ptr<BoundExpression> child, LogicalType logicalType, std::optional<TypeMods> typeMods);
   std::optional<TypeMods> typeMods;
   std::shared_ptr<BoundExpression> child;
   LogicalType logicalType;
   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;
};

} // namespace lingodb::ast