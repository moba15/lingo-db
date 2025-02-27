//
// Created by mor on 10.01.25.
//

#pragma once
namespace server {


class StatementExecution {
   public:
   StatementExecution() = default;
   ~StatementExecution() = default;
   StatementExecution(const StatementExecution&) = default;
   StatementExecution& operator=(const StatementExecution&) = default;
   StatementExecution(StatementExecution&&) = default;
   StatementExecution& operator=(StatementExecution&&) = default;
};
}
