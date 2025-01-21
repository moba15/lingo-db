#include "flight_sql_server.h"

#include "runtime/Session.h"
#include "server/parser/ParaParser.h"

#include <libpg_query/src/postgres/include/c.h>
#include <runtime/ArrowTable.h>
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
   auto paras = parser.getParas("select * from hoeren where matrnr=(select matrnr from hoeren where vorlnr=? and matrnr=? and vorlnr=(select * from vorlesungen where sws LIKE ?))");
   for (auto para : *paras) { std::cout << "Found: " << para->name() << std::endl; }*/
   executer->fromData("Select * from region");
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
   auto sessions = std::make_unique<std::unordered_map<std::string, std::shared_ptr<runtime::Session>>>();

   auto statementHandler = std::make_unique<server::StatementHandler>(std::move(statementExecution), 32, session);

   sessions->emplace("tpch", session);

   std::unique_ptr<server::FlightSqlServerTestImpl> server = std::unique_ptr<server::FlightSqlServerTestImpl>(
      new server::FlightSqlServerTestImpl(std::move(root), std::move(sessions), std::move(statementHandler)));
   server->RegisterSqlInfo(arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_READ_ONLY, false);
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

/***
 * ---------------------------------------------------
 * Prepared statement
 * ---------------------------------------------------
 */
arrow::Result<arrow::flight::sql::ActionCreatePreparedStatementResult> FlightSqlServerTestImpl::CreatePreparedStatement(
   const arrow::flight::ServerCallContext& context,
   const arrow::flight::sql::ActionCreatePreparedStatementRequest& request) {
   std::cout << "CreatePreparedStatement" << std::endl;
   CHECK_FOR_VALID_SERVER_SESSION()

   ARROW_ASSIGN_OR_RAISE(auto const handle, statementHandler->addStatementToQueue(request.query));
   ARROW_ASSIGN_OR_RAISE(auto pid, statementHandler->executeQeueryStatement(handle, true));
   if (pid == 0) {
      this->statementHandler.~unique_ptr();
      std::exit(0);

      return arrow::Status::Cancelled("");
   } else {
      ARROW_ASSIGN_OR_RAISE(auto statementInformation, statementHandler->getStatement(handle));
      if (statementInformation->type == StatementType::AD_HOC_QUERY && statementInformation->relations != nullptr) {
         return arrow::flight::sql::ActionCreatePreparedStatementResult{statementInformation->relations->at(0)->getArrowSchema(), nullptr, handle};
      }
      return arrow::flight::sql::ActionCreatePreparedStatementResult{nullptr, nullptr, handle};
   }
}

arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>>
FlightSqlServerTestImpl::GetFlightInfoPreparedStatement(const arrow::flight::ServerCallContext& context,
                                                        const arrow::flight::sql::PreparedStatementQuery& command,
                                                        const arrow::flight::FlightDescriptor& descriptor) {
   CHECK_FOR_VALID_SERVER_SESSION()
   std::cout << "GetFlightInfoPreparedStatement: " << descriptor.cmd << std::endl;

   // The schema can be built from a vector of fields, and we do so here.
   std::shared_ptr<arrow::Schema> schema = sessions->at("tpch")->getCatalog()->findRelation("orders")->getArrowSchema();

   ARROW_ASSIGN_OR_RAISE(auto ticket_string,
                         arrow::flight::sql::CreateStatementQueryTicket(command.prepared_statement_handle));
   arrow::flight::Ticket ticket{std::move(ticket_string)};
   std::vector endpoints{
      arrow::flight::FlightEndpoint{{descriptor.cmd}, {}, std::nullopt, ""}};
   // TODO: Set true only when "ORDER BY" is used in a main "SELECT"
   // in the given query.
   const bool ordered = false;
   ARROW_ASSIGN_OR_RAISE(auto result, arrow::flight::FlightInfo::Make(*schema, descriptor, endpoints, -1, -1, ordered));
   return std::make_unique<arrow::flight::FlightInfo>(result);
}

arrow::Result<int64_t>
FlightSqlServerTestImpl::DoPutPreparedStatementUpdate(const arrow::flight::ServerCallContext& context,
                                                      const arrow::flight::sql::PreparedStatementUpdate& command,
                                                      arrow::flight::FlightMessageReader* reader) {
   std::cout << "DoPutPreparedStatementUpdate" << std::endl;
   ARROW_RETURN_NOT_OK(statementHandler->executeQeueryStatement(command.prepared_statement_handle, false));
   ARROW_ASSIGN_OR_RAISE(auto result, statementHandler->waitAndGetStatementResult(command.prepared_statement_handle));
   if (!std::holds_alternative<int>(result)) {
      return arrow::Status::Invalid("FlightSqlServerTestImpl::DoPutPreparedStatementUpdated");
   }
   return std::get<int>(result);
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
   std::cout << "DoGetPreparedStatement " << std::endl;
   CHECK_FOR_VALID_SERVER_SESSION()
   ARROW_ASSIGN_OR_RAISE(auto result, statementHandler->waitAndGetStatementResult(command.prepared_statement_handle));
   if (!std::holds_alternative<std::unique_ptr<arrow::flight::FlightDataStream>>(result)) {
      return arrow::Status::Invalid("FlightSqlServerTestImpl::DoGetPreparedStatementd");
   }
   return std::move(std::get<std::unique_ptr<arrow::flight::FlightDataStream>>(result));
}

