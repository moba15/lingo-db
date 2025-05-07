#pragma once
#include <cstdint>
#include <stdexcept>
#include <string>
namespace lingodb::ast {
enum class ConstantType : uint8_t;
class ConstantValue {
   public:
   virtual ~ConstantValue() = default;
   explicit ConstantValue(ConstantType type);
   ConstantType type;

   virtual std::string toString();
};

class IntConstantValue : public ConstantValue {
   public:
   explicit IntConstantValue(int iVal);
   int iVal;
   std::string toString() override;
};

class StringConstantValue : public ConstantValue {
   public:
   explicit StringConstantValue(std::string iVal);
   std::string sVal;
   std::string toString() override;
};

enum class ConstantType : uint8_t {
   INT = 1,
   STRING = 2,
   INVALID = 99
};
} // namespace lingodb::ast