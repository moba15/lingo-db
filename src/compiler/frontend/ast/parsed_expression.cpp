#include "lingodb/compiler/frontend/ast/parsed_expression.h"

#include "lingodb/compiler/frontend/ast/result_modifier.h"

#include <cassert>
#include <clang/AST/Type.h>
namespace lingodb::ast {

size_t ParsedExpression::hash() {
   size_t result = std::hash<uint8_t>{}(static_cast<uint8_t>(type));
   result = result * 31 + std::hash<uint8_t>{}(static_cast<uint8_t>(exprClass));
   return result;
}
bool ParsedExpression::operator==(ParsedExpression& other) {
   return type == other.type && exprClass == other.exprClass;
}

///ColumnRef
ColumnRefExpression::ColumnRefExpression(std::string columnName, std::string tableName)
   : ColumnRefExpression(tableName.empty() ? std::vector<std::string>{std::move(columnName)} : std::vector<std::string>{std::move(tableName), std::move(columnName)}) {
}
ColumnRefExpression::ColumnRefExpression(std::string columnName) : ColumnRefExpression(std::vector<std::string>{std::move(columnName)}) {
}

ColumnRefExpression::ColumnRefExpression(std::vector<std::string> columnNames) : ParsedExpression(ExpressionType::COLUMN_REF, cType), columnNames(columnNames) {
}

size_t ColumnRefExpression::hash() {
   size_t result = ParsedExpression::hash();
   for (const auto& name : columnNames) {
      result = result * 31 + std::hash<std::string>{}(name);
   }

   return result;
}
bool ColumnRefExpression::operator==(ParsedExpression& other) {
   if (!ParsedExpression::operator==(other)) return false;

   const auto& otherRef = static_cast<ColumnRefExpression&>(other);
   return columnNames == otherRef.columnNames;
}

/// ComparisonExpression
ComparisonExpression::ComparisonExpression(ExpressionType type) : ParsedExpression(type, cType) {
}
ComparisonExpression::ComparisonExpression(ExpressionType type, std::shared_ptr<ParsedExpression> left, std::shared_ptr<ParsedExpression> right) : ParsedExpression(type, cType), left(std::move(left)), rightChildren({std::move(right)}) {
}

ComparisonExpression::ComparisonExpression(ExpressionType type, std::shared_ptr<ParsedExpression> left, std::vector<std::shared_ptr<ParsedExpression>> rightChildren) : ParsedExpression(type, cType), left(std::move(left)), rightChildren(rightChildren) {
}

/// ConjunctionExpression
ConjunctionExpression::ConjunctionExpression(ExpressionType type) : ParsedExpression(type, cType), children() {}
ConjunctionExpression::ConjunctionExpression(ExpressionType type, std::shared_ptr<lingodb::ast::ParsedExpression> left, std::shared_ptr<lingodb::ast::ParsedExpression> right) : ConjunctionExpression(type, std::vector{left, right}) {}
ConjunctionExpression::ConjunctionExpression(ExpressionType type, std::vector<std::shared_ptr<ParsedExpression>> children) : ParsedExpression(type, cType), children(std::move(children)) {
}

size_t ConjunctionExpression::hash() {
   size_t result = ParsedExpression::hash();
   // Hash all children expressions using built-in hash combine
   for (const auto& child : children) {
      result ^= child->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
   }
   return result;
}

bool ConjunctionExpression::operator==(ParsedExpression& other) {
   if (!ParsedExpression::operator==(other)) {
      return false;
   }
   auto& otherConj = static_cast<ConjunctionExpression&>(other);
   // Compare the number of children first
   if (children.size() != otherConj.children.size()) {
      return false;
   }
   // Compare each child expression
   for (size_t i = 0; i < children.size(); i++) {
      if ((*children[i] != *otherConj.children[i])) {
         return false;
      }
   }
   return true;
}

/// ConstantExpression
ConstantExpression::ConstantExpression() : ParsedExpression(ExpressionType::VALUE_CONSTANT, cType) {}

size_t ConstantExpression::hash() {
   size_t result = ParsedExpression::hash();
   if (value) {
      result = result * 31 + value->hash();
   }
   return result;
}

bool ConstantExpression::operator==(ParsedExpression& other) {
   if (!ParsedExpression::operator==(other)) return false;

   const auto& otherConst = static_cast<const ConstantExpression&>(other);

   // Handle cases where one value is null and the other isn't
   if ((value && !otherConst.value) || (!value && otherConst.value)) {
      return false;
   }

   // If both values are null, they're equal
   if (!value && !otherConst.value) {
      return true;
   }

   // Compare the actual values
   return *value == *otherConst.value;
}
/// FunctionExpression
FunctionExpression::FunctionExpression(std::string catalog, std::string schema, std::string functionName, bool isOperator, bool distinct, bool exportState) : ParsedExpression(ExpressionType::FUNCTION, cType), catalog(catalog), schema(schema), functionName(functionName), isOperator(isOperator), distinct(distinct), exportState(exportState) {
   auto found = std::find(aggregationFunctions.begin(), aggregationFunctions.end(), functionName);
   if (found != aggregationFunctions.end()) {
      //! TODO Check if this make sense here
      this->type = ExpressionType::AGGREGATE;
   }
}

size_t FunctionExpression::hash() {
   size_t result = ParsedExpression::hash();
   // Hash basic function properties
   result = result * 31 + std::hash<std::string>{}(catalog);
   result = result * 31 + std::hash<std::string>{}(schema);
   result = result * 31 + std::hash<std::string>{}(functionName);
   result = result * 31 + std::hash<bool>{}(isOperator);
   result = result * 31 + std::hash<bool>{}(distinct);
   result = result * 31 + std::hash<bool>{}(exportState);
   result = result * 31 + std::hash<bool>{}(star);

   // Hash function arguments
   for (const auto& arg : arguments) {
      result = result * 31 + arg->hash();
   }

   // Hash optional components
   if (filter) {
      result = result * 31 + filter->hash();
   }
   if (orderBy) {
      result = result * 31 + orderBy->hash();
   }

   return result;
}
bool FunctionExpression::operator==(ParsedExpression& other) {
   if (!ParsedExpression::operator==(other)) return false;
   const auto& otherFunc = static_cast<const FunctionExpression&>(other);
   if (catalog != otherFunc.catalog ||
       schema != otherFunc.schema ||
       functionName != otherFunc.functionName ||
       isOperator != otherFunc.isOperator ||
       distinct != otherFunc.distinct ||
       exportState != otherFunc.exportState ||
       arguments.size() != otherFunc.arguments.size()) {
      return false;
   }

   // Compare function arguments
   for (size_t i = 0; i < arguments.size(); i++) {
      if (*arguments[i] != *(otherFunc.arguments[i])) {
         return false;
      }
   }

   // Compare optional filter and orderBy if present
   if ((filter && !otherFunc.filter) || (!filter && otherFunc.filter)) {
      return false;
   }
   if (filter && *filter != *otherFunc.filter) {
      return false;
   }

   if ((orderBy && !otherFunc.orderBy) || (!orderBy && otherFunc.orderBy)) {
      return false;
   }
   if (orderBy && *orderBy != *otherFunc.orderBy) {
      return false;
   }

   return star == otherFunc.star;
}

///StarExpression
StarExpression::StarExpression(std::string relationName)
   : ParsedExpression(ExpressionType::STAR, ExpressionClass::STAR), relationName(std::move(relationName)) {
}

size_t StarExpression::hash() {
   size_t result = ParsedExpression::hash();
   // Hash the relation name
   result ^= std::hash<std::string>{}(relationName) + 0x9e3779b9 + (result << 6) + (result >> 2);
   // Hash the expr if it exists
   if (expr) {
      result ^= expr->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
   }
   // Hash the columnsExpr boolean
   result ^= std::hash<bool>{}(columnsExpr) + 0x9e3779b9 + (result << 6) + (result >> 2);
   return result;
}

bool StarExpression::operator==(ParsedExpression& other) {
   if (!ParsedExpression::operator==(other)) {
      return false;
   }
   auto& otherStar = static_cast<StarExpression&>(other);

   // Compare relation name
   if (relationName != otherStar.relationName) {
      return false;
   }

   // Compare columnsExpr
   if (columnsExpr != otherStar.columnsExpr) {
      return false;
   }

   // Compare expr (handle null cases)
   if (expr == otherStar.expr) {
      return true; // Both null or same pointer
   }
   if (!expr || !otherStar.expr) {
      return false; // One is null, other isn't
   }
   return *expr == *otherStar.expr;
}

OperatorExpression::OperatorExpression(ExpressionType type, std::shared_ptr<ParsedExpression> left) : ParsedExpression(type, cType), children(std::vector{left}) {
}
OperatorExpression::OperatorExpression(ExpressionType type, std::shared_ptr<ParsedExpression> left, std::shared_ptr<ParsedExpression> right) : ParsedExpression(type, cType), children(std::vector{left, right}) {
}
OperatorExpression::OperatorExpression(std::string opString, std::shared_ptr<ParsedExpression> left, std::shared_ptr<ParsedExpression> right) : ParsedExpression(ExpressionType::OPERATOR_UNKNOWN, cType), children(std::vector{left, right}), opString(opString) {
}
size_t OperatorExpression::hash() {
   size_t result = ParsedExpression::hash();

   // Hash the operator string
   result ^= std::hash<std::string>{}(opString) + 0x9e3779b9 + (result << 6) + (result >> 2);

   // Hash all children expressions
   for (const auto& child : children) {
      result ^= child->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
   }

   return result;
}

bool OperatorExpression::operator==(ParsedExpression& other) {
   if (!ParsedExpression::operator==(other)) {
      return false;
   }

   auto& otherOp = static_cast<OperatorExpression&>(other);

   // Compare operator strings
   if (opString != otherOp.opString) {
      return false;
   }

   // Compare children
   if (children.size() != otherOp.children.size()) {
      return false;
   }

   for (size_t i = 0; i < children.size(); i++) {
      if ((*children[i] != *otherOp.children[i])) {
         return false;
      }
   }

   return true;
}

CastExpression::CastExpression(LogicalTypeWithMods logicalTypeWithMods, std::shared_ptr<ParsedExpression> child) : ParsedExpression(ExpressionType::CAST, cType), logicalTypeWithMods(logicalTypeWithMods), child(std::move(child)) {
}

size_t CastExpression::hash() {
   size_t result = ParsedExpression::hash();

   // Hash the logical type with mods if present
   if (logicalTypeWithMods) {
      // Hash the logical type
      result ^= std::hash<uint8_t>{}(static_cast<uint8_t>(logicalTypeWithMods->logicalType)) +
         0x9e3779b9 + (result << 6) + (result >> 2);

      // Hash type modifiers
      for (const auto& mod : logicalTypeWithMods->typeModifiers) {
         if (mod) {
            result ^= mod->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
         }
      }
   }

   // Hash the optional interval type if present
   if (optInterval) {
      result ^= std::hash<uint8_t>{}(static_cast<uint8_t>(*optInterval)) +
         0x9e3779b9 + (result << 6) + (result >> 2);
   }

   // Hash the child expression
   if (child) {
      result ^= child->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
   }

   return result;
}

bool CastExpression::operator==(ParsedExpression& other) {
   if (!ParsedExpression::operator==(other)) {
      return false;
   }

   auto& otherCast = static_cast<CastExpression&>(other);

   // Compare logical type with mods
   if (logicalTypeWithMods.has_value() != otherCast.logicalTypeWithMods.has_value()) {
      return false;
   }

   if (logicalTypeWithMods) {
      if (logicalTypeWithMods->logicalType != otherCast.logicalTypeWithMods->logicalType) {
         return false;
      }

      if (logicalTypeWithMods->typeModifiers.size() !=
          otherCast.logicalTypeWithMods->typeModifiers.size()) {
         return false;
      }

      for (size_t i = 0; i < logicalTypeWithMods->typeModifiers.size(); i++) {
         if ((*logicalTypeWithMods->typeModifiers[i] !=
              *otherCast.logicalTypeWithMods->typeModifiers[i])) {
            return false;
         }
      }
   }

   // Compare optional interval
   if (optInterval != otherCast.optInterval) {
      return false;
   }

   // Compare child expressions
   if (child == otherCast.child) {
      return true; // Both null or same pointer
   }
   if (!child || !otherCast.child) {
      return false; // One is null, other isn't
   }
   return *child == *otherCast.child;
}

WindowFrame::WindowFrame(WindowFrameType start) : start(start) {
}
WindowFrame::WindowFrame(WindowFrameType start, std::shared_ptr<ParsedExpression> startExpr) : start(start), startExpr(startExpr) {
}
WindowExpression::WindowExpression() : ParsedExpression(ExpressionType::WINDOW_INVALID, cType) {
}

size_t WindowExpression::hash() {
   size_t result = ParsedExpression::hash();

   // Hash function expression
   if (functionExpression) {
      result ^= functionExpression->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
   }

   // Hash partition expressions
   for (const auto& partition : partitions) {
      if (partition) {
         result ^= partition->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
      }
   }

   // Hash order by modifier
   if (order && *order) {
      //TODO
      // result ^= (*order)->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
   }

   // Hash filter expression
   if (filter) {
      result ^= filter->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
   }

   // Hash boolean flags
   result ^= std::hash<bool>{}(ignoreNulls) + 0x9e3779b9 + (result << 6) + (result >> 2);
   result ^= std::hash<bool>{}(distinct) + 0x9e3779b9 + (result << 6) + (result >> 2);

   // Hash window boundary
   if (windowFrame) {
      result ^= std::hash<uint8_t>{}(static_cast<uint8_t>(windowFrame->windowMode)) +
         0x9e3779b9 + (result << 6) + (result >> 2);
      result ^= std::hash<uint8_t>{}(static_cast<uint8_t>(windowFrame->start)) +
         0x9e3779b9 + (result << 6) + (result >> 2);
      result ^= std::hash<uint8_t>{}(static_cast<uint8_t>(windowFrame->end)) +
         0x9e3779b9 + (result << 6) + (result >> 2);
      if (windowFrame->startExpr) {
         result ^= windowFrame->startExpr->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
      }
      if (windowFrame->endExpr) {
         result ^= windowFrame->endExpr->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
      }
   }

   // Hash expressions
   if (startExpr) {
      result ^= startExpr->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
   }
   if (endExpr) {
      result ^= endExpr->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
   }
   if (offsetExpr) {
      result ^= offsetExpr->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
   }
   if (defaultExpr) {
      result ^= defaultExpr->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
   }

   // Hash argument orders
   if (argOrders) {
      //TODO
      //result ^= argOrders->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
   }

   return result;
}

bool WindowExpression::operator==(ParsedExpression& other) {
   if (!ParsedExpression::operator==(other)) {
      return false;
   }

   auto& otherWindow = static_cast<WindowExpression&>(other);

   // Compare function expressions
   if (!((functionExpression == otherWindow.functionExpression) ||
         (functionExpression && otherWindow.functionExpression &&
          *functionExpression == *otherWindow.functionExpression))) {
      return false;
   }

   // Compare partitions
   if (partitions.size() != otherWindow.partitions.size()) {
      return false;
   }
   for (size_t i = 0; i < partitions.size(); i++) {
      if (!((partitions[i] == otherWindow.partitions[i]) ||
            (partitions[i] && otherWindow.partitions[i] &&
             *partitions[i] == *otherWindow.partitions[i]))) {
         return false;
      }
   }

   // Compare order
   if (order.has_value() != otherWindow.order.has_value()) {
      return false;
   }
   if (order && *order && otherWindow.order && *otherWindow.order
       //&& !(**order == **otherWindow.order)
   ) {
      return false;
   }

   // Compare filter
   if (!((filter == otherWindow.filter) ||
         (filter && otherWindow.filter && *filter == *otherWindow.filter))) {
      return false;
   }

   // Compare boolean flags
   if (ignoreNulls != otherWindow.ignoreNulls || distinct != otherWindow.distinct) {
      return false;
   }

   // Compare window boundary
   if ((windowFrame == nullptr) != (otherWindow.windowFrame == nullptr)) {
      return false;
   }
   if (windowFrame) {
      if (windowFrame->windowMode != otherWindow.windowFrame->windowMode ||
          windowFrame->start != otherWindow.windowFrame->start ||
          windowFrame->end != otherWindow.windowFrame->end) {
         return false;
      }
      if (!((windowFrame->startExpr == otherWindow.windowFrame->startExpr) ||
            (windowFrame->startExpr && otherWindow.windowFrame->startExpr &&
             *windowFrame->startExpr == *otherWindow.windowFrame->startExpr))) {
         return false;
      }
      if (!((windowFrame->endExpr == otherWindow.windowFrame->endExpr) ||
            (windowFrame->endExpr && otherWindow.windowFrame->endExpr &&
             *windowFrame->endExpr == *otherWindow.windowFrame->endExpr))) {
         return false;
      }
   }

   // Compare expressions
   if (!((startExpr == otherWindow.startExpr) ||
         (startExpr && otherWindow.startExpr && *startExpr == *otherWindow.startExpr))) {
      return false;
   }
   if (!((endExpr == otherWindow.endExpr) ||
         (endExpr && otherWindow.endExpr && *endExpr == *otherWindow.endExpr))) {
      return false;
   }
   if (!((offsetExpr == otherWindow.offsetExpr) ||
         (offsetExpr && otherWindow.offsetExpr && *offsetExpr == *otherWindow.offsetExpr))) {
      return false;
   }
   if (!((defaultExpr == otherWindow.defaultExpr) ||
         (defaultExpr && otherWindow.defaultExpr && *defaultExpr == *otherWindow.defaultExpr))) {
      return false;
   }

   // Compare argument orders
   /*if (!((argOrders == otherWindow.argOrders) ||
          (argOrders && otherWindow.argOrders && *argOrders == *otherWindow.argOrders))) {
        return false;
    }*/

   return true;
}

BetweenExpression::BetweenExpression(ExpressionType type, std::shared_ptr<ParsedExpression> input, std::shared_ptr<ParsedExpression> lower, std::shared_ptr<ParsedExpression> upper) : ParsedExpression(type, cType), input(input), lower(lower), upper(upper) {
   assert(lower != nullptr && upper != nullptr && input != nullptr);
   assert(type == ExpressionType::COMPARE_BETWEEN || type == ExpressionType::COMPARE_NOT_BETWEEN);
}

size_t BetweenExpression::hash() {
   size_t result = ParsedExpression::hash();

   // Hash input expression
   if (input) {
      result ^= input->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
   }

   // Hash lower bound
   if (lower) {
      result ^= lower->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
   }

   // Hash upper bound
   if (upper) {
      result ^= upper->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
   }

   // Hash asymmetric flag
   result ^= std::hash<bool>{}(asymmetric) + 0x9e3779b9 + (result << 6) + (result >> 2);

   return result;
}

bool BetweenExpression::operator==(ParsedExpression& other) {
   if (!ParsedExpression::operator==(other)) {
      return false;
   }

   auto& otherBetween = static_cast<BetweenExpression&>(other);

   // Compare asymmetric flag
   if (asymmetric != otherBetween.asymmetric) {
      return false;
   }

   // Compare input expressions
   if (!((input == otherBetween.input) ||
         (input && otherBetween.input && *input == *otherBetween.input))) {
      return false;
   }

   // Compare lower bounds
   if (!((lower == otherBetween.lower) ||
         (lower && otherBetween.lower && *lower == *otherBetween.lower))) {
      return false;
   }

   // Compare upper bounds
   if (!((upper == otherBetween.upper) ||
         (upper && otherBetween.upper && *upper == *otherBetween.upper))) {
      return false;
   }

   return true;
}

SubqueryExpression::SubqueryExpression(SubqueryType subQueryType, std::shared_ptr<TableProducer> subquery) : ParsedExpression(ExpressionType::SUBQUERY, cType), subQueryType(subQueryType), subquery(subquery) {
}

size_t SubqueryExpression::hash() {
   size_t result = ParsedExpression::hash();

   // Hash subquery type
   result ^= std::hash<uint8_t>{}(static_cast<uint8_t>(subQueryType)) +
      0x9e3779b9 + (result << 6) + (result >> 2);

   // Hash subquery
   if (subquery) {
      //result ^= subquery->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
   }

   // Hash test expression
   if (testExpr) {
      result ^= testExpr->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
   }

   return result;
}

bool SubqueryExpression::operator==(ParsedExpression& other) {
   if (!ParsedExpression::operator==(other)) {
      return false;
   }

   auto& otherSubquery = static_cast<SubqueryExpression&>(other);

   // Compare subquery types
   if (subQueryType != otherSubquery.subQueryType) {
      return false;
   }

   // Compare subqueries
   /*if (!((subquery == otherSubquery.subquery) ||
         (subquery && otherSubquery.subquery && *subquery == *otherSubquery.subquery))) {
      return false;
         }*/

   // Compare test expressions
   if (!((testExpr == otherSubquery.testExpr) ||
         (testExpr && otherSubquery.testExpr && *testExpr == *otherSubquery.testExpr))) {
      return false;
   }

   return true;
}

CaseExpression::CaseExpression(std::optional<std::shared_ptr<ParsedExpression>> caseExpr, std::vector<CaseCheck> caseChecks, std::shared_ptr<ParsedExpression> elseExpr) : ParsedExpression(ExpressionType::CASE_EXPR, cType), caseChecks(std::move(caseChecks)), caseExpr(caseExpr), elseExpr(std::move(elseExpr)) {
}

size_t CaseExpression::hash() {
   size_t result = ParsedExpression::hash();

   // Hash caseExpr if present
   if (caseExpr && *caseExpr) {
      result ^= (*caseExpr)->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
   }

   // Hash all caseChecks (WHEN/THEN pairs)
   for (const auto& check : caseChecks) {
      if (check.whenExpr) {
         result ^= check.whenExpr->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
      }
      if (check.thenExpr) {
         result ^= check.thenExpr->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
      }
   }

   // Hash elseExpr if present
   if (elseExpr) {
      result ^= elseExpr->hash() + 0x9e3779b9 + (result << 6) + (result >> 2);
   }

   return result;
}

bool CaseExpression::operator==(ParsedExpression& other) {
   if (!ParsedExpression::operator==(other)) {
      return false;
   }
   auto& otherCase = static_cast<CaseExpression&>(other);

   // Compare caseExpr
   if (caseExpr.has_value() != otherCase.caseExpr.has_value()) {
      return false;
   }
   if (caseExpr && *caseExpr && otherCase.caseExpr && *otherCase.caseExpr) {
      if ((**caseExpr != **otherCase.caseExpr)) {
         return false;
      }
   }

   // Compare caseChecks
   if (caseChecks.size() != otherCase.caseChecks.size()) {
      return false;
   }
   for (size_t i = 0; i < caseChecks.size(); ++i) {
      const auto& a = caseChecks[i];
      const auto& b = otherCase.caseChecks[i];
      if (!((a.whenExpr == b.whenExpr) ||
            (a.whenExpr && b.whenExpr && *a.whenExpr == *b.whenExpr))) {
         return false;
      }
      if (!((a.thenExpr == b.thenExpr) ||
            (a.thenExpr && b.thenExpr && *a.thenExpr == *b.thenExpr))) {
         return false;
      }
   }

   // Compare elseExpr
   if (!((elseExpr == otherCase.elseExpr) ||
         (elseExpr && otherCase.elseExpr && *elseExpr == *otherCase.elseExpr))) {
      return false;
   }

   return true;
}

SetColumnExpression::SetColumnExpression(std::vector<std::pair<std::shared_ptr<ColumnRefExpression>, std::shared_ptr<ParsedExpression>>> sets) : ParsedExpression(ExpressionType::SET, cType), sets(std::move(sets)) {
}
// ... existing code ...
size_t SetColumnExpression::hash() {
   size_t result = ParsedExpression::hash();

   // Combine hash for each (column, expression) pair in order
   for (const auto& kv : sets) {
      const auto& col = kv.first;
      const auto& expr = kv.second;

      size_t pairHash = 0;
      if (col) {
         pairHash ^= col->hash() + 0x9e3779b9 + (pairHash << 6) + (pairHash >> 2);
      } else {
         // Distinguish null column positions
         pairHash ^= 0x517cc1b7 + (pairHash << 6) + (pairHash >> 2);
      }

      if (expr) {
         pairHash ^= expr->hash() + 0x9e3779b9 + (pairHash << 6) + (pairHash >> 2);
      } else {
         // Distinguish null expression positions
         pairHash ^= 0x85ebca6b + (pairHash << 6) + (pairHash >> 2);
      }

      // Mix into the running result
      result ^= pairHash + 0x9e3779b9 + (result << 6) + (result >> 2);
   }

   return result;
}
bool SetColumnExpression::operator==(ParsedExpression& other) {
   if (!ParsedExpression::operator==(other)) {
      return false;
   }

   auto* otherSet = dynamic_cast<SetColumnExpression*>(&other);
   if (!otherSet) {
      return false;
   }

   if (sets.size() != otherSet->sets.size()) {
      return false;
   }

   for (size_t i = 0; i < sets.size(); ++i) {
      const auto& [colA, exprA] = sets[i];
      const auto& [colB, exprB] = otherSet->sets[i];

      // Compare columns (allow nulls and pointer equality)
      if (!(colA == colB || (colA && colB && *colA == *colB))) {
         return false;
      }
      // Compare expressions (allow nulls and pointer equality)
      if (!(exprA == exprB || (exprA && exprB && *exprA == *exprB))) {
         return false;
      }
   }

   return true;
}

} // namespace lingodb::ast