arrow::Status FlightSqlServerTestImpl::ClosePreparedStatement(
   const arrow::flight::ServerCallContext& context,
   const arrow::flight::sql::ActionClosePreparedStatementRequest& request) {
   std::cout << "ClosePreparedStatement " << std::endl;
   CHECK_FOR_VALID_SERVER_SESSION()
   return this->statementHandler->closeStatement(request.prepared_statement_handle);
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
   std::cout << "GetFlightInfoSqlInfo: " << info << std::endl;
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
   std::cout << "DoGetSqlInfo: " << info << std::endl;
   auto schema = arrow::flight::sql::SqlSchema::GetSqlInfoSchema();
   auto fields = schema->fields();
   for (auto field : fields) {
      std::cout << field->ToString() << std::endl;
   }

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
      unionBuilder.AppendChild(children.at(i), fieldNames.at(i));
   }
   ARROW_RETURN_NOT_OK(unionBuilder.Append(0));
   for (size_t i = 0; i < 1000; i++) {
      auto arrowResult = getSqlInfoResult(i);
      if (arrowResult.ok()) {
         ARROW_ASSIGN_OR_RAISE(auto result, arrowResult);
         if (std::holds_alternative<std::string>(result)) {
            ARROW_RETURN_NOT_OK(stringBuilder->Append(std::get<std::string>(result)));

            ARROW_RETURN_NOT_OK(unionBuilder.Append(0));
            ARROW_RETURN_NOT_OK(builder.Append(i));
         } else if (std::holds_alternative<bool>(result)) {
            // ARROW_RETURN_NOT_OK(booleanBuilder->Append(std::get<bool>(result)));

            ARROW_RETURN_NOT_OK(booleanBuilder->Append(std::get<bool>(result)));

            ARROW_RETURN_NOT_OK(unionBuilder.Append(1));
            ARROW_RETURN_NOT_OK(builder.Append(i));
         }
      }
   }
   ARROW_ASSIGN_OR_RAISE(auto info_names, builder.Finish());
   ARROW_ASSIGN_OR_RAISE(auto values, unionBuilder.Finish());

   std::shared_ptr<arrow::RecordBatch> batch =
      arrow::RecordBatch::Make(schema, 1, {std::move(info_names), std::move(values)});
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
   std::cout << "DoGetTableTypes" << std::endl;
   auto schema = arrow::flight::sql::SqlSchema::GetTableTypesSchema();
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
   std::cout << "DoGetCatalogs" << std::endl;
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
arrow::Status CustomAuthHandler::IsValid(const arrow::flight::ServerCallContext& context,
                                         const std::string& token,
                                         std::string* peer_identity) {
   return arrow::Status::OK();
}
arrow::Result<arrow::flight::sql::SqlInfoResult> FlightSqlServerTestImpl::getSqlInfoResult(size_t type) {
   switch (type) {
      case arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_NAME: return "Flight SQL Server Name";
      case arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_VERSION: return "Flight SQL Server Version";
      case arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_ARROW_VERSION: return "Arrow Version";
      case arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_READ_ONLY: return false;
      case arrow::flight::sql::SqlInfoOptions::FLIGHT_SQL_SERVER_SQL: return false;
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
      case arrow::flight::sql::SqlInfoOptions::SQL_DDL_TABLE: return false; //asd
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
   return arrow::Status::OK();
}
std::unique_ptr<std::vector<std::pair<std::string, std::shared_ptr<runtime::Relation>>>> FlightSqlServerTestImpl::getAllPossibleRelations() {
   auto result = std::make_unique<std::vector<std::pair<std::string, std::shared_ptr<runtime::Relation>>>>();
   for (auto session : *sessions) {
      auto catalog_name = session.first;
      auto db_schema_name = session.first;
      auto catalog = session.second->getCatalog();
      if (typeid(*catalog.get()) == typeid(runtime::LocalCatalog)) {
         std::cout << "Is type LOCAL" << std::endl;
         //TODO
      } else if (typeid(*catalog.get()) == typeid(runtime::DBCatalog)) {
         std::cout << "Is type DB" << std::endl;
         auto dbCatalog = std::static_pointer_cast<runtime::DBCatalog>(catalog);
         for (auto relation : dbCatalog->relations) {
            result->emplace_back(std::make_pair(catalog_name, relation.second));
         }
      }
   }
   return result;
}
} // namespace server