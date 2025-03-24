#include "flight_sql_server.h"

#include "lingodb/runtime/Session.h"
#include "lingodb/server/parser/ParaParser.h"
#include "lingodb/compiler/mlir-support/eval.h"
#include <libpg_query/src/postgres/include/c.h>
#include <lingodb/runtime/ArrowTable.h>
arrow::Status startServer(size_t port, std::unordered_map<std::string, std::string>& urls);

int main(int argc, char** argv) {
   if (argc < 3) {
      std::cout << "usage: " << argv[0] << " <port> <...dbName:PathToDb>" << std::endl;
      return 1;
   }
   if (argc > 3) {
      std::cout << "Currently only one database at a time is supported " << std::endl;
      return 1;
   }
   auto build_info = arrow::GetBuildInfo();
   std::cout << "Apache Arrow version: " << build_info.version_string << std::endl;
   arrow::Status st;
   std::string portString = argv[1];
   std::stringstream portStringStream(portString);
   size_t port;
   portStringStream >> port;
   if (port == 0) {
      std::cout << "Please use a valid port!" << std::endl;
      return 1;
   }
   std::cout << "Starting server with port: " << port << std::endl;
   std::unordered_map<std::string, std::string> urls{};
   for (int i = 2; i < argc; i++) {
      std::string arg = argv[i];
      std::stringstream ss(arg);
      std::string splitted;
      int place = 0;
      std::string name;
      std::string url;
      while (getline(ss, splitted, ':')) {
         if (place == 0) {
            name = splitted;
         } else if (place == 1) {
            url = splitted;
         }
         place++;
      }
      if (place != 2) {
         std::cout << "Please use: dbName:PathToDb" << std::endl;
         return 1;
      }
      urls.insert({name, url});
   }

   st = startServer(port, urls);

   if (!st.ok()) {
      std::cerr << st << std::endl;
      return 1;
   }
   if (!st.ok()) {
      std::cerr << st << std::endl;

      return 1;
   }

   return 0;
}
arrow::Status startServer(size_t port, std::unordered_map<std::string, std::string>& urls) {
   //  auto reuslt = arrow::flight::Location::Parse("127.0.0.1");

   arrow::flight::Location server_location;
   ARROW_ASSIGN_OR_RAISE(server_location, arrow::flight::Location::ForGrpcTcp("0.0.0.0", port));
   auto fs = std::make_shared<arrow::fs::LocalFileSystem>();
   arrow::flight::FlightServerOptions options(server_location);
   options.auth_handler = std::make_shared<server::CustomAuthHandler>();
   auto statementExecution = std::make_unique<server::StatementExecution>();
   auto sessions = std::make_shared<std::unordered_map<std::string, std::shared_ptr<lingodb::runtime::Session>>>();

   auto statementHandler = std::make_unique<server::StatementHandler>(std::move(statementExecution), 16, sessions);

   for (auto url : urls) {
      std::cout << "Loading: " << url.first << ":" << url.second << std::endl;
      auto session = lingodb::runtime::Session::createSession(url.second, true);
      sessions->emplace("tpch", session);
   }

   std::unique_ptr<server::FlightSqlServerTestImpl> server = std::unique_ptr<server::FlightSqlServerTestImpl>(
      new server::FlightSqlServerTestImpl( sessions, std::move(statementHandler)));
   server->RegisterSqlInfo(arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_READ_ONLY, false);
   lingodb::compiler::support::eval::init();
   auto scheduler = lingodb::scheduler::startScheduler(1);
   ARROW_RETURN_NOT_OK(server->start(options));
   std::cout << "Listening on port " << server->port() << std::endl;
   std::thread server_thread([&server]() {
      if (server->Serve().ok()) {
      } else {
         std::cerr << "Server Server error" << std::endl;
      }
   });
   for (std::string line; std::getline(std::cin, line);) {
      if (line == "stop") {
         ARROW_RETURN_NOT_OK(server->Shutdown());
         server_thread.join();
         return arrow::Status::OK();
      }
   }

   return arrow::Status::OK();
}

namespace server {
arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>>
FlightSqlServerTestImpl::GetFlightInfoStatement(const arrow::flight::ServerCallContext& context,
                                                const arrow::flight::sql::StatementQuery& command,
                                                const arrow::flight::FlightDescriptor& descriptor) {
   CHECK_FOR_VALID_SERVER_SESSION()
   PrintIfDebug("GetFlightInfoStatement" << descriptor.cmd);
   // The schema can be built from a vector of fields, and we do so here.
   auto schema = arrow::schema({arrow::field("id", arrow::int32()),
                                arrow::field("name", arrow::utf8())});
   std::string transaction_query = command.transaction_id;
   transaction_query += ':';
   transaction_query += command.query;
   ARROW_ASSIGN_OR_RAISE(auto ticket_string,
                         arrow::flight::sql::CreateStatementQueryTicket(transaction_query));
   arrow::flight::Ticket ticket{std::move(ticket_string)};
   arrow::flight::Location server_location;
   ARROW_ASSIGN_OR_RAISE(server_location, arrow::flight::Location::ForGrpcTcp("0.0.0.0", 8083));
   std::vector endpoints{
      arrow::flight::FlightEndpoint{{ticket}, {server_location}, std::nullopt, ""}};
   // TODO: Set true only when "ORDER BY" is used in a main "SELECT" in the given query
   const bool ordered = false;
   ARROW_ASSIGN_OR_RAISE(auto result, arrow::flight::FlightInfo::Make(*schema, descriptor, endpoints, -1, -1, ordered));
   return std::make_unique<arrow::flight::FlightInfo>(result);
}

arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> FlightSqlServerTestImpl::DoGetStatement(const arrow::flight::ServerCallContext& context, const arrow::flight::sql::StatementQueryTicket& command) {
   std::cout << "DoGetStatement" << std::endl;
   return arrow::Status::NotImplemented("");
}

/***
 * ---------------------------------------------------
 * Prepared statement
 * ---------------------------------------------------
 */
arrow::Result<arrow::flight::sql::ActionCreatePreparedStatementResult> FlightSqlServerTestImpl::CreatePreparedStatement(
   const arrow::flight::ServerCallContext& context,
   const arrow::flight::sql::ActionCreatePreparedStatementRequest& request) {
   PrintIfDebug("CreatePreparedStatement");
   CHECK_FOR_VALID_SERVER_SESSION()

   ARROW_ASSIGN_OR_RAISE(auto const handle, statementHandler->addStatementToQueue(request.query));

   ARROW_ASSIGN_OR_RAISE(auto statementInformation, statementHandler->getStatement(handle));
   if (statementInformation->type == StatementType::AD_HOC_QUERY && statementInformation->relations != nullptr) {
      return arrow::flight::sql::ActionCreatePreparedStatementResult{statementInformation->relations->at(0)->getArrowSchema(), nullptr, handle};
   }
   return arrow::flight::sql::ActionCreatePreparedStatementResult{nullptr, nullptr, handle};
}

arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>>
FlightSqlServerTestImpl::GetFlightInfoPreparedStatement(const arrow::flight::ServerCallContext& context,
                                                        const arrow::flight::sql::PreparedStatementQuery& command,
                                                        const arrow::flight::FlightDescriptor& descriptor) {
   CHECK_FOR_VALID_SERVER_SESSION()
   PrintIfDebug("GetFlightInfoPreparedStatement" << descriptor.cmd);

   ARROW_ASSIGN_OR_RAISE(auto pid, statementHandler->executeQeueryStatement(command.prepared_statement_handle, true));
   if (pid == 0) {
      this->statementHandler.~unique_ptr();

      std::exit(0);
      return arrow::Status::Cancelled("");
   }
   ARROW_RETURN_NOT_OK(statementHandler->waitAndLoadResult(command.prepared_statement_handle));
   ARROW_ASSIGN_OR_RAISE(auto schema, statementHandler->getSchemaOfStatement(command.prepared_statement_handle));
   std::vector endpoints{
      arrow::flight::FlightEndpoint{{descriptor.cmd}, {}, std::nullopt, ""}};
   // TODO: Set true only when "ORDER BY" is used in a main "SELECT" in the given query
   const bool ordered = false;
   ARROW_ASSIGN_OR_RAISE(auto result, arrow::flight::FlightInfo::Make(*schema, descriptor, endpoints, -1, -1, ordered));
   return std::make_unique<arrow::flight::FlightInfo>(result);
}

arrow::Result<int64_t>
FlightSqlServerTestImpl::DoPutPreparedStatementUpdate(const arrow::flight::ServerCallContext& context,
                                                      const arrow::flight::sql::PreparedStatementUpdate& command,
                                                      arrow::flight::FlightMessageReader* reader) {
   PrintIfDebug("DoPutPreparedStatementUpdate");
   ARROW_RETURN_NOT_OK(statementHandler->executeQeueryStatement(command.prepared_statement_handle, false));
   ARROW_RETURN_NOT_OK(statementHandler->waitAndLoadResult(command.prepared_statement_handle));

   return 0;
}

arrow::Status
FlightSqlServerTestImpl::DoPutPreparedStatementQuery(const arrow::flight::ServerCallContext& context,
                                                     const arrow::flight::sql::PreparedStatementQuery& command,
                                                     arrow::flight::FlightMessageReader* reader,
                                                     arrow::flight::FlightMetadataWriter* writer) {
   return arrow::Status::NotImplemented("FlightSqlServerTestImpl::DoPutPreparedStatementQuery");
}

arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>>
FlightSqlServerTestImpl::DoGetPreparedStatement(const arrow::flight::ServerCallContext& context,
                                                const arrow::flight::sql::PreparedStatementQuery& command) {
   PrintIfDebug("DoGetPreparedStatement")
      CHECK_FOR_VALID_SERVER_SESSION()

         ARROW_ASSIGN_OR_RAISE(auto result, statementHandler->getResultStream(command.prepared_statement_handle))

            return std::move(result);
}

arrow::Status FlightSqlServerTestImpl::ClosePreparedStatement(
   const arrow::flight::ServerCallContext& context,
   const arrow::flight::sql::ActionClosePreparedStatementRequest& request) {
   PrintIfDebug("ClosePreparedStatement")
      CHECK_FOR_VALID_SERVER_SESSION() return this->statementHandler->closeStatement(request.prepared_statement_handle);
}
/***
 * ---------------------------------------------------
 * Metadata
 * ---------------------------------------------------
 */

arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>>
FlightSqlServerTestImpl::GetFlightInfoSqlInfo(const arrow::flight::ServerCallContext& context,
                                              const arrow::flight::sql::GetSqlInfo& command,
                                              const arrow::flight::FlightDescriptor& descriptor) {
   CHECK_FOR_VALID_SERVER_SESSION()
   std::string info(command.info.begin(), command.info.end());
   PrintIfDebug("GetFlightInfoSqlInfo")
      std::vector endpoints{
         arrow::flight::FlightEndpoint{{descriptor.cmd}, {}, std::nullopt, ""}};

   ARROW_ASSIGN_OR_RAISE(auto result, arrow::flight::FlightInfo::Make(*arrow::flight::sql::SqlSchema::GetSqlInfoSchema(), descriptor, endpoints, -1, -1, false));
   return std::make_unique<arrow::flight::FlightInfo>(result);
}
arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>>
FlightSqlServerTestImpl::DoGetSqlInfo(const arrow::flight::ServerCallContext& context,
                                      const arrow::flight::sql::GetSqlInfo& command) {
   CHECK_FOR_VALID_SERVER_SESSION()
   std::string info(command.info.begin(), command.info.end());
   PrintIfDebug("DoGetSqlInfo") auto schema = arrow::flight::sql::SqlSchema::GetSqlInfoSchema();
   arrow::Int32Builder builder{};

   auto stringBuilder = std::make_shared<arrow::StringBuilder>();
   auto booleanBuilder = std::make_shared<arrow::BooleanBuilder>();
   auto bigintBuilder = std::make_shared<arrow::Int64Builder>();
   auto int32_bitmaskBuilder = std::make_shared<arrow::Int32Builder>();
   auto stringListBuilder = std::make_shared<arrow::ListBuilder>(arrow::default_memory_pool(), std::make_shared<arrow::StringBuilder>());
   auto int32_to_int32_list_mapBuilder = std::make_shared<arrow::MapBuilder>(arrow::default_memory_pool(), std::make_shared<arrow::Int32Builder>(), std::make_shared<arrow::ListBuilder>(arrow::default_memory_pool(), std::make_shared<arrow::Int32Builder>()));
   const std::vector<std::shared_ptr<arrow::ArrayBuilder>>& children{{stringBuilder, booleanBuilder, bigintBuilder, int32_bitmaskBuilder, stringListBuilder, int32_to_int32_list_mapBuilder}};
   arrow::DenseUnionBuilder unionBuilder{arrow::default_memory_pool()};

   std::vector<std::string> fieldNames{"string_value", "bool_value", "bigint_value", "int32_bitmask", "string_list", "int32_to_int32_list_map"};
   for (size_t i = 0; i < children.size(); i++) {
      auto result = unionBuilder.AppendChild(children.at(i), fieldNames.at(i));
      std::cout << fieldNames.at(i) << ": " << std::to_string(result) << std::endl;
   }
   for (size_t info_name_id = 0; info_name_id < 1000; info_name_id++) {
      auto arrowResult = getSqlInfoResult(info_name_id);
      if (arrowResult.ok()) {
         ARROW_ASSIGN_OR_RAISE(auto result, arrowResult);
         if (std::holds_alternative<std::string>(result)) {
            ARROW_RETURN_NOT_OK(unionBuilder.Append(0));
            ARROW_RETURN_NOT_OK(stringBuilder->Append(std::get<std::string>(result)));
            ARROW_RETURN_NOT_OK(builder.Append(info_name_id));
         } else if (std::holds_alternative<bool>(result)) {
            ARROW_RETURN_NOT_OK(unionBuilder.Append(1));
            ARROW_RETURN_NOT_OK(booleanBuilder->Append(std::get<bool>(result)));
            ARROW_RETURN_NOT_OK(builder.Append(info_name_id));
         } else if (std::holds_alternative<int32>(result)) {
            ARROW_RETURN_NOT_OK(unionBuilder.Append(1));
            ARROW_RETURN_NOT_OK(int32_bitmaskBuilder->Append(std::get<int32>(result)));
            ARROW_RETURN_NOT_OK(builder.Append(info_name_id));
         }
      }
   }
   ARROW_ASSIGN_OR_RAISE(auto info_names, builder.Finish());
   ARROW_ASSIGN_OR_RAISE(auto values, unionBuilder.Finish());

   std::shared_ptr<arrow::RecordBatch> batch =
      arrow::RecordBatch::Make(schema, 4, {std::move(info_names), std::move(values)});
   std::cout << batch->ToString() << std::endl;
   ARROW_ASSIGN_OR_RAISE(auto reader, arrow::RecordBatchReader::Make({batch}));
   return std::make_unique<arrow::flight::RecordBatchStream>(reader);
}

arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>>
FlightSqlServerTestImpl::GetFlightInfoTables(const arrow::flight::ServerCallContext& context,
                                             const arrow::flight::sql::GetTables& command,
                                             const arrow::flight::FlightDescriptor& descriptor) {
   CHECK_FOR_VALID_SERVER_SESSION()
   std::vector endpoints{
      arrow::flight::FlightEndpoint{{descriptor.cmd}, {}, std::nullopt, ""}};
   ARROW_ASSIGN_OR_RAISE(auto result, arrow::flight::FlightInfo::Make(*arrow::flight::sql::SqlSchema::GetTablesSchema(), descriptor, endpoints, -1, -1, false));
   return std::make_unique<arrow::flight::FlightInfo>(result);
}
arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>>
FlightSqlServerTestImpl::DoGetTables(const arrow::flight::ServerCallContext& context,
                                     const arrow::flight::sql::GetTables& command) {
   auto schema = arrow::flight::sql::SqlSchema::GetTablesSchema();
   auto fields = schema->fields();
   for (auto& field : fields) {
      std::cout << field->ToString() << std::endl;
   }

   auto relations = getAllPossibleRelations();
   std::vector<std::string> catalog_names_raw{};
   std::vector<std::string> db_schema_names_raw;
   std::vector<std::string> table_names_raw;
   std::vector<std::string> table_types_raw;

   for (auto& relation : *relations) {
      catalog_names_raw.push_back(relation.first);
      db_schema_names_raw.push_back(relation.first);
      table_names_raw.push_back(relation.second->getName());
      table_types_raw.push_back("TABLE");
   }
   arrow::StringBuilder builder{};
   ARROW_RETURN_NOT_OK(builder.AppendValues(catalog_names_raw));
   ARROW_ASSIGN_OR_RAISE(auto catalog_names, builder.Finish());

   ARROW_RETURN_NOT_OK(builder.AppendValues(db_schema_names_raw));
   ARROW_ASSIGN_OR_RAISE(auto db_schema_names, builder.Finish());

   ARROW_RETURN_NOT_OK(builder.AppendValues(table_names_raw));
   ARROW_ASSIGN_OR_RAISE(auto table_names, builder.Finish());

   ARROW_RETURN_NOT_OK(builder.AppendValues(table_types_raw));
   ARROW_ASSIGN_OR_RAISE(auto table_types, builder.Finish());

   std::shared_ptr<arrow::RecordBatch> batch =
      arrow::RecordBatch::Make(schema, relations->size(), {std::move(catalog_names), std::move(db_schema_names), std::move(table_names), std::move(table_types)});
   ARROW_ASSIGN_OR_RAISE(auto reader, arrow::RecordBatchReader::Make({batch}));
   return std::make_unique<arrow::flight::RecordBatchStream>(reader);
}

arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>>
FlightSqlServerTestImpl::GetFlightInfoTableTypes(const arrow::flight::ServerCallContext& context,
                                                 const arrow::flight::FlightDescriptor& descriptor) {
   CHECK_FOR_VALID_SERVER_SESSION()
   std::vector endpoints{
      arrow::flight::FlightEndpoint{{descriptor.cmd}, {}, std::nullopt, ""}};
   ARROW_ASSIGN_OR_RAISE(auto result, arrow::flight::FlightInfo::Make(*arrow::flight::sql::SqlSchema::GetTableTypesSchema(), descriptor, endpoints, -1, -1, false));
   return std::make_unique<arrow::flight::FlightInfo>(result);
}
arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>>
FlightSqlServerTestImpl::DoGetTableTypes(const arrow::flight::ServerCallContext& context) {
   PrintIfDebug("DoGetTableTypes") auto schema = arrow::flight::sql::SqlSchema::GetTableTypesSchema();
   arrow::StringBuilder builder{};
   ARROW_RETURN_NOT_OK(builder.Append("TABLE"));
   ARROW_ASSIGN_OR_RAISE(auto tableTypes, builder.Finish());
   std::shared_ptr<arrow::RecordBatch> batch =
      arrow::RecordBatch::Make(schema, 1, {std::move(tableTypes)});
   ARROW_ASSIGN_OR_RAISE(auto reader, arrow::RecordBatchReader::Make({batch}));
   return std::make_unique<arrow::flight::RecordBatchStream>(reader);
};

arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> FlightSqlServerTestImpl::GetFlightInfoSchemas(const arrow::flight::ServerCallContext& context, const arrow::flight::sql::GetDbSchemas& command, const arrow::flight::FlightDescriptor& descriptor) {
   CHECK_FOR_VALID_SERVER_SESSION()
   std::vector endpoints{
      arrow::flight::FlightEndpoint{{descriptor.cmd}, {}, std::nullopt, ""}};
   ARROW_ASSIGN_OR_RAISE(auto result, arrow::flight::FlightInfo::Make(*arrow::flight::sql::SqlSchema::GetDbSchemasSchema(), descriptor, endpoints, -1, -1, false));
   return std::make_unique<arrow::flight::FlightInfo>(result);
}
arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> FlightSqlServerTestImpl::DoGetDbSchemas(const arrow::flight::ServerCallContext& context, const arrow::flight::sql::GetDbSchemas& command) {
   auto schema = arrow::flight::sql::SqlSchema::GetDbSchemasSchema();
   auto fields = schema->fields();
   for (auto field : fields) {
      std::cout << field->ToString() << std::endl;
   }
   std::vector<std::string> catalogAndDBSchemaNames{sessions->size()};
   size_t i = 0;
   for (auto& session : *sessions) {
      catalogAndDBSchemaNames[i] = session.first;
      i++;
   }
   arrow::StringBuilder builder{};
   ARROW_RETURN_NOT_OK(builder.AppendValues(catalogAndDBSchemaNames));
   std::shared_ptr<arrow::Array> catalogNames;
   ARROW_ASSIGN_OR_RAISE(catalogNames, builder.Finish());

   ARROW_RETURN_NOT_OK(builder.AppendValues(catalogAndDBSchemaNames));
   std::shared_ptr<arrow::Array> dbSchemaName;
   ARROW_ASSIGN_OR_RAISE(dbSchemaName, builder.Finish());
   std::cout << catalogNames->length() << std::endl;
   std::cout << dbSchemaName->length() << std::endl;
   std::shared_ptr<arrow::RecordBatch> batch =
      arrow::RecordBatch::Make(schema, sessions->size(), {std::move(catalogNames), std::move(dbSchemaName)});
   ARROW_ASSIGN_OR_RAISE(auto reader, arrow::RecordBatchReader::Make({batch}));
   return std::make_unique<arrow::flight::RecordBatchStream>(reader);
}

arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>>
FlightSqlServerTestImpl::GetFlightInfoCatalogs(const arrow::flight::ServerCallContext& context,
                                               const arrow::flight::FlightDescriptor& descriptor) {
   CHECK_FOR_VALID_SERVER_SESSION()
   std::vector endpoints{
      arrow::flight::FlightEndpoint{{descriptor.cmd}, {}, std::nullopt, ""}};
   ARROW_ASSIGN_OR_RAISE(auto result, arrow::flight::FlightInfo::Make(*arrow::flight::sql::SqlSchema::GetCatalogsSchema(), descriptor, endpoints, -1, -1, false));
   return std::make_unique<arrow::flight::FlightInfo>(result);
}
arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> FlightSqlServerTestImpl::DoGetCatalogs(const arrow::flight::ServerCallContext& context) {
   PrintIfDebug("DoGetCatalogs")
      const std::shared_ptr<arrow::Schema>& schema =
         arrow::flight::sql::SqlSchema::GetCatalogsSchema();
   arrow::StringBuilder catalog_name_builder;

   for (auto session : *sessions) {
      ARROW_RETURN_NOT_OK(catalog_name_builder.Append(session.first));
   }

   ARROW_ASSIGN_OR_RAISE(auto catalog_name, catalog_name_builder.Finish());
   std::shared_ptr<arrow::RecordBatch> batch =
      arrow::RecordBatch::Make(schema, sessions->size(), {std::move(catalog_name)});
   ARROW_ASSIGN_OR_RAISE(auto reader, arrow::RecordBatchReader::Make({batch}));
   return std::make_unique<arrow::flight::RecordBatchStream>(reader);
};

/***
 *---------------------------------------------------
 * Utility
 *---------------------------------------------------
 */
arrow::Status FlightSqlServerTestImpl::start(const arrow::flight::FlightServerOptions& options) {

   return Init(options);
}

arrow::Result<arrow::flight::sql::SqlInfoResult> FlightSqlServerTestImpl::getSqlInfoResult(size_t type) {
   switch (type) {
      case arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_NAME: return "Flight SQL Server Name";
      case arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_VERSION: return "Flight SQL Server Version";
      case arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_ARROW_VERSION: return "Arrow Version";
      case arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_READ_ONLY: return false;
      case arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_SQL: return true;
      case arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_SUBSTRAIT: return false;
      case arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_SUBSTRAIT_MIN_VERSION: return "";
      case arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_SUBSTRAIT_MAX_VERSION: return "";
      case arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_TRANSACTION: return arrow::flight::sql::SqlInfoOptions::SqlSupportedTransaction::SQL_SUPPORTED_TRANSACTION_NONE;
      case arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_CANCEL: return false;
      case arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_BULK_INGESTION: return false;
      case arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_INGEST_TRANSACTIONS_SUPPORTED: return false;
      case arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_STATEMENT_TIMEOUT: return 0;
      case arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_TRANSACTION_TIMEOUT: return 0;
      case arrow::flight::sql::SqlInfoOptions::SQL_DDL_CATALOG: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_DDL_SCHEMA: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_DDL_TABLE: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_IDENTIFIER_CASE: return arrow::flight::sql::SqlInfoOptions::SqlSupportedCaseSensitivity::SQL_CASE_SENSITIVITY_UNKNOWN;
      case arrow::flight::sql::SqlInfoOptions::SQL_IDENTIFIER_QUOTE_CHAR: return "Identifier Quote";
      case arrow::flight::sql::SqlInfoOptions::SQL_QUOTED_IDENTIFIER_CASE: return arrow::flight::sql::SqlInfoOptions::SqlSupportedCaseSensitivity::SQL_CASE_SENSITIVITY_UNKNOWN;
      case arrow::flight::sql::SqlInfoOptions::SQL_ALL_TABLES_ARE_SELECTABLE: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_NULL_ORDERING: return arrow::flight::sql::SqlInfoOptions::SqlNullOrdering::SQL_NULLS_SORTED_AT_END;
      case arrow::flight::sql::SqlInfoOptions::SQL_KEYWORDS: return {"asd"};
      case arrow::flight::sql::SqlInfoOptions::SQL_NUMERIC_FUNCTIONS: return {"asd"};
      case arrow::flight::sql::SqlInfoOptions::SQL_STRING_FUNCTIONS: return {"asd"};
      case arrow::flight::sql::SqlInfoOptions::SQL_SYSTEM_FUNCTIONS: return {"asd"};
      case arrow::flight::sql::SqlInfoOptions::SQL_DATETIME_FUNCTIONS: return {"asd"};
      case arrow::flight::sql::SqlInfoOptions::SQL_SEARCH_STRING_ESCAPE: return {"asd"};
      case arrow::flight::sql::SqlInfoOptions::SQL_EXTRA_NAME_CHARACTERS: return {"asd"};
      case arrow::flight::sql::SqlInfoOptions::SQL_SUPPORTS_COLUMN_ALIASING: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_NULL_PLUS_NULL_IS_NULL: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_SUPPORTS_CONVERT: return std::unordered_map<int32_t, std::vector<int32_t>>{{1, {2, 3}}};
      case arrow::flight::sql::SqlInfoOptions::SQL_SUPPORTS_TABLE_CORRELATION_NAMES: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_SUPPORTS_DIFFERENT_TABLE_CORRELATION_NAMES: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_SUPPORTS_EXPRESSIONS_IN_ORDER_BY: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_SUPPORTS_ORDER_BY_UNRELATED: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_SUPPORTED_GROUP_BY: return 0;
      case arrow::flight::sql::SqlInfoOptions::SQL_SUPPORTS_LIKE_ESCAPE_CLAUSE: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_SUPPORTS_NON_NULLABLE_COLUMNS: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_SUPPORTED_GRAMMAR: return 0;
      case arrow::flight::sql::SqlInfoOptions::SQL_ANSI92_SUPPORTED_LEVEL: return 0;
      case arrow::flight::sql::SqlInfoOptions::SQL_SUPPORTS_INTEGRITY_ENHANCEMENT_FACILITY: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_OUTER_JOINS_SUPPORT_LEVEL: return 0;
      case arrow::flight::sql::SqlInfoOptions::SQL_SCHEMA_TERM: return "";
      case arrow::flight::sql::SqlInfoOptions::SQL_PROCEDURE_TERM: return "";
      case arrow::flight::sql::SqlInfoOptions::SQL_CATALOG_TERM: return "";
      case arrow::flight::sql::SqlInfoOptions::SQL_CATALOG_AT_START: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_SCHEMAS_SUPPORTED_ACTIONS: return 0b0;
      case arrow::flight::sql::SqlInfoOptions::SQL_CATALOGS_SUPPORTED_ACTIONS: return 0b0;
      case arrow::flight::sql::SqlInfoOptions::SQL_SUPPORTED_POSITIONED_COMMANDS: return 0b0;
      case arrow::flight::sql::SqlInfoOptions::SQL_SELECT_FOR_UPDATE_SUPPORTED: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_STORED_PROCEDURES_SUPPORTED: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_SUPPORTED_SUBQUERIES: return 0b0;
      case arrow::flight::sql::SqlInfoOptions::SQL_CORRELATED_SUBQUERIES_SUPPORTED: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_SUPPORTED_UNIONS: return 0b0;
      case arrow::flight::sql::SqlInfoOptions::SQL_MAX_BINARY_LITERAL_LENGTH: return (int64) static_cast<int64_t>(10); ;
      case arrow::flight::sql::SqlInfoOptions::SQL_MAX_CHAR_LITERAL_LENGTH: return (int64) static_cast<int64_t>(10);
      case arrow::flight::sql::SqlInfoOptions::SQL_MAX_COLUMN_NAME_LENGTH: return (int64) static_cast<int64>(10); ;
      case arrow::flight::sql::SqlInfoOptions::SQL_MAX_COLUMNS_IN_GROUP_BY: return (int64) static_cast<int64>(10); ;
      case arrow::flight::sql::SqlInfoOptions::SQL_MAX_COLUMNS_IN_INDEX: return (int64) static_cast<int64>(10); ;
      case arrow::flight::sql::SqlInfoOptions::SQL_MAX_COLUMNS_IN_ORDER_BY: return (int64) static_cast<int64>(10); ;
      case arrow::flight::sql::SqlInfoOptions::SQL_MAX_COLUMNS_IN_SELECT: return (int64) static_cast<int64>(10); ;
      case arrow::flight::sql::SqlInfoOptions::SQL_MAX_COLUMNS_IN_TABLE: return (int64) static_cast<int64>(10); ;
      case arrow::flight::sql::SqlInfoOptions::SQL_MAX_CONNECTIONS: return (int64) static_cast<int64>(10); ;
      case arrow::flight::sql::SqlInfoOptions::SQL_MAX_CURSOR_NAME_LENGTH: return (int64) static_cast<int64>(10); ;
      case arrow::flight::sql::SqlInfoOptions::SQL_MAX_INDEX_LENGTH: return (int64) static_cast<int64>(10); ;
      case arrow::flight::sql::SqlInfoOptions::SQL_SCHEMA_NAME_LENGTH: return (int64) static_cast<int64>(10); ;
      case arrow::flight::sql::SqlInfoOptions::SQL_MAX_PROCEDURE_NAME_LENGTH: return (int64) static_cast<int64>(10); ;
      case arrow::flight::sql::SqlInfoOptions::SQL_MAX_CATALOG_NAME_LENGTH: return (int64) static_cast<int64>(10); ;
      case arrow::flight::sql::SqlInfoOptions::SQL_MAX_ROW_SIZE: return (int64) static_cast<int64>(10); ;
      case arrow::flight::sql::SqlInfoOptions::SQL_MAX_ROW_SIZE_INCLUDES_BLOBS: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_MAX_STATEMENT_LENGTH: return (int32) static_cast<int32>(10); ;
      case arrow::flight::sql::SqlInfoOptions::SQL_MAX_STATEMENTS: return (int32) static_cast<int32>(10); ;
      case arrow::flight::sql::SqlInfoOptions::SQL_MAX_TABLE_NAME_LENGTH: return (int32) static_cast<int32>(10); ;
      case arrow::flight::sql::SqlInfoOptions::SQL_MAX_TABLES_IN_SELECT: return (int32) static_cast<int32>(10); ;
      case arrow::flight::sql::SqlInfoOptions::SQL_MAX_USERNAME_LENGTH: return (int32) static_cast<int32>(10); ;
      case arrow::flight::sql::SqlInfoOptions::SQL_DEFAULT_TRANSACTION_ISOLATION: return 0;
      case arrow::flight::sql::SqlInfoOptions::SQL_TRANSACTIONS_SUPPORTED: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_SUPPORTED_TRANSACTIONS_ISOLATION_LEVELS: return 0b0;
      case arrow::flight::sql::SqlInfoOptions::SQL_DATA_DEFINITION_CAUSES_TRANSACTION_COMMIT: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_DATA_DEFINITIONS_IN_TRANSACTIONS_IGNORED: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_SUPPORTED_RESULT_SET_TYPES: return 0b0;
      case arrow::flight::sql::SqlInfoOptions::SQL_SUPPORTED_CONCURRENCIES_FOR_RESULT_SET_UNSPECIFIED: return 0b0;
      case arrow::flight::sql::SqlInfoOptions::SQL_SUPPORTED_CONCURRENCIES_FOR_RESULT_SET_FORWARD_ONLY: return 0b0;
      case arrow::flight::sql::SqlInfoOptions::SQL_SUPPORTED_CONCURRENCIES_FOR_RESULT_SET_SCROLL_SENSITIVE: return 0b0;
      case arrow::flight::sql::SqlInfoOptions::SQL_SUPPORTED_CONCURRENCIES_FOR_RESULT_SET_SCROLL_INSENSITIVE: return 0b0;
      case arrow::flight::sql::SqlInfoOptions::SQL_BATCH_UPDATES_SUPPORTED: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_SAVEPOINTS_SUPPORTED: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_NAMED_PARAMETERS_SUPPORTED: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_LOCATORS_UPDATE_COPY: return false;
      case arrow::flight::sql::SqlInfoOptions::SQL_STORED_FUNCTIONS_USING_CALL_SYNTAX_SUPPORTED: return false;
   }
   return arrow::Status::Invalid("");
}
arrow::Status CustomAuthHandler::Authenticate(const arrow::flight::ServerCallContext& context,
                                              arrow::flight::ServerAuthSender* outgoing,
                                              arrow::flight::ServerAuthReader* incoming) {
   PrintIfDebug("Authenticate");
   return arrow::Status::OK();
}
arrow::Status CustomAuthHandler::IsValid(const arrow::flight::ServerCallContext& context,
                                         const std::string& token,
                                         std::string* peer_identity) {
   PrintIfDebug("IsValid: token:" << token << "peer identity:" << peer_identity);
   return arrow::Status::OK();
}
std::unique_ptr<std::vector<std::pair<std::string, std::shared_ptr<lingodb::runtime::Relation>>>> FlightSqlServerTestImpl::getAllPossibleRelations() {
   auto result = std::make_unique<std::vector<std::pair<std::string, std::shared_ptr<lingodb::runtime::Relation>>>>();
   for (auto session : *sessions) {
      auto catalog_name = session.first;
      auto db_schema_name = session.first;
      auto catalog = session.second->getCatalog();
      if (typeid(*catalog.get()) == typeid(lingodb::runtime::DBCatalog)) {
         PrintIfDebug("Is type LOCAL")
         //TODO
      } else if (typeid(*catalog.get()) == typeid(lingodb::runtime::DBCatalog)) {
         PrintIfDebug("Is type DB") auto dbCatalog = std::static_pointer_cast<lingodb::runtime::DBCatalog>(catalog);
         for (auto relation : dbCatalog->relations) {
            result->emplace_back(std::make_pair(catalog_name, relation.second));
         }
      }
   }
   return result;
}
} // namespace server