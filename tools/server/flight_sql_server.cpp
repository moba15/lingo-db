#include "flight_sql_server.h"

#include "runtime/Session.h"
#include "server/pararef-finder/ParaParser.h"

#include <runtime/ArrowTable.h>
#include <server/pararef-finder/ParaParser.h>
#include <unistd.h>
arrow::Status startServer(std::string sqlUrl);

arrow::Status connectClient(int port);

int main(int argc, char** argv) {
   if (argc != 3) { std::cout << "usage: " << argv[0] << " <port>" << std::endl; }
   auto build_info = arrow::GetBuildInfo();
   std::cout << "Apache Arrow version: " << build_info.version_string << std::endl;
   arrow::Status st;
   if (*argv[2] == 'c') {
      std::cout << "Start client" << std::endl;
      st = connectClient(8082);
   } else {
      std::cout << "Start server..." << std::endl;
      st = startServer(std::string(argv[2]));
   }

   if (!st.ok()) {
      std::cerr << st << std::endl;
      return 1;
   }
   //st = connectClient();
   if (!st.ok()) {
      std::cerr << st << std::endl;

      return 1;
   }

   return 0;
}
arrow::Status startServer(std::string sqlUrl) {
   //  auto reuslt = arrow::flight::Location::Parse("127.0.0.1");
   std::cout << "Loading: " << sqlUrl << std::endl;
   auto session = runtime::Session::createSession(sqlUrl, true);

   auto executionContext = session->createExecutionContext();
   auto queryExecutionConfig = execution::createQueryExecutionConfig(execution::ExecutionMode::DEFAULT, true);
   auto executer = execution::QueryExecuter::createDefaultExecuter(std::move(queryExecutionConfig), *session);
   /* ParaParser parser{session};
   auto paras = parser.getParas("select * from hoeren where matrnr=(select matrnr from hoeren where vorlnr=? and matrnr=? and vorlnr=(select * from vorlesungen where sws=? ))");
   for (auto para : paras) { std::cout << "Found: " << para->name() << std::endl; }*/
   executer->fromData("select * from hoeren");
   try {
      executer->execute();
   } catch (const std::runtime_error& error) {
      std::cerr << "Error" << std::endl;
      return arrow::Status::Invalid(error.what());
   }

   arrow::flight::Location server_location;
   ARROW_ASSIGN_OR_RAISE(server_location, arrow::flight::Location::ForGrpcTcp("0.0.0.0", 8083));
   auto fs = std::make_shared<arrow::fs::LocalFileSystem>();
   auto root = std::make_shared<arrow::fs::SubTreeFileSystem>("./flight_datasets/", fs);
   arrow::flight::FlightServerOptions options(server_location);
   auto statementExecution = std::make_unique<server::StatementExecution>();
   auto statementHandler = std::make_unique<server::StatementHandler>(std::move(statementExecution), 32);
   auto sessions = std::make_unique<std::vector<std::shared_ptr<runtime::Session>>>();
   sessions->emplace_back(session);

   std::unique_ptr<server::FlightSqlServerTestImpl> server = std::unique_ptr<server::FlightSqlServerTestImpl>(
      new server::FlightSqlServerTestImpl(std::move(root), std::move(sessions), std::move(statementHandler)));
   session->getCatalog()->findRelation("hoeren");
   ARROW_RETURN_NOT_OK(server->start(options));
   std::cout << "Listening on port " << server->port() << std::endl;
   return server->Serve();
}

