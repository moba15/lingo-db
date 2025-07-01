#pragma once
#include <cstdint>
#include <lingodb/compiler/frontend/sql-parser/gen/location.hh>
namespace lingodb::ast {
enum class ConstraintType : std::uint8_t {
   INVALID = 0,     // invalid constraint type
   NOT_NULL = 1,    // NOT NULL constraint
   CHECK = 2,       // CHECK constraint
   UNIQUE = 3,      // UNIQUE constraint
   FOREIGN_KEY = 4, // FOREIGN KEY constraint
   NULLABLE = 5, // NULLABLE constraint
};

class Constraint {
   public:
   Constraint(ConstraintType type) : type(type){}

   ConstraintType type; // The type of the constraint
   location loc;


};
//TODO Maybe add custom classes for different constraints like NotNullConstraint, CheckConstraint, etc.
}