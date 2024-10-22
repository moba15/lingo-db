# sql-to-mlir
find_package(Parquet REQUIRED)
find_package(ArrowFlightSql REQUIRED)
add_executable(server flight_sql_server.cpp)
find_package(Parquet REQUIRED)
target_link_libraries(server PUBLIC runner runtime utility mlir-support  PRIVATE arrow PRIVATE ArrowFlight::arrow_flight_shared PRIVATE ArrowFlightSql::arrow_flight_sql_shared PRIVATE Parquet::parquet_shared)
set_target_properties(server PROPERTIES  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
target_link_directories( server PUBLIC ${CMAKE_BINARY_DIR}/lib/execution/cranelift/rust-cranelift/release)

