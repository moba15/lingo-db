//
// Created by mor on 17.10.24.
//

#pragma once

#include <arrow/filesystem/api.h>
#include <arrow/flight/sql/api.h>
#include <arrow/flight/sql/server.h>
#include <arrow/io/api.h>
#include <arrow/csv/api.h>
#include <iostream>
#include <parquet/arrow/reader.h>
#include <parquet/arrow/schema.h>
#include <parquet/arrow/writer.h>
namespace server {

class FlightSqlServer {

};


class FlightSqlServerTestImpl : public FlightSqlServer, public arrow::flight::sql::FlightSqlServerBase {
   public:
   explicit FlightSqlServerTestImpl(std::shared_ptr<arrow::fs::FileSystem> root) {}
   arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> GetFlightInfoStatement(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::StatementQuery &command, const arrow::flight::FlightDescriptor &descriptor) override;
  // arrow::Result<std::unique_ptr<arrow::flight::SchemaResult>> GetSchemaStatement(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::StatementQuery &command, const arrow::flight::FlightDescriptor &descriptor) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> DoGetSqlInfo(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::GetSqlInfo &command) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> DoGetStatement(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::StatementQueryTicket &command) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> GetFlightInfoSqlInfo(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::GetSqlInfo &command, const arrow::flight::FlightDescriptor &descriptor) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> GetFlightInfoPreparedStatement(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::PreparedStatementQuery &command, const arrow::flight::FlightDescriptor &descriptor) override;

   /*arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> GetFlightInfoSubstraitPlan(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::StatementSubstraitPlan &command, const arrow::flight::FlightDescriptor &descriptor) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> DoGetStatement(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::StatementQueryTicket &command) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> GetFlightInfoPreparedStatement(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::PreparedStatementQuery &command, const arrow::flight::FlightDescriptor &descriptor) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> DoGetPreparedStatement(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::PreparedStatementQuery &command) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> GetFlightInfoCatalogs(const arrow::flight::ServerCallContext &context, const arrow::flight::FlightDescriptor &descriptor) override;
   arrow::Result<std::unique_ptr<arrow::flight::SchemaResult>> GetSchemaStatement(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::StatementQuery &command, const arrow::flight::FlightDescriptor &descriptor) override;
   arrow::Result<std::unique_ptr<arrow::flight::SchemaResult>> GetSchemaSubstraitPlan(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::StatementSubstraitPlan &command, const arrow::flight::FlightDescriptor &descriptor) override;
   arrow::Result<std::unique_ptr<arrow::flight::SchemaResult>> GetSchemaPreparedStatement(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::PreparedStatementQuery &command, const arrow::flight::FlightDescriptor &descriptor) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> DoGetCatalogs(const arrow::flight::ServerCallContext &context) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> GetFlightInfoXdbcTypeInfo(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::GetXdbcTypeInfo &command, const arrow::flight::FlightDescriptor &descriptor) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> DoGetXdbcTypeInfo(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::GetXdbcTypeInfo &command) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> GetFlightInfoSqlInfo(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::GetSqlInfo &command, const arrow::flight::FlightDescriptor &descriptor) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> DoGetSqlInfo(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::GetSqlInfo &command) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> GetFlightInfoSchemas(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::GetDbSchemas &command, const arrow::flight::FlightDescriptor &descriptor) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> DoGetDbSchemas(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::GetDbSchemas &command) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> GetFlightInfoTables(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::GetTables &command, const arrow::flight::FlightDescriptor &descriptor) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> DoGetTables(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::GetTables &command) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> GetFlightInfoTableTypes(const arrow::flight::ServerCallContext &context, const arrow::flight::FlightDescriptor &descriptor) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> DoGetTableTypes(const arrow::flight::ServerCallContext &context) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> GetFlightInfoPrimaryKeys(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::GetPrimaryKeys &command, const arrow::flight::FlightDescriptor &descriptor) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> DoGetPrimaryKeys(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::GetPrimaryKeys &command) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> GetFlightInfoExportedKeys(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::GetExportedKeys &command, const arrow::flight::FlightDescriptor &descriptor) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> DoGetExportedKeys(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::GetExportedKeys &command) override;
   arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> GetFlightInfoImportedKeys(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::GetImportedKeys &command, const arrow::flight::FlightDescriptor &descriptor) override;
   arrow::Result<int64_t> DoPutCommandStatementUpdate(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::StatementUpdate &command) override;
   arrow::Result<int64_t> DoPutCommandSubstraitPlan(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::StatementSubstraitPlan &command) override;
   arrow::Result<arrow::flight::sql::ActionCreatePreparedStatementResult> CreatePreparedStatement(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::ActionCreatePreparedStatementRequest &request) override;
   arrow::Result<arrow::flight::sql::ActionCreatePreparedStatementResult> CreatePreparedSubstraitPlan(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::ActionCreatePreparedSubstraitPlanRequest &request) override;
   arrow::Status ClosePreparedStatement(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::ActionClosePreparedStatementRequest &request) override;
   arrow::Status DoPutPreparedStatementQuery(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::PreparedStatementQuery &command, arrow::flight::FlightMessageReader *reader, arrow::flight::FlightMetadataWriter *writer) override;
   arrow::Result<int64_t> DoPutPreparedStatementUpdate(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::PreparedStatementUpdate &command, arrow::flight::FlightMessageReader *reader) override;
   arrow::Result<int64_t> DoPutCommandStatementIngest(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::StatementIngest &command, arrow::flight::FlightMessageReader *reader) override;
   arrow::Result<arrow::flight::sql::ActionBeginTransactionResult> BeginTransaction(const arrow::flight::ServerCallContext &context, const arrow::flight::sql::ActionBeginTransactionRequest &request) override;
   arrow::Result<arrow::flight::GetSessionOptionsResult> GetSessionOptions(const arrow::flight::ServerCallContext &context, const arrow::flight::GetSessionOptionsRequest &request) override;*/



};

} // namespace server

