set(BISON_INPUT_FILE ${CMAKE_SOURCE_DIR}/src/compiler/frontend/sql-parser/parser.y)
set(FLEX_INPUT_FILE ${CMAKE_SOURCE_DIR}/src/compiler/frontend/sql-parser/lexer.l)
BISON_TARGET(parser ${BISON_INPUT_FILE} ${GEN_OUTPUT_DIR}/parser.cpp)
FLEX_TARGET(lexer ${FLEX_INPUT_FILE} ${GEN_OUTPUT_DIR}/lexer.cpp)


ADD_FLEX_BISON_DEPENDENCY(lexer parser)
add_library(lingo-db-parser
        driver.cpp

        ../ast/parsed_expression.cpp
        ../ast/tableref.cpp
        ../ast/group_by_node.cpp
        ../ast/pipe_operator.cpp
        ../ast/aggregation_node.cpp
        ../ast/result_modifier.cpp
        ../ast/extend_node.cpp

        ../ast/select_node.cpp

        ${BISON_parser_OUTPUTS}
        ${FLEX_lexer_OUTPUTS}

    )

add_executable(lingo-db-parser-main main.cpp)
target_link_libraries(lingo-db-parser-main lingo-db-parser)

target_link_libraries(lingo-db-parser-main runner utility runtime mlir-support scheduler)
set_target_properties(lingo-db-parser-main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")