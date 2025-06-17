set(BISON_INPUT_FILE ${CMAKE_SOURCE_DIR}/src/compiler/frontend/sql-parser/parser.y)
set(FLEX_INPUT_FILE ${CMAKE_SOURCE_DIR}/src/compiler/frontend/sql-parser/lexer.l)
BISON_TARGET(parser ${BISON_INPUT_FILE} ${GEN_OUTPUT_DIR}/parser.cpp)
FLEX_TARGET(lexer ${FLEX_INPUT_FILE} ${GEN_OUTPUT_DIR}/lexer.cpp)


ADD_FLEX_BISON_DEPENDENCY(lexer parser)
add_library(lingo-db-parser
        driver.cpp

        parsed_expression.cpp
        tableref.cpp
        group_by_node.cpp
        pipe_operator.cpp
        aggregation_node.cpp
        result_modifier.cpp
        extend_node.cpp

        query_node/select_node.cpp

        common/node_id_generator.cpp
        common/constant_value.cpp

        ${BISON_parser_OUTPUTS}
        ${FLEX_lexer_OUTPUTS}

    )

add_executable(lingo-db-parser-main main.cpp)
target_link_libraries(lingo-db-parser-main lingo-db-parser)

target_link_libraries(lingo-db-parser-main runner utility runtime mlir-support scheduler)
set_target_properties(lingo-db-parser-main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")