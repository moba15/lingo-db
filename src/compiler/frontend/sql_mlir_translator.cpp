#include "lingodb/compiler/frontend/sql_mlir_translator.h"

#include "lingodb/catalog/Defs.h"
#include "lingodb/catalog/MLIRTypes.h"
#include "lingodb/catalog/TableCatalogEntry.h"
#include "lingodb/compiler/Dialect/RelAlg/Transforms/queryopt/QueryGraph.h"
#include "lingodb/compiler/Dialect/SubOperator/SubOperatorDialect.h"
#include "lingodb/compiler/Dialect/SubOperator/SubOperatorOps.h"
#include "lingodb/compiler/Dialect/TupleStream/TupleStreamOps.h"
#include "lingodb/compiler/Dialect/util/UtilDialect.h"
#include "lingodb/compiler/frontend/ast/aggregation_node.h"
#include "lingodb/compiler/frontend/ast/bound/bound_extend_node.h"
#include "lingodb/compiler/frontend/ast/bound/bound_tableref.h"
#include "lingodb/utility/Serialization.h"

//todo: remove
#include "lingodb/compiler/old-frontend/SQL/Parser.h"

#include <mlir-c/IR.h>
namespace lingodb::translator {
using namespace lingodb::compiler::dialect;
SQLMlirTranslator::SQLMlirTranslator(mlir::ModuleOp moduleOp, std::shared_ptr<catalog::Catalog> catalog) : moduleOp(moduleOp),
                                                                                                           attrManager(moduleOp->getContext()->getLoadedDialect<tuples::TupleStreamDialect>()->getColumnManager()), catalog(catalog), translationContext(std::make_shared<TranslationContext>())

{
   moduleOp.getContext()->getLoadedDialect<util::UtilDialect>()->getFunctionHelper().setParentModule(moduleOp);
}
std::optional<mlir::Value> SQLMlirTranslator::translateStart(mlir::OpBuilder& builder, std::shared_ptr<ast::TableProducer> tableProducer, std::shared_ptr<analyzer::SQLContext> context) {
   mlir::Block* block = new mlir::Block();
   mlir::Type localTableType;
   {
      mlir::OpBuilder::InsertionGuard guard(builder);
      builder.setInsertionPointToStart(block);

      auto tree = translateTableProducer(builder, tableProducer, context);

      context->currentScope->evalBeforeAggr.clear();
      std::vector<mlir::Attribute> attrs;
      std::vector<mlir::Attribute> names;
      std::vector<mlir::Attribute> colMemberNames;
      std::vector<mlir::Attribute> colTypes;
      auto& memberManager = builder.getContext()->getLoadedDialect<subop::SubOperatorDialect>()->getMemberManager();

      for (auto& named : context->currentScope->targetInfo.targetColumns) {
         switch (named->type) {
            case ast::NamedResultType::Column: {
               auto colMemberName = memberManager.getUniqueMember(named->displayName);
               names.push_back(builder.getStringAttr(named->displayName));
               auto columnInfo = std::static_pointer_cast<ast::ColumnInfo>(named);
               auto type = createTypeForColumn(builder.getContext(), columnInfo->column);
               colTypes.push_back(mlir::TypeAttr::get(type));
               colMemberNames.push_back(builder.getStringAttr(colMemberName));
               auto attrDef = columnInfo->createRef(attrManager);
               attrs.push_back(attrDef);
               break;
            }
            case ast::NamedResultType::Function: {
               auto functionInfo = std::static_pointer_cast<ast::FunctionInfo>(named);
               names.push_back(builder.getStringAttr(functionInfo->name));
               auto colMemberName = memberManager.getUniqueMember(functionInfo->name);
               colMemberNames.push_back(builder.getStringAttr(colMemberName));
               colTypes.push_back(mlir::TypeAttr::get(functionInfo->resultType.toMlirType(builder.getContext())));
               auto attrDef = functionInfo->createRef(attrManager);
               attrs.push_back(attrDef);
               break;
            }
            case ast::NamedResultType::EXPRESSION: {
               names.push_back(builder.getStringAttr(named->displayName));
               auto colMemberName = memberManager.getUniqueMember(named->name);
               colMemberNames.push_back(builder.getStringAttr(colMemberName));
               colTypes.push_back(mlir::TypeAttr::get(named->resultType.toMlirType(builder.getContext())));

               colTypes.push_back(mlir::TypeAttr::get(named->resultType.type.getMLIRTypeCreator()->createType(builder.getContext())));

               auto attrDef = named->createRef(attrManager);
               attrs.push_back(attrDef);
               break;
            }
            default: {
               error("Not implemented", tableProducer->loc);
            }
         }
      }

      localTableType = subop::LocalTableType::get(
         builder.getContext(),
         subop::StateMembersAttr::get(
            builder.getContext(),
            builder.getArrayAttr(colMemberNames),
            builder.getArrayAttr(colTypes)),
         builder.getArrayAttr(names));

      mlir::Value result = builder.create<relalg::MaterializeOp>(
         builder.getUnknownLoc(),
         localTableType,
         tree,
         builder.getArrayAttr(attrs),
         builder.getArrayAttr(names));

      // Use the materialized result in the QueryReturnOp instead of the input tree
      builder.create<relalg::QueryReturnOp>(builder.getUnknownLoc(), result);
   }
   relalg::QueryOp queryOp = builder.create<relalg::QueryOp>(builder.getUnknownLoc(), mlir::TypeRange{localTableType}, mlir::ValueRange{});
   queryOp.getQueryOps().getBlocks().clear();
   queryOp.getQueryOps().push_back(block);
   return queryOp.getResults()[0];
}

mlir::Value SQLMlirTranslator::translateTableProducer(mlir::OpBuilder& builder, std::shared_ptr<ast::TableProducer> tableProducer, std::shared_ptr<analyzer::SQLContext> context) {
   mlir::Value tree;

   switch (tableProducer->nodeType) {
      case ast::NodeType::PIPE_OP: {
         auto pipeOperator = std::static_pointer_cast<ast::PipeOperator>(tableProducer);

         if (pipeOperator->input) {
            tree = translateTableProducer(builder, pipeOperator->input, context);
         }
         tree = translatePipeOperator(builder, pipeOperator, context, tree);
         break;
      }
      case ast::NodeType::BOUND_TABLE_REF: {
         auto tableRef = std::static_pointer_cast<ast::BoundTableRef>(tableProducer);
         tree = translateTableRef(builder, tableRef, context);

         break;
      }
      case ast::NodeType::BOUND_RESULT_MODIFIER: {
         auto resultModifier = std::static_pointer_cast<ast::BoundResultModifier>(tableProducer);
         if (resultModifier->input) {
            tree = translateTableProducer(builder, resultModifier->input, context);
         }
         tree = translateResultModifier(builder, resultModifier, context, tree);
         break;
      }
      default: error("Not implemented", tableProducer->loc);
   }

   return tree;
}

mlir::Value SQLMlirTranslator::translatePipeOperator(mlir::OpBuilder& builder, std::shared_ptr<ast::PipeOperator> pipeOperator, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree) {
   switch (pipeOperator->pipeOpType) {
      case ast::PipeOperatorType::SELECT: {
         return tree;
      }
      case ast::PipeOperatorType::WHERE: {
         assert(pipeOperator->node->nodeType == ast::NodeType::BOUND_EXPRESSION);
         auto whereClause = std::static_pointer_cast<ast::BoundExpression>(pipeOperator->node);
         //Make predicate
         auto* pred = new mlir::Block;
         mlir::OpBuilder predBuilder(builder.getContext());
         pred->addArgument(tuples::TupleType::get(builder.getContext()), builder.getUnknownLoc());
         auto tupleScope = translationContext->createTupleScope();
         translationContext->setCurrentTuple(pred->getArgument(0));

         predBuilder.setInsertionPointToStart(pred);
         auto expr = translateExpression(predBuilder, whereClause, context);
         predBuilder.create<tuples::ReturnOp>(builder.getUnknownLoc(), expr);

         auto sel = builder.create<relalg::SelectionOp>(builder.getUnknownLoc(), tuples::TupleStreamType::get(builder.getContext()), tree);
         sel.getPredicate().push_back(pred);
         return sel.getResult();
      }
      case ast::PipeOperatorType::AGGREGATE: {
         assert(pipeOperator->node->nodeType == ast::NodeType::BOUND_AGGREGATION);
         auto aggregationNode = std::static_pointer_cast<ast::BoundAggregationNode>(pipeOperator->node);
         //TODO logic here
         tree = translateAggregation(builder, aggregationNode, context, tree);
         tree = createMap(builder, "map", context->currentScope->evalBeforeAggr, context, tree);
         context->currentScope->evalBeforeAggr.clear();
         return tree;
      }
      case ast::PipeOperatorType::EXTEND: {
         auto extendNode = std::static_pointer_cast<ast::BoundExtendNode>(pipeOperator->node);
         tree = createMap(builder, extendNode->mapName, extendNode->extensions, context, tree);
         return tree;
      }
      default: error("Not implememted", pipeOperator->loc);
   }
}

mlir::Value SQLMlirTranslator::translateResultModifier(mlir::OpBuilder& builder, std::shared_ptr<ast::BoundResultModifier> resultModifier, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree) {
   switch (resultModifier->modifierType) {
      case ast::ResultModifierType::BOUND_ORDER_BY: {
         auto orderByModifier = std::static_pointer_cast<ast::BoundOrderByModifier>(resultModifier);
         std::vector<mlir::Attribute> mapping;
         for (auto orderByElement : orderByModifier->orderByElements) {
            relalg::SortSpec spec = relalg::SortSpec::asc;
            if (orderByElement->type == ast::OrderType::DESCENDING) {
               spec = relalg::SortSpec::desc;
            }
            if (orderByElement->expression->type == ast::ExpressionType::BOUND_COLUMN_REF) {
               auto columnRef = std::static_pointer_cast<ast::BoundColumnRefExpression>(orderByElement->expression);
               auto namedResult = columnRef->namedResult;
               auto attrDef = namedResult->createRef(attrManager);
               mapping.push_back(relalg::SortSpecificationAttr::get(builder.getContext(), attrDef, spec));

            } else {
               error("Not implemented", orderByElement->expression->loc);
            }
         }
         return builder.create<relalg::SortOp>(builder.getUnknownLoc(), tuples::TupleStreamType::get(builder.getContext()), tree, builder.getArrayAttr(mapping));
      }
      case ast::ResultModifierType::BOUND_LIMIT: {
         auto limitModifier = std::static_pointer_cast<ast::BoundLimitModifier>(resultModifier);
         auto value = std::static_pointer_cast<ast::IntValue>(std::static_pointer_cast<ast::BoundConstantExpression>(limitModifier->limitExpression)->value);
         return builder.create<relalg::LimitOp>(builder.getUnknownLoc(), tuples::TupleStreamType::get(builder.getContext()), value->iVal, tree);
      }
      default: error("ResultModifier Not implemented", resultModifier->loc);
   }
}

mlir::Value SQLMlirTranslator::translateExpression(mlir::OpBuilder& builder, std::shared_ptr<ast::BoundExpression> expression, std::shared_ptr<analyzer::SQLContext> context) {
   switch (expression->exprClass) {
      case ast::ExpressionClass::BOUND_COLUMN_REF: {
         auto columnRef = std::static_pointer_cast<ast::BoundColumnRefExpression>(expression);
         auto nameResult = columnRef->namedResult;

         mlir::Type type = nameResult->resultType.toMlirType(builder.getContext());

         if (nameResult->type == ast::NamedResultType::Column) {
            type = createTypeForColumn(builder.getContext(), std::static_pointer_cast<ast::ColumnInfo>(nameResult)->column);
         }

         auto attrDef = nameResult->createRef(attrManager);
         return builder.create<tuples::GetColumnOp>(
            builder.getUnknownLoc(),
            type, attrDef, translationContext->getCurrentTuple());
      }
      case ast::ExpressionClass::BOUND_CONSTANT: {
         auto constExpr = std::static_pointer_cast<ast::BoundConstantExpression>(expression);
         switch (constExpr->value->type) {
            case ast::ConstantType::INT: {
               auto value = std::static_pointer_cast<ast::IntValue>(constExpr->value);
               return builder.create<db::ConstantOp>(builder.getUnknownLoc(), builder.getI32Type(), builder.getI32IntegerAttr(value->iVal));
            }
            case ast::ConstantType::STRING: {
               auto value = std::static_pointer_cast<ast::StringValue>(constExpr->value);
               mlir::Type stringType = db::StringType::get(builder.getContext());
               if (value->sVal.size() <= 8 && value->sVal.size() > 0) {
                  stringType = db::CharType::get(builder.getContext(), value->sVal.size());
               };
               return builder.create<db::ConstantOp>(builder.getUnknownLoc(), stringType, builder.getStringAttr(value->sVal));
            }
            case ast::ConstantType::FLOAT: {
               auto value = std::static_pointer_cast<ast::FloatValue>(constExpr->value);
               assert(constExpr->resultType.has_value());
               return builder.create<db::ConstantOp>(builder.getUnknownLoc(), constExpr->resultType.value().type.getMLIRTypeCreator()->createType(builder.getContext()), builder.getStringAttr(value->fVal));
            }

            default: error("Not implemented", expression->loc);
         }
      }
      case ast::ExpressionClass::BOUND_COMPARISON: {
         auto comparisonExpr = std::static_pointer_cast<ast::BoundComparisonExpression>(expression);
         auto left = translateExpression(builder, comparisonExpr->left, context);
         //Handle in
         if (comparisonExpr->type == ast::ExpressionType::COMPARE_IN || comparisonExpr->type == ast::ExpressionType::COMPARE_NOT_IN) {
            std::vector<mlir::Value> values;
            values.push_back(left);
            for (auto& rightChild : comparisonExpr->rightChildren) {
               auto right = translateExpression(builder, rightChild, context);
               values.push_back(right);
            }

            auto oneOf = builder.create<db::OneOfOp>(builder.getUnknownLoc(), compiler::frontend::sql::SQLTypeInference::toCommonBaseTypes(builder, values));
            if (comparisonExpr->type == ast::ExpressionType::COMPARE_NOT_IN) {
               return builder.create<db::NotOp>(builder.getUnknownLoc(), oneOf);
            }
            return oneOf;
         }
         //Not IN
         assert(comparisonExpr->rightChildren.size() == 1);

         auto right = translateExpression(builder, comparisonExpr->rightChildren[0], context);
         if (comparisonExpr->type == ast::ExpressionType::COMPARE_LIKE || comparisonExpr->type == ast::ExpressionType::COMPARE_NOT_LIKE) {
            auto ct = compiler::frontend::sql::SQLTypeInference::toCommonBaseTypes(builder, {left, right});
            auto isNullable = mlir::isa<db::NullableType>(left.getType()) || mlir::isa<db::NullableType>(right.getType());
            mlir::Type resType = isNullable ? (mlir::Type) db::NullableType::get(builder.getContext(), builder.getI1Type()) : (mlir::Type) builder.getI1Type();
            auto like = builder.create<db::RuntimeCall>(builder.getUnknownLoc(), resType, "Like", mlir::ValueRange({ct[0], ct[1]})).getRes();
            return comparisonExpr->type == ast::ExpressionType::COMPARE_NOT_LIKE ? builder.create<db::NotOp>(builder.getUnknownLoc(), like) : like;
         }
         db::DBCmpPredicate pred;
         switch (expression->type) {
            case ast::ExpressionType::COMPARE_EQUAL:
               pred = db::DBCmpPredicate::eq;
               break;
            case ast::ExpressionType::COMPARE_NOTEQUAL:
               pred = db::DBCmpPredicate::neq;
               break;
            case ast::ExpressionType::COMPARE_LESSTHAN:
               pred = db::DBCmpPredicate::lt;
               break;
            case ast::ExpressionType::COMPARE_GREATERTHAN:
               pred = db::DBCmpPredicate::gt;
               break;
            case ast::ExpressionType::COMPARE_LESSTHANOREQUALTO:
               pred = db::DBCmpPredicate::lte;
               break;
            case ast::ExpressionType::COMPARE_GREATERTHANOREQUALTO:
               pred = db::DBCmpPredicate::gte;
               break;
            default: throw std::runtime_error("not implemented");
         }
         auto ct = compiler::frontend::sql::SQLTypeInference::toCommonBaseTypes(builder, {left, right});
         return builder.create<db::CmpOp>(builder.getUnknownLoc(), pred, ct[0], ct[1]);
      }
      case ast::ExpressionClass::BOUND_CONJUNCTION: {
         auto conjunction = std::static_pointer_cast<ast::BoundConjunctionExpression>(expression);
         std::vector<mlir::Value> values;
         for (auto exp : conjunction->children) {
            values.push_back(translateExpression(builder, exp, context));
         }
         switch (conjunction->type) {
            case ast::ExpressionType::CONJUNCTION_AND: {
               return builder.create<db::AndOp>(builder.getUnknownLoc(), values);
            }
            case ast::ExpressionType::CONJUNCTION_OR: {
               return builder.create<db::OrOp>(builder.getUnknownLoc(), values);
            }
            default: throw std::runtime_error("not implemented");
         }
      }
      case ast::ExpressionClass::BOUND_OPERATOR: {
         auto operatorExpr = std::static_pointer_cast<ast::BoundOperatorExpression>(expression);

         mlir::Value left, right;

         switch (operatorExpr->type) {
            case ast::ExpressionType::OPERATOR_MINUS:
            case ast::ExpressionType::OPERATOR_DIVIDE:
            case ast::ExpressionType::OPERATOR_MOD:
            case ast::ExpressionType::OPERATOR_TIMES:
            case ast::ExpressionType::OPERATOR_PLUS: {
               assert(operatorExpr->children.size() == 2);
               left = translateExpression(builder, operatorExpr->children[0], context);
               right = translateExpression(builder, operatorExpr->children[1], context);
               return translateBinaryOperatorExpression(builder, operatorExpr, context, left, right);
            }
            default: error("Not implemented", expression->loc);
         }
      }
      case ast::ExpressionClass::BOUND_CAST: {
         auto castExpr = std::static_pointer_cast<ast::BoundCastExpression>(expression);
         auto toCast = translateExpression(builder, castExpr->child, context);
         //TODO support for typeModifiers/ move to analyzer
         // Inspire From Parse.cpp;
         //auto resType = createType(typeName, getTypeModList(castNode->type_name_->typmods_)).getMLIRTypeCreator()->createType(builder.getContext());

         assert(castExpr->resultType.has_value());
         auto resType = castExpr->resultType.value().type.getMLIRTypeCreator()->createType(builder.getContext());
         if (auto constOp = mlir::dyn_cast_or_null<db::ConstantOp>(toCast.getDefiningOp())) {
            if (auto intervalType = mlir::dyn_cast<db::IntervalType>(resType)) {
               //TODO maybe create stringRepresentation at analyzer Level?
               auto stringRepresentation = mlir::cast<mlir::StringAttr>(constOp.getValue()).str();
               //!Shortcutted here, implement different interval types later
               /*
                * TODO
               * auto typeModList = getTypeModList(castNode->type_name_->typmods_);
                     if (typeModList.size() > 0 && std::get<size_t>(typeModList[0]) & 4) {
                        //interval in years
                        stringRepresentation = std::to_string(std::stol(stringRepresentation) * 12);
                     }
                */
               //TODO maybe create stringRepresentation at analyzer Level?
               if (intervalType.getUnit() == db::IntervalUnitAttr::daytime && !stringRepresentation.ends_with("days")) {
                  stringRepresentation += "days";
               }
               constOp->setAttr("value", builder.getStringAttr(stringRepresentation));
            }
            constOp.getResult().setType(resType);
            return constOp;
         } else {
            return compiler::frontend::sql::SQLTypeInference::castValueToType(builder, toCast, resType);
         }
      }
      case ast::ExpressionClass::BOUND_BETWEEN: {
         auto boundBetween = std::static_pointer_cast<ast::BoundBetweenExpression>(expression);
         mlir::Value input, lower, upper;
         input = translateExpression(builder, boundBetween->input, context);
         lower = translateExpression(builder, boundBetween->lower, context);
         upper = translateExpression(builder, boundBetween->upper, context);
         auto ct = compiler::frontend::sql::SQLTypeInference::toCommonBaseTypes(builder, {input, lower, upper});
         mlir::Value between = builder.create<db::BetweenOp>(builder.getUnknownLoc(), ct[0], ct[1], ct[2], true, true);
         return between;
      }
      case ast::ExpressionClass::BOUND_FUNCTION: {
         auto function = std::static_pointer_cast<ast::BoundFunctionExpression>(expression);
         if (function->functionName == "EXTRACT") {
            assert(function->arguments.size() == 2);
            auto part = translateExpression(builder, function->arguments[0], context);
            auto arg2 = translateExpression(builder, function->arguments[1], context);
            return builder.create<db::RuntimeCall>(builder.getUnknownLoc(), wrapNullableType(builder.getContext(), builder.getI64Type(), {part, arg2}), "ExtractFromDate", mlir::ValueRange({part, arg2})).getRes();
         }
      }
      case ast::ExpressionClass::BOUND_SUBQUERY: {
         auto subquery = std::static_pointer_cast<ast::BoundSubqueryExpression>(expression);
         context->pushNewScope(subquery->sqlScope);
         auto translatedSubquery = translateTableProducer(builder, subquery->subquery, context);
         context->popCurrentScope();
         switch (subquery->subqueryType) {
            case ast::SubqueryType::SCALAR: {
               mlir::Type resType = subquery->resultType.value().type.getMLIRTypeCreator()->createType(builder.getContext());
               assert(subquery->namedResult.has_value());
               if (subquery->namedResult.value()->resultType.isNullable) {
                  resType = db::NullableType::get(builder.getContext(), resType);
               }
               assert(subquery->namedResult.has_value());
               //TODO use zero instead of null
               mlir::Value scalarValue = builder.create<relalg::GetScalarOp>(builder.getUnknownLoc(), resType, subquery->namedResult.value()->createRef(attrManager), translatedSubquery);
               return scalarValue;
            }
            case ast::SubqueryType::ANY:
            case ast::SubqueryType::NOT_ANY: {
               auto* block = new mlir::Block;
               mlir::OpBuilder predBuilder(builder.getContext());
               block->addArgument(tuples::TupleType::get(builder.getContext()), builder.getUnknownLoc());
               auto tupleScope = translationContext->createTupleScope();
               translationContext->setCurrentTuple(block->getArgument(0));

               predBuilder.setInsertionPointToStart(block);
               mlir::Value expr = translateExpression(predBuilder, subquery->testExpr, context);

               auto mlirType = subquery->namedResult.value()->resultType.toMlirType(builder.getContext());

               mlir::Value colVal = predBuilder.create<tuples::GetColumnOp>(predBuilder.getUnknownLoc(), mlirType, subquery->namedResult.value()->createRef(attrManager), block->getArgument(0));

               auto ct = compiler::frontend::sql::SQLTypeInference::toCommonBaseTypes(builder, {colVal, expr});
               //TODO extract and remove hardcoded
               db::DBCmpPredicate dbCmpPred = db::DBCmpPredicate::eq;
               mlir::Value pred = predBuilder.create<db::CmpOp>(predBuilder.getUnknownLoc(), dbCmpPred, ct[0], ct[1]);
               ;
               predBuilder.create<tuples::ReturnOp>(builder.getUnknownLoc(), pred);

               auto sel = builder.create<relalg::SelectionOp>(builder.getUnknownLoc(), tuples::TupleStreamType::get(builder.getContext()), translatedSubquery);
               sel.getPredicate().push_back(block);
               translatedSubquery = sel.getResult();
               auto existsOp = builder.create<relalg::ExistsOp>(builder.getUnknownLoc(), builder.getI1Type(), translatedSubquery);
               if (subquery->subqueryType == ast::SubqueryType::NOT_ANY) {
                  return builder.create<db::NotOp>(builder.getUnknownLoc(), existsOp);
               }
               return existsOp;
            }
            default: error("Subquery type not implemented", expression->loc);
         }
      }

      default: error("Not implemented", expression->loc);
   }
}

mlir::Value SQLMlirTranslator::translateBinaryOperatorExpression(mlir::OpBuilder& builder, std::shared_ptr<ast::BoundOperatorExpression> expression, std::shared_ptr<analyzer::SQLContext> context, mlir::Value left, mlir::Value right) {
   auto loc = builder.getUnknownLoc();
   switch (expression->type) {
      case ast::ExpressionType::OPERATOR_PLUS: {
         if (mlir::isa<db::DateType>(getBaseType(left.getType())) && mlir::isa<db::IntervalType>(getBaseType(right.getType()))) {
            return builder.create<db::RuntimeCall>(loc, left.getType(), "DateAdd", mlir::ValueRange({left, right})).getRes();
         }
         //TODO use already found out common type!!!!!!
         return builder.create<db::AddOp>(builder.getUnknownLoc(), compiler::frontend::sql::SQLTypeInference::toCommonBaseTypes(builder, {left, right}));
      }
      case ast::ExpressionType::OPERATOR_MINUS: {
         if (mlir::isa<db::DateType>(left.getType()) && mlir::isa<db::IntervalType>(right.getType())) {
            return builder.create<db::RuntimeCall>(loc, left.getType(), "DateSubtract", mlir::ValueRange({left, right})).getRes();
         }
         //TODO use already found out common type!!!!!!
         return builder.create<db::SubOp>(builder.getUnknownLoc(), compiler::frontend::sql::SQLTypeInference::toCommonBaseTypes(builder, {left, right}));
      }
      case ast::ExpressionType::OPERATOR_TIMES: {
         //TODO use already found out common type!!!!!!
         return builder.create<db::MulOp>(builder.getUnknownLoc(), compiler::frontend::sql::SQLTypeInference::toCommonNumber(builder, {left, right}));
      }
      case ast::ExpressionType::OPERATOR_DIVIDE: {
         //TODO use already found out common type!!!!!!
         return builder.create<db::DivOp>(builder.getUnknownLoc(), compiler::frontend::sql::SQLTypeInference::toCommonNumber(builder, {left, right}));
      }
      case ast::ExpressionType::OPERATOR_MOD: {
         //TODO use already found out common type!!!!!!
         return builder.create<db::ModOp>(builder.getUnknownLoc(), compiler::frontend::sql::SQLTypeInference::toCommonNumber(builder, {left, right}));
      }
      default: error("Not implemented", expression->loc);
   }
}

mlir::Value SQLMlirTranslator::translateTableRef(mlir::OpBuilder& builder, std::shared_ptr<ast::BoundTableRef> tableRef, std::shared_ptr<analyzer::SQLContext> context) {
   mlir::Value last;
   switch (tableRef->type) {
      case ast::TableReferenceType::BASE_TABLE: {
         auto baseTableRef = std::static_pointer_cast<ast::BoundBaseTableRef>(tableRef);
         std::string relation = baseTableRef->tableCatalogEntry->getName();
         //std::string alias = relation;
         if (!baseTableRef->tableCatalogEntry) {
            error("Table not found", baseTableRef->loc);
         }
         auto rel = baseTableRef->tableCatalogEntry;
         std::string scopeName = baseTableRef->alias.empty() ? relation : baseTableRef->alias;
         std::vector<mlir::NamedAttribute> columns{};
         for (auto& col : rel->getColumns()) {
            auto attrDef = attrManager.createDef(scopeName, col.getColumnName());
            attrDef.getColumn().type = createTypeForColumn(builder.getContext(), col);
            columns.push_back(builder.getNamedAttr(col.getColumnName(), attrDef));
         }
         return builder.create<relalg::BaseTableOp>(builder.getUnknownLoc(), tuples::TupleStreamType::get(builder.getContext()), relation, builder.getDictionaryAttr(columns));
      }
      case ast::TableReferenceType::JOIN: {
         auto boundJoin = std::static_pointer_cast<ast::BoundJoinRef>(tableRef);
         mlir::Value left, right;
         left = translateTableProducer(builder, boundJoin->left, context);
         right = translateTableProducer(builder, boundJoin->right, context);
         switch (boundJoin->type) {
            case ast::JoinType::INNER: {
               switch (boundJoin->refType) {
                  case ast::JoinCondType::CROSS: {
                  }
                  default: error("Not implemented", tableRef->loc);
               }

               //TODO translate predicate
               auto joinOp = builder.create<relalg::InnerJoinOp>(builder.getUnknownLoc(), tuples::TupleStreamType::get(builder.getContext()), left, right);

               return joinOp;
            }
            case ast::JoinType::CROSS: {
               auto joinOp = builder.create<relalg::CrossProductOp>(builder.getUnknownLoc(), tuples::TupleStreamType::get(builder.getContext()), left, right);
               return joinOp;
            }

            case ast::JoinType::LEFT: {
               mlir::Value left, right;
               left = translateTableProducer(builder, boundJoin->left, context);
               right = translateTableProducer(builder, boundJoin->right, context);
               mlir::Block* pred;
               if (!std::holds_alternative<std::shared_ptr<ast::BoundExpression>>(boundJoin->condition)) {
                  error("Not implemented", tableRef->loc);
               }
               auto* block = new mlir::Block;
               {
                  mlir::OpBuilder predBuilder(builder.getContext());
                  block->addArgument(tuples::TupleType::get(builder.getContext()), builder.getUnknownLoc());

                  predBuilder.setInsertionPointToStart(block);
                  mlir::Value expr = translateExpression(predBuilder, std::get<std::shared_ptr<ast::BoundExpression>>(boundJoin->condition), context);
                  predBuilder.create<tuples::ReturnOp>(builder.getUnknownLoc(), expr);
               }

               std::vector<mlir::Attribute> outerJoinMapping{};
               std::ranges::transform(boundJoin->outerJoinMapping, std::back_inserter(outerJoinMapping), [&](std::shared_ptr<ast::NamedResult> namedResult) {
                  return namedResult->createDef(attrManager);
               });

               mlir::ArrayAttr mapping = builder.getArrayAttr(outerJoinMapping);

               auto join = builder.create<relalg::OuterJoinOp>(builder.getUnknownLoc(), tuples::TupleStreamType::get(builder.getContext()), left, right, mapping);

               return join;
            }
            case ast::JoinType::OUTER: {
            }
            default: error("Not implemented", tableRef->loc);
         }

         right = translateTableProducer(builder, boundJoin->right, context);
         error("TODO hier weiter machen", tableRef->loc);
      }
      case ast::TableReferenceType::SUBQUERY: {
         auto subquery = std::static_pointer_cast<ast::BoundSubqueryRef>(tableRef);
         mlir::Value subQuery;
         auto translated = translateTableProducer(builder, subquery->subSelect, context);

         return translated;
      }
      default:
         error("Not implemented", tableRef->loc);
   }
}

mlir::Value SQLMlirTranslator::translateAggregation(mlir::OpBuilder& builder, std::shared_ptr<ast::BoundAggregationNode> aggregation, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree) {
   if (aggregation->groupByNode && aggregation->aggregations.empty()) {
      return tree;
   }
   //create map
   tree = createMap(builder, aggregation->mapName, aggregation->toMapExpressions, context, tree);

   //Translate group by Attributes
   std::vector<mlir::Attribute> groupByAttrs;
   std::unordered_map<std::string, mlir::Attribute> groupedExpressions;
   std::unordered_map<std::string, size_t> groupByAttrToPos;
   for (auto& groupBy : aggregation->groupByNode->groupExpressions) {
      if (groupBy->type == ast::ExpressionType::BOUND_COLUMN_REF) {
         auto columnRef = std::static_pointer_cast<ast::BoundColumnRefExpression>(groupBy);
         auto namedResult = columnRef->namedResult;

         auto attrDef = namedResult->createRef(attrManager);
         //TODO
         /*auto attrName = fieldsToString(columnRef->fields_);
         groupByAttrToPos[attrName] = i;*/
         groupByAttrs.push_back(attrDef);
      } else {
         error("Not implemented", groupBy->loc);
      }
   }

   //TODO maby split logic into two different methods!

   /*
    *Perform aggregation
   */

   //TODO rollup logic
   bool rollup = false;
   if (rollup) {
      error("Not implemented", aggregation->loc);
   } else {
      static size_t groupById = 0;
      auto tupleStreamType = tuples::TupleStreamType::get(builder.getContext());
      auto tupleType = tuples::TupleType::get(builder.getContext());

      std::string groupByName = "aggr" + std::to_string(groupById++);
      auto tupleScope = translationContext->createTupleScope();
      auto* block = new mlir::Block;
      block->addArgument(tupleStreamType, builder.getUnknownLoc());
      block->addArgument(tupleType, builder.getUnknownLoc());
      mlir::Value relation = block->getArgument(0);
      mlir::OpBuilder aggrBuilder(builder.getContext());
      aggrBuilder.setInsertionPointToStart(block);
      std::vector<mlir::Value> createdValues;
      std::vector<mlir::Attribute> createdCols;
      std::unordered_map<std::string, tuples::Column*> mapping;

      //AggrFunctions
      for (auto aggrFunction : aggregation->aggregations) {
         mlir::Value expr; //TODO??
         auto aggrFuncName = aggrFunction->functionName;

         auto attrDef = attrManager.createDef(aggrFunction->scope, aggrFunction->aliasOrUniqueIdentifier);
         if (aggrFuncName == "count*") {
            expr = aggrBuilder.create<relalg::CountRowsOp>(builder.getUnknownLoc(), builder.getI64Type(), relation);
            //TODO not star
            //TODO use zero instead of null
            /*if (groupByAttrs.empty()) {
               context.useZeroInsteadNull.insert(&attrDef.getColumn());
            }*/
         } else {
            //TODO move logic to analyzer
            auto relalgAggrFunc = llvm::StringSwitch<relalg::AggrFunc>(aggrFuncName)
                                     .Case("sum", relalg::AggrFunc::sum)
                                     .Case("avg", relalg::AggrFunc::avg)
                                     .Case("min", relalg::AggrFunc::min)
                                     .Case("max", relalg::AggrFunc::max)
                                     .Case("count", relalg::AggrFunc::count)
                                     .Case("stddev_samp", relalg::AggrFunc::stddev_samp)
                                     .Default(relalg::AggrFunc::count);
            //TODO use zero instead of null
            /*if (relalgAggrFunc == relalg::AggrFunc::count) {
               error("Use zero instead of null", aggrFunction->loc);
               /*if (groupByAttrs.empty()) {
                  context.useZeroInsteadNull.insert(&attrDef.getColumn());
               }*/
            //}
            assert(aggrFunction->arguments.size() == 1);
            tuples::ColumnRefAttr refAttr;
            switch (aggrFunction->arguments[0]->type) {
               case ast::ExpressionType::BOUND_COLUMN_REF: {
                  auto columnRef = std::static_pointer_cast<ast::BoundColumnRefExpression>(aggrFunction->arguments[0]);
                  auto namedResult = columnRef->namedResult;
                  refAttr = namedResult->createRef(attrManager);
                  break;
               }
               default: {
                  //Is in map
                  refAttr = attrManager.createRef(aggregation->mapName, aggrFunction->arguments[0]->alias);

                  break;
               };
            }

            mlir::Value currRel = relation;
            //TODO distinct
            if (aggrFunction->distinct) {
               currRel = aggrBuilder.create<relalg::ProjectionOp>(builder.getUnknownLoc(), relalg::SetSemantic::distinct, currRel, builder.getArrayAttr({refAttr}));
            }
            mlir::Type aggrResultType;
            assert(aggrFunction->resultType.has_value());
            aggrResultType = aggrFunction->resultType->toMlirType(builder.getContext());

            if (aggrFunction->arguments[0]->type != ast::ExpressionType::BOUND_COLUMN_REF) {
               //TODO better, over context!!
               assert(aggrFunction->arguments[0]->namedResult.has_value());
               aggrResultType = aggrFunction->arguments[0]->namedResult.value()->resultType.toMlirType(builder.getContext());
            }
            //TODO define type
            if (relalgAggrFunc == relalg::AggrFunc::avg) {
               auto baseType = getBaseType(aggrResultType);
               if (baseType.isIntOrFloat() && !baseType.isIntOrIndex()) {
                  //keep aggrResultType
               } else if (mlir::isa<db::DecimalType>(baseType)) {
                  mlir::OpBuilder b(builder.getContext());
                  mlir::Value x = b.create<db::ConstantOp>(b.getUnknownLoc(), baseType, b.getUnitAttr());
                  mlir::Value x2 = b.create<db::ConstantOp>(b.getUnknownLoc(), db::DecimalType::get(b.getContext(), 19, 0), b.getUnitAttr());
                  mlir::Value div = b.create<db::DivOp>(b.getUnknownLoc(), x, x2);
                  aggrResultType = div.getType();
                  div.getDefiningOp()->erase();
                  x2.getDefiningOp()->erase();
                  x.getDefiningOp()->erase();
               } else {
                  mlir::OpBuilder b(builder.getContext());
                  mlir::Value x = b.create<db::ConstantOp>(b.getUnknownLoc(), db::DecimalType::get(b.getContext(), 19, 0), b.getUnitAttr());
                  mlir::Value div = b.create<db::DivOp>(b.getUnknownLoc(), x, x);
                  aggrResultType = div.getType();
                  div.getDefiningOp()->erase();
                  x.getDefiningOp()->erase();
               }
               if (mlir::isa<db::NullableType>(refAttr.getColumn().type)) {
                  aggrResultType = db::NullableType::get(builder.getContext(), aggrResultType);
               }
            }
            //TODO move to analyzer
            if (!mlir::isa<db::NullableType>(aggrResultType) && (groupByAttrs.empty())) {
               aggrResultType = db::NullableType::get(builder.getContext(), aggrResultType);
               aggrFunction->functionInfo->resultType.isNullable = true;
            }

            expr = aggrBuilder.create<relalg::AggrFuncOp>(builder.getUnknownLoc(), aggrResultType, relalgAggrFunc, currRel, refAttr);
         }
         attrDef.getColumn().type = expr.getType();

         //TODO mapping.insert({12, "&attrDef.getColumn()"});
         createdCols.push_back(attrDef);
         createdValues.push_back(expr);
      }

      aggrBuilder.create<tuples::ReturnOp>(builder.getUnknownLoc(), createdValues);
      auto groupByOp = builder.create<relalg::AggregationOp>(builder.getUnknownLoc(), tupleStreamType, tree, builder.getArrayAttr(groupByAttrs), builder.getArrayAttr(createdCols));
      groupByOp.getAggrFunc().push_back(block);
      return groupByOp.getResult();
   }
   return tree;
}

mlir::Value SQLMlirTranslator::createMap(mlir::OpBuilder& builder, std::string mapName, std::vector<std::shared_ptr<ast::BoundExpression>> toMap, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree) {
   if (toMap.empty()) {
      return tree;
   }
   auto* block = new mlir::Block;
   static size_t mapId = 0;

   mlir::OpBuilder mapBuilder(builder.getContext());
   block->addArgument(tuples::TupleType::get(builder.getContext()), builder.getUnknownLoc());
   auto tupleScope = translationContext->createTupleScope();
   mlir::Value tuple = block->getArgument(0);
   translationContext->setCurrentTuple(tuple);

   mapBuilder.setInsertionPointToStart(block);
   std::vector<mlir::Value> createdValues;
   std::vector<mlir::Attribute> createdCols;

   for (auto p : toMap) {
      mlir::Value expr = translateExpression(mapBuilder, p, context);
      //TODO does the use of alias make sense here?
      auto attrDef = attrManager.createDef(mapName, p->alias);
      if (p->namedResult.has_value()) {
         p->namedResult.value()->scope = mapName;
         attrDef = p->namedResult.value()->createDef(attrManager);
      }

      attrDef.getColumn().type = expr.getType();
      //TODO MAP context.mapAttribute(scope, p.first->colId, &attrDef.getColumn());
      createdCols.push_back(attrDef);
      createdValues.push_back(expr);
   }
   auto mapOp = builder.create<relalg::MapOp>(builder.getUnknownLoc(), tuples::TupleStreamType::get(builder.getContext()), tree, builder.getArrayAttr(createdCols));
   mapOp.getPredicate().push_back(block);
   mapBuilder.create<tuples::ReturnOp>(builder.getUnknownLoc(), createdValues);
   return mapOp.getResult();
}

/*
 * Helper functions
 */

mlir::Type SQLMlirTranslator::createTypeForColumn(mlir::MLIRContext* context, const catalog::Column& colDef) {
   mlir::Type baseType = createBaseTypeFromColumnType(context, colDef.getLogicalType());
   return colDef.getIsNullable() ? db::NullableType::get(context, baseType) : baseType;
}

mlir::Type SQLMlirTranslator::createBaseTypeFromColumnType(mlir::MLIRContext* context, const catalog::Type& t) {
   return t.getMLIRTypeCreator()->createType(context);
}

} // namespace lingodb::translator