# sql-to-mlir
find_package(ArrowFlight REQUIRED )
find_package(ArrowFlightSql REQUIRED )

add_executable(server flight_sql_server.cpp)
target_link_libraries(server PUBLIC runner utility runtime mlir-support scheduler PRIVATE serverLib PRIVATE arrow PRIVATE ArrowFlight::arrow_flight_shared PRIVATE ArrowFlightSql::arrow_flight_sql_shared)
set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
target_link_directories(server PUBLIC ${CMAKE_BINARY_DIR}/lib/execution/cranelift/rust-cranelift/release)

