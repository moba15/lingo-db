//
// Created by mor on 17.10.24.
//

#pragma once

#include "execution/Execution.h"
#include "runtime/Session.h"
#include "server/ipc/IPCHelper.h"
#include "server/shm/SyncHelper.h"
#include <iostream>
#include <thread>
#include <arrow/csv/api.h>
#include <arrow/filesystem/api.h>
#include <arrow/flight/api.h>
#include <arrow/flight/sql/client.h>
#include <arrow/flight/sql/server.h>
#include <arrow/ipc/api.h>
#include <fcntl.h>
#include <server/StatementHandler.h>
#include <sys/shm.h>
#include <sys/types.h>
#define SharedmemSize 100

#define CHECK_FOR_VALID_SERVER_SESSION() \
   if (sessions == nullptr || sessions->size() == 0) { return arrow::Status::Invalid("Session is null"); }

namespace server {

class FlightSqlServer {};

struct Test {
   ~Test() { std::cout << "delete test" << std::endl; }
};

class FlightSqlServerTestImpl : public FlightSqlServer, public arrow::flight::sql::FlightSqlServerBase {
   public:
   //TODO check if session is given correctly
   explicit FlightSqlServerTestImpl(std::shared_ptr<arrow::fs::FileSystem> root,
                                    std::unique_ptr<std::unordered_map<std::string, std::shared_ptr<runtime::Session>>>
                                       sessions,
                                    std::unique_ptr<StatementHandler>
                                       statementHandler)
      : sessions{std::move(sessions)}, statementHandler(std::move(statementHandler)) {}

   arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>>
   GetFlightInfoStatement(const arrow::flight::ServerCallContext& context,
                          const arrow::flight::sql::StatementQuery& command,
                          const arrow::flight::FlightDescriptor& descriptor) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>>
   DoGetSqlInfo(const arrow::flight::ServerCallContext& context,
                const arrow::flight::sql::GetSqlInfo& command) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>>
   GetFlightInfoSqlInfo(const arrow::flight::ServerCallContext& context, const arrow::flight::sql::GetSqlInfo& command,
                        const arrow::flight::FlightDescriptor& descriptor) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>>
   GetFlightInfoPreparedStatement(const arrow::flight::ServerCallContext& context,
                                  const arrow::flight::sql::PreparedStatementQuery& command,
                                  const arrow::flight::FlightDescriptor& descriptor) override;
   arrow::Result<arrow::flight::sql::ActionCreatePreparedStatementResult>
   CreatePreparedStatement(const arrow::flight::ServerCallContext& context,
                           const arrow::flight::sql::ActionCreatePreparedStatementRequest& request) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>>
   DoGetPreparedStatement(const arrow::flight::ServerCallContext& context,
                          const arrow::flight::sql::PreparedStatementQuery& command) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>>
   GetFlightInfoCatalogs(const arrow::flight::ServerCallContext& context,
                         const arrow::flight::FlightDescriptor& descriptor) override;
   arrow::Status DoPutPreparedStatementQuery(const arrow::flight::ServerCallContext& context,
                                             const arrow::flight::sql::PreparedStatementQuery& command,
                                             arrow::flight::FlightMessageReader* reader,
                                             arrow::flight::FlightMetadataWriter* writer) override;
   arrow::Status
   ClosePreparedStatement(const arrow::flight::ServerCallContext& context,
                          const arrow::flight::sql::ActionClosePreparedStatementRequest& request) override;
   arrow::Result<int64_t> DoPutPreparedStatementUpdate(const arrow::flight::ServerCallContext& context,
                                                       const arrow::flight::sql::PreparedStatementUpdate& command,
                                                       arrow::flight::FlightMessageReader* reader) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>>
   GetFlightInfoTables(const arrow::flight::ServerCallContext& context, const arrow::flight::sql::GetTables& command,
                       const arrow::flight::FlightDescriptor& descriptor) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>>
   DoGetTables(const arrow::flight::ServerCallContext& context, const arrow::flight::sql::GetTables& command) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>>
   DoGetTableTypes(const arrow::flight::ServerCallContext& context) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>>
   GetFlightInfoTableTypes(const arrow::flight::ServerCallContext& context,
                           const arrow::flight::FlightDescriptor& descriptor) override;

   arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> DoGetCatalogs(const arrow::flight::ServerCallContext& context) override;

   arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> GetFlightInfoSchemas(const arrow::flight::ServerCallContext& context, const arrow::flight::sql::GetDbSchemas& command, const arrow::flight::FlightDescriptor& descriptor) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> DoGetDbSchemas(const arrow::flight::ServerCallContext& context, const arrow::flight::sql::GetDbSchemas& command) override;

   arrow::Status start(const arrow::flight::FlightServerOptions& options);

   private:
   std::unique_ptr<std::vector<std::pair<std::string, std::shared_ptr<runtime::Relation>>>>  getAllPossibleRelations();
   std::unique_ptr<std::unordered_map<std::string, std::shared_ptr<runtime::Session>>> sessions;
   std::unique_ptr<StatementHandler> statementHandler;
};



class CustomAuthHandler : public arrow::flight::ServerAuthHandler {
   arrow::Status Authenticate(const arrow::flight::ServerCallContext& context,
                              arrow::flight::ServerAuthSender* outgoing,
                              arrow::flight::ServerAuthReader* incoming) override;
   arrow::Status IsValid(const arrow::flight::ServerCallContext& context, const std::string& token,
                         std::string* peerIdentity) override;
};

} // namespace server
