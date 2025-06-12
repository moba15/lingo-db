#pragma once
#include <cstdint>
#include <stdexcept>
#include <string>
namespace lingodb::ast {
enum class ConstantType : uint8_t;
class Value {
   public:
   virtual ~Value() = default;
   explicit Value(ConstantType type);
   ConstantType type;

   virtual std::string toString();
};

class IntValue : public Value {
   public:
   explicit IntValue(int iVal);
   int iVal;
   std::string toString() override;
};

class FloatValue : public Value {
   public:
   explicit FloatValue(std::string fVal);
   //TODO float or double?
   std::string fVal;
   std::string toString() override;
};

class StringValue : public Value {
   public:
   explicit StringValue(std::string iVal);
   std::string sVal;
   std::string toString() override;
};

class Interval {
   public:
   int32_t months;
   int32_t days;
   int64_t micros;
};

class IntervalValue : public Value {
   public:
   explicit IntervalValue(Interval iVal);
   Interval iVal;
   std::string toString() override;
};


enum class ConstantType : uint8_t {
   INT = 1,
   FLOAT = 2,
   STRING = 3,
   INTERVAL = 4,

   INVALID = 99,

};
} // namespace lingodb::ast