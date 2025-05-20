include_directories(lingo-db-analyzer  ${GEN_OUTPUT_DIR})
add_library(lingo-db-analyzer
        sql_scope.cpp
        sql_analyzer.cpp
        sql_context.cpp

        ${BISON_parser_OUTPUTS}
        ${FLEX_lexer_OUTPUTS}

)
target_link_libraries(lingo-db-analyzer PUBLIC lingo-db-parser)

add_executable(lingo-db-analyzer-main main.cpp)
target_link_libraries(lingo-db-analyzer-main lingo-db-analyzer)
target_link_libraries(lingo-db-analyzer-main runner utility runtime mlir-support scheduler)
set_target_properties(lingo-db-analyzer-main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
