//
// Created by mor on 17.10.24.
//
#include "flight_sql_server.h"
arrow::Status startServer();
arrow::Status connectClient(int port);
int main(int argc, char** argv) {
   if(argc != 3) {
      std::cout << "usage: " << argv[0] << " <port>" << std::endl;
   }
   arrow::Status st;
   if(*argv[2] == 'c') {
      std::cout << "Start client" << std::endl;
      st = connectClient(8082);
   } else {
      std::cout << "Start server..." << std::endl;
      st = startServer();
   }



   if(!st.ok()) {
      std::cerr << st << std::endl;

      return 1;
   }
   //st = connectClient();
   if(!st.ok()) {
      std::cerr << st << std::endl;

      return 1;
   }



   return 0;
}
arrow::Status startServer() {
 //  auto reuslt = arrow::flight::Location::Parse("127.0.0.1");
   arrow::flight::Location server_location;
   ARROW_ASSIGN_OR_RAISE(server_location,
                         arrow::flight::Location::ForGrpcTcp("0.0.0.0", 8082));
   auto fs = std::make_shared<arrow::fs::LocalFileSystem>();
   auto root = std::make_shared<arrow::fs::SubTreeFileSystem>("./flight_datasets/", fs);
   arrow::flight::FlightServerOptions options(server_location);
   options.auth_handler = std::make_shared<server::CustomAuthHandler>();

   auto server = std::unique_ptr<arrow::flight::FlightServerBase>(
      new server::FlightSqlServerTestImpl(std::move(root)));
   ARROW_RETURN_NOT_OK(server->Init(options));
   std::cout << "Listening on port " << server->port() << std::endl;







   return  server->Serve();
}

arrow::Status connectClient(int port) {
   ARROW_ASSIGN_OR_RAISE(auto location, arrow::flight::Location::ForGrpcTcp("0.0.0.0", port))
   ARROW_ASSIGN_OR_RAISE(auto client, arrow::flight::FlightClient::Connect(location))



   auto sql_client = std::make_unique<arrow::flight::sql::FlightSqlClient>(std::move(client));

   arrow::flight::FlightCallOptions call_options{};

   auto descriptor = arrow::flight::FlightDescriptor::Path({"airquality.parquet"});


   ARROW_ASSIGN_OR_RAISE(auto info, sql_client->Execute(call_options, "select nice from test;"));

   const auto endpoints = info->endpoints();
   for (auto i = 0; i < endpoints.size(); i++) {
      auto &ticket = endpoints[i].ticket;
      //TODO Move outside an make unique


      ARROW_ASSIGN_OR_RAISE(auto stream, sql_client->DoGet(call_options, ticket));

      auto schema = stream->GetSchema();
      ARROW_RETURN_NOT_OK(schema);

      std::cout << "Schema:" << schema->get()->ToString() << std::endl;
      while(true) {
         ARROW_ASSIGN_OR_RAISE(arrow::flight::FlightStreamChunk chunk, stream->Next());
         if (chunk.data == nullptr) {
            break;
         }
         std::cout << chunk.data->ToString();
      }
   }




  // ARROW_ASSIGN_OR_RAISE(auto info, sql_client->GetFlightInfo(call_options, descriptor));


   return arrow::Status::OK();
}

namespace server {

arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> FlightSqlServerTestImpl::GetFlightInfoStatement(const arrow::flight::ServerCallContext& context, const arrow::flight::sql::StatementQuery& command, const arrow::flight::FlightDescriptor& descriptor) {
   const std::string& query = command.query;

   std::shared_ptr<arrow::Field> field_day, field_month, field_year;
   std::shared_ptr<arrow::Schema> schema;

   // Every field needs its name and data type.
   field_day = arrow::field("Day", arrow::int8());
   field_month = arrow::field("Month", arrow::int8());
   field_year = arrow::field("Year", arrow::int16());

   // The schema can be built from a vector of fields, and we do so here.
   schema = arrow::schema({field_day, field_month, field_year});

   ARROW_ASSIGN_OR_RAISE(auto ticket_string, arrow::flight::sql::CreateStatementQueryTicket("test"));
   arrow::flight::Ticket ticket{std::move(ticket_string)};

   std::vector<arrow::flight::FlightEndpoint> endpoints{
      arrow::flight::FlightEndpoint{ticket, {}, std::nullopt, ""}};
   // TODO: Set true only when "ORDER BY" is used in a main "SELECT"
   // in the given query.
   const bool ordered = false;
   ARROW_ASSIGN_OR_RAISE(
      auto result,
      arrow::flight::FlightInfo::Make(*schema, descriptor, endpoints, 100, 200, ordered, ""));


   return std::make_unique<arrow::flight::FlightInfo>(result);
}

arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> FlightSqlServerTestImpl::DoGetStatement(const arrow::flight::ServerCallContext& context, const arrow::flight::sql::StatementQueryTicket& command) {
   ARROW_ASSIGN_OR_RAISE(auto infile, arrow::io::ReadableFile::Open("/home/mor/projects/lingo-db/resources/server/test/test_in.csv"));
   std::shared_ptr<arrow::Table> csv_table;
   // The CSV reader has several objects for various options. For now, we'll use defaults.
   ARROW_ASSIGN_OR_RAISE(
      auto csv_reader,
      arrow::csv::TableReader::Make(
         arrow::io::default_io_context(), infile, arrow::csv::ReadOptions::Defaults(),
         arrow::csv::ParseOptions::Defaults(), arrow::csv::ConvertOptions::Defaults()));



   ARROW_ASSIGN_OR_RAISE(std::shared_ptr<arrow::Table> table,csv_reader->Read());
   std::vector<std::shared_ptr<arrow::RecordBatch>> batches;

   arrow::TableBatchReader batch_reader(*table);

   ARROW_ASSIGN_OR_RAISE(batches, batch_reader.ToRecordBatches());
   ARROW_ASSIGN_OR_RAISE(auto owning_reader, arrow::RecordBatchReader::Make(

                                                std::move(batches), table->schema()));

   auto stream = std::unique_ptr<arrow::flight::FlightDataStream>(new arrow::flight::RecordBatchStream(owning_reader));


   return stream;
}

//Test
arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> FlightSqlServerTestImpl::GetFlightInfoSqlInfo(const arrow::flight::ServerCallContext& context, const arrow::flight::sql::GetSqlInfo& command, const arrow::flight::FlightDescriptor& descriptor) {
   return FlightSqlServerBase::GetFlightInfoSqlInfo(context, command, descriptor);
}
arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> FlightSqlServerTestImpl::GetFlightInfoPreparedStatement(const arrow::flight::ServerCallContext& context, const arrow::flight::sql::PreparedStatementQuery& command, const arrow::flight::FlightDescriptor& descriptor) {
   const std::string& query = command.prepared_statement_handle;

   std::shared_ptr<arrow::Field> field_day, field_month, field_year;
   std::shared_ptr<arrow::Schema> schema;

   // Every field needs its name and data type.
   field_day = arrow::field("Day", arrow::int8());
   field_month = arrow::field("Month", arrow::int8());
   field_year = arrow::field("Year", arrow::int16());

   // The schema can be built from a vector of fields, and we do so here.
   schema = arrow::schema({field_day, field_month, field_year});

   ARROW_ASSIGN_OR_RAISE(auto ticket_string, arrow::flight::sql::CreateStatementQueryTicket("test"));
   arrow::flight::Ticket ticket{std::move(ticket_string)};

   std::vector<arrow::flight::FlightEndpoint> endpoints{
      arrow::flight::FlightEndpoint{ticket, {}, std::nullopt, ""}};
   // TODO: Set true only when "ORDER BY" is used in a main "SELECT"
   // in the given query.
   const bool ordered = false;
   ARROW_ASSIGN_OR_RAISE(
      auto result,
      arrow::flight::FlightInfo::Make(*schema, descriptor, endpoints, -1, -1, ordered, ""));


   return std::make_unique<arrow::flight::FlightInfo>(result);
}
arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> FlightSqlServerTestImpl::DoGetSqlInfo(const arrow::flight::ServerCallContext& context, const arrow::flight::sql::GetSqlInfo& command) {
   return FlightSqlServerBase::DoGetSqlInfo(context, command);
}
arrow::Result<arrow::flight::sql::ActionCreatePreparedStatementResult> FlightSqlServerTestImpl::CreatePreparedStatement(const arrow::flight::ServerCallContext& context, const arrow::flight::sql::ActionCreatePreparedStatementRequest& request) {
   std::shared_ptr<arrow::Field> field_day, field_month, field_year;
   std::shared_ptr<arrow::Schema> schema;

   // Every field needs its name and data type.
   field_day = arrow::field("Day", arrow::int8());
   field_month = arrow::field("Month", arrow::int8());
   field_year = arrow::field("Year", arrow::int16());

   // The schema can be built from a vector of fields, and we do so here.
   schema = arrow::schema({field_day, field_month, field_year});



   return arrow::flight::sql::ActionCreatePreparedStatementResult{std::move(schema), std::move(schema), "asdasdasdasd"};
}
arrow::Status FlightSqlServerTestImpl::ClosePreparedStatement(const arrow::flight::ServerCallContext& context, const arrow::flight::sql::ActionClosePreparedStatementRequest& request) {
   //TODO
   return arrow::Status::OK();
}

arrow::Status CustomAuthHandler::IsValid(const arrow::flight::ServerCallContext& context, const std::string& token, std::string* peer_identity) {
   return arrow::Status::OK();
}
arrow::Status CustomAuthHandler::Authenticate(const arrow::flight::ServerCallContext& context, arrow::flight::ServerAuthSender* outgoing, arrow::flight::ServerAuthReader* incoming) {
   return  arrow::Status::OK();
}
} // namespace server