arrow::Status connectClient(int port) {
   ARROW_ASSIGN_OR_RAISE(auto location, arrow::flight::Location::ForGrpcTcp("0.0.0.0", port))
   ARROW_ASSIGN_OR_RAISE(auto client, arrow::flight::FlightClient::Connect(location))

   auto sql_client = std::make_unique<arrow::flight::sql::FlightSqlClient>(std::move(client));

   arrow::flight::FlightCallOptions call_options{};

   ARROW_ASSIGN_OR_RAISE(auto info, sql_client->Execute(call_options, "select nice from test;"));

   const auto endpoints = info->endpoints();
   for (auto i = 0; i < endpoints.size(); i++) {
      auto& ticket = endpoints[i].ticket;
      //TODO Move outside an make unique

      ARROW_ASSIGN_OR_RAISE(auto stream, sql_client->DoGet(call_options, ticket));

      auto schema = stream->GetSchema();
      ARROW_RETURN_NOT_OK(schema);

      std::cout << "Schema:" << schema->get()->ToString() << std::endl;
      while (true) {
         ARROW_ASSIGN_OR_RAISE(arrow::flight::FlightStreamChunk chunk, stream->Next());
         if (chunk.data == nullptr) { break; }
         std::cout << chunk.data->ToString();
      }
   }

   return arrow::Status::OK();
}

namespace server {

arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>>
FlightSqlServerTestImpl::GetFlightInfoStatement(const arrow::flight::ServerCallContext& context,
                                                const arrow::flight::sql::StatementQuery& command,
                                                const arrow::flight::FlightDescriptor& descriptor) {
   return arrow::Status::NotImplemented("FlightSqlServerTestImpl::GetFlightInfoStatement");
}



arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>>
FlightSqlServerTestImpl::GetFlightInfoSqlInfo(const arrow::flight::ServerCallContext& context,
                                              const arrow::flight::sql::GetSqlInfo& command,
                                              const arrow::flight::FlightDescriptor& descriptor) {
   return arrow::Status::NotImplemented("FlightSqlServerTestImpl::GetFlightInfoSqlInfo");
}
arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>>
FlightSqlServerTestImpl::GetFlightInfoPreparedStatement(const arrow::flight::ServerCallContext& context,
                                                        const arrow::flight::sql::PreparedStatementQuery& command,
                                                        const arrow::flight::FlightDescriptor& descriptor) {
   CHECK_FOR_VALID_SERVER_SESSION()
   std::cout << "GetFlightInfoPreparedStatement " << std::endl;



   // The schema can be built from a vector of fields, and we do so here.
    std::shared_ptr<arrow::Schema> schema = sessions->at(0)->getCatalog()->findRelation("hoeren")->getArrowSchema();

   ARROW_ASSIGN_OR_RAISE(auto ticket_string,
                         arrow::flight::sql::CreateStatementQueryTicket(command.prepared_statement_handle));
   arrow::flight::Ticket ticket{std::move(ticket_string)};
   std::vector endpoints{
      arrow::flight::FlightEndpoint{{descriptor.cmd}, {}, std::nullopt, ""}};
   // TODO: Set true only when "ORDER BY" is used in a main "SELECT"
   // in the given query.
   const bool ordered = false;
   ARROW_ASSIGN_OR_RAISE(auto result, arrow::flight::FlightInfo::Make(*schema, descriptor,
                                                               endpoints, -1, -1, ordered));

   return std::make_unique<arrow::flight::FlightInfo>(result);
}

arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>>
FlightSqlServerTestImpl::DoGetPreparedStatement(const arrow::flight::ServerCallContext& context,
                                                const arrow::flight::sql::PreparedStatementQuery& command) {
   std::cout << "DoGetPreparedStatement " << std::endl;
   CHECK_FOR_VALID_SERVER_SESSION()
   ARROW_ASSIGN_OR_RAISE(auto stream, statementHandler->waitAndGetStatementResult(command.prepared_statement_handle));

   return stream;
}

arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>>
FlightSqlServerTestImpl::GetFlightInfoCatalogs(const arrow::flight::ServerCallContext& context,
                                               const arrow::flight::FlightDescriptor& descriptor) {
   return arrow::Status::NotImplemented("FlightSqlServerTestImpl::GetFlightInfoCatalogs");
}

arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>>
FlightSqlServerTestImpl::DoGetSqlInfo(const arrow::flight::ServerCallContext& context,
                                      const arrow::flight::sql::GetSqlInfo& command) {
   return arrow::Status::NotImplemented("FlightSqlServerTestImpl::DoGetSqlInfo");
}
arrow::Result<arrow::flight::sql::ActionCreatePreparedStatementResult> FlightSqlServerTestImpl::CreatePreparedStatement(
   const arrow::flight::ServerCallContext& context,
   const arrow::flight::sql::ActionCreatePreparedStatementRequest& request) {
   std::cout << "CreatePreparedStatement" << std::endl;
   CHECK_FOR_VALID_SERVER_SESSION()

   ARROW_ASSIGN_OR_RAISE(auto const handle, statementHandler->addStatementToQueue(request.query));
   ARROW_RETURN_NOT_OK(statementHandler->executeStatement(handle, sessions->at(0)));
   auto r = this->sessions->at(0)->getCatalog()->findRelation("hoeren")->getArrowSchema();

   return arrow::flight::sql::ActionCreatePreparedStatementResult{std::move(r), nullptr, handle};
}

arrow::Status
FlightSqlServerTestImpl::DoPutPreparedStatementQuery(const arrow::flight::ServerCallContext& context,
                                                     const arrow::flight::sql::PreparedStatementQuery& command,
                                                     arrow::flight::FlightMessageReader* reader,
                                                     arrow::flight::FlightMetadataWriter* writer) {
   return arrow::Status::NotImplemented("FlightSqlServerTestImpl::DoPutPreparedStatementQuery");
}

arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>>
FlightSqlServerTestImpl::GetFlightInfoTables(const arrow::flight::ServerCallContext& context,
                                             const arrow::flight::sql::GetTables& command,
                                             const arrow::flight::FlightDescriptor& descriptor) {
   return arrow::Status::NotImplemented("FlightSqlServerTestImpl::GetFlightInfoTables");
}

arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>>
FlightSqlServerTestImpl::DoGetTables(const arrow::flight::ServerCallContext& context,
                                     const arrow::flight::sql::GetTables& command) {
   return arrow::Status::OK();
}

arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>>
FlightSqlServerTestImpl::DoGetTableTypes(const arrow::flight::ServerCallContext& context) {
   return FlightSqlServerBase::DoGetTableTypes(context);
};
arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>>
FlightSqlServerTestImpl::GetFlightInfoTableTypes(const arrow::flight::ServerCallContext& context,
                                                 const arrow::flight::FlightDescriptor& descriptor) {
   return arrow::Status::NotImplemented("FlightSqlServerTestImpl::GetFlightInfoTableTypes");
};

arrow::Status FlightSqlServerTestImpl::ClosePreparedStatement(
   const arrow::flight::ServerCallContext& context,
   const arrow::flight::sql::ActionClosePreparedStatementRequest& request) {
   return arrow::Status::OK();
}
arrow::Result<int64_t>
FlightSqlServerTestImpl::DoPutPreparedStatementUpdate(const arrow::flight::ServerCallContext& context,
                                                      const arrow::flight::sql::PreparedStatementUpdate& command,
                                                      arrow::flight::FlightMessageReader* reader) {
   return arrow::Status::NotImplemented("FlightSqlServerTestImpl::DoPutPreparedStatementUpdate");
}

arrow::Status FlightSqlServerTestImpl::start(const arrow::flight::FlightServerOptions& options) {
   return Init(options);
}
arrow::Status CustomAuthHandler::IsValid(const arrow::flight::ServerCallContext& context,
                                         const std::string& token,
                                         std::string* peer_identity) {
   return arrow::Status::OK();
}
arrow::Status CustomAuthHandler::Authenticate(const arrow::flight::ServerCallContext& context,
                                              arrow::flight::ServerAuthSender* outgoing,
                                              arrow::flight::ServerAuthReader* incoming) {
   return arrow::Status::OK();
}
} // namespace server