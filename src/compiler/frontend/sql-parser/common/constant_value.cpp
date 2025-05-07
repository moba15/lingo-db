#include "lingodb/compiler/frontend/sql-parser/common/constant_value.h"

#include <llvm/Support/Casting.h>

#include <memory>
namespace lingodb::ast {
ConstantValue::ConstantValue(ConstantType type) : type(type) {
}
std::string ConstantValue::toString() {
   throw std::runtime_error("toString() not implemented for ConstantValue");
}
/*
 * IntConstantValue
 */
IntConstantValue::IntConstantValue(int iVal) : ConstantValue(ConstantType::INT), iVal(iVal) {
}
std::string IntConstantValue::toString() {
   return std::to_string(iVal);
}

/*
 * StringConstantValue
 */
StringConstantValue::StringConstantValue(std::string sVal) : ConstantValue(ConstantType::STRING), sVal(sVal) {
}
std::string StringConstantValue::toString() {
   return sVal;
}
} // namespace lingodb::ast