#include "lingodb/runtime/helpers.h"

#include <iostream>

#include "lingodb/runtime/DumpRuntime.h"
#include <arrow/util/decimal.h>
#include <arrow/vendored/datetime.h>
void lingodb::runtime::DumpRuntime::dumpIndex(uint64_t val) {
   std::cout << "index(" << val << ")" << std::endl;
}
void lingodb::runtime::DumpRuntime::dumpInt(bool null, int64_t val) {
   if (null) {
      std::cout << "int(NULL)" << std::endl;
   } else {
      std::cout << "int(" << val << ")" << std::endl;
   }
}
void lingodb::runtime::DumpRuntime::dumpUInt(bool null, uint64_t val) {
   if (null) {
      std::cout << "uint(NULL)" << std::endl;
   } else {
      std::cout << "uint(" << val << ")" << std::endl;
   }
}
void lingodb::runtime::DumpRuntime::dumpBool(bool null, bool val) {
   if (null) {
      std::cout << "bool(NULL)" << std::endl;
   } else {
      std::cout << "bool(" << std::boolalpha << val << ")" << std::endl;
   }
}
void lingodb::runtime::DumpRuntime::dumpDecimal(bool null, uint64_t low, uint64_t high, int32_t scale) {
   if (null) {
      std::cout << "decimal(NULL)" << std::endl;
   } else {
      arrow::Decimal128 decimalrep(arrow::BasicDecimal128(high, low));
      std::cout << "decimal(" << decimalrep.ToString(scale) << ")" << std::endl;
   }
}
void lingodb::runtime::DumpRuntime::dumpDate(bool null, int64_t date) {
   static arrow_vendored::date::sys_days epoch = arrow_vendored::date::sys_days{arrow_vendored::date::jan / 1 / 1970};
   if (null) {
      std::cout << "date(NULL)" << std::endl;
   } else {
      std::cout << "date(" << arrow_vendored::date::format("%F", epoch + std::chrono::nanoseconds{date}) << ")" << std::endl;
   }
}
namespace {
template <class Unit>
void dumpTimestamp(bool null, uint64_t date) {
   static arrow_vendored::date::sys_days epoch = arrow_vendored::date::sys_days{arrow_vendored::date::jan / 1 / 1970};
   if (null) {
      std::cout << "timestamp(NULL)" << std::endl;
   } else {
      std::cout << "timestamp(" << arrow_vendored::date::format("%F %T", epoch + Unit{date}) << ")" << std::endl;
   }
}
} // namespace
void lingodb::runtime::DumpRuntime::dumpTimestampSecond(bool null, uint64_t date) {
   dumpTimestamp<std::chrono::seconds>(null, date);
}
void lingodb::runtime::DumpRuntime::dumpTimestampMilliSecond(bool null, uint64_t date) {
   dumpTimestamp<std::chrono::milliseconds>(null, date);
}
void lingodb::runtime::DumpRuntime::dumpTimestampMicroSecond(bool null, uint64_t date) {
   dumpTimestamp<std::chrono::microseconds>(null, date);
}
void lingodb::runtime::DumpRuntime::dumpTimestampNanoSecond(bool null, uint64_t date) {
   dumpTimestamp<std::chrono::nanoseconds>(null, date);
}

void lingodb::runtime::DumpRuntime::dumpIntervalMonths(bool null, uint32_t interval) {
   if (null) {
      std::cout << "interval(NULL)" << std::endl;
   } else {
      std::cout << "interval(" << interval << " months)" << std::endl;
   }
}
void lingodb::runtime::DumpRuntime::dumpIntervalDaytime(bool null, uint64_t interval) {
   if (null) {
      std::cout << "interval(NULL)" << std::endl;
   } else {
      std::cout << "interval(" << interval << " daytime)" << std::endl;
   }
}
void lingodb::runtime::DumpRuntime::dumpFloat(bool null, double val) {
   if (null) {
      std::cout << "float(NULL)" << std::endl;
   } else {
      std::cout << "float(" << val << ")" << std::endl;
   }
}

void lingodb::runtime::DumpRuntime::dumpString(bool null, lingodb::runtime::VarLen32 string) {
   if (null) {
      std::cout << "string(NULL)" << std::endl;
   } else {
      std::cout << "string(\"" << string.str() << "\")" << std::endl;
   }
}
void lingodb::runtime::DumpRuntime::dumpChar(bool null, uint64_t val, size_t bytes) {
   std::cout << "char<" << bytes << ">";
   if (null) {
      std::cout << "(NULL)" << std::endl;
   } else {
      char chars[sizeof(val)];
      memcpy(chars, &val, sizeof(val));
      std::cout << "(\"" << std::string(chars, bytes) << "\")" << std::endl;
   }
}