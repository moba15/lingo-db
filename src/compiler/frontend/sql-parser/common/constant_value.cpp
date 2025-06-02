#include "lingodb/compiler/frontend/sql-parser/common/constant_value.h"

#include <llvm/Support/Casting.h>

#include <memory>
namespace lingodb::ast {
Value::Value(ConstantType type) : type(type) {
}
std::string Value::toString() {
   return "ConstantValue: " + std::to_string(static_cast<int>(type));
   throw std::runtime_error("toString() not implemented for ConstantValue");
}
/*
 * IntConstantValue
 */
IntValue::IntValue(int iVal) : Value(ConstantType::INT), iVal(iVal) {
}
std::string IntValue::toString() {
   return "int:" + std::to_string(iVal);
}

FloatValue::FloatValue(std::string fVal) : Value(ConstantType::FLOAT), fVal(fVal) {
}
std::string FloatValue::toString() {
   return "float: " + fVal;
}

/*
 * StringConstantValue
 */
StringValue::StringValue(std::string sVal) : Value(ConstantType::STRING), sVal(sVal) {
}
std::string StringValue::toString() {
   return "string: " + sVal;
}

/*
 * IntervalValue
*/
IntervalValue::IntervalValue(Interval iVal) : Value(ConstantType::INTERVAL), iVal(iVal) {
}
std::string IntervalValue::toString() {
   return Value::toString();
}
} // namespace lingodb::ast