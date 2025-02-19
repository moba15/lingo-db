# sql-to-mlir
find_package(ArrowFlightSql REQUIRED)
find_package(ArrowFlight REQUIRED)

add_executable(server flight_sql_server.cpp)
set_target_properties(server PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_link_libraries(server PRIVATE serverLib PUBLIC runner runtime utility mlir-support PRIVATE arrow PRIVATE ArrowFlight::arrow_flight_shared PRIVATE ArrowFlightSql::arrow_flight_sql_shared)
set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

