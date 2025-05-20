set(BISON_INPUT_FILE ${CMAKE_SOURCE_DIR}/src/compiler/frontend/sql-parser/parser.y)
set(FLEX_INPUT_FILE ${CMAKE_SOURCE_DIR}/src/compiler/frontend/sql-parser/lexer.l)
BISON_TARGET(parser ${BISON_INPUT_FILE} ${GEN_OUTPUT_DIR}/parser.cpp)
FLEX_TARGET(lexer ${FLEX_INPUT_FILE} ${GEN_OUTPUT_DIR}/lexer.cpp)


ADD_FLEX_BISON_DEPENDENCY(lexer parser)
add_library(lingo-db-parser
        driver.cpp

        node_factory.cpp
        base_expression.cpp
        parsed_expression.cpp
        query_node.cpp
        tableref.cpp
        group_by_node.cpp
        ast_node.cpp
        pipe_operator.cpp
        aggregation_node.cpp
        result_modifier.cpp
        tabel_producer.cpp

        query_node/select_node.cpp


        parsed_expression/comparison_expression.cpp
        parsed_expression/columnref_expression.cpp
        parsed_expression/star_expression.cpp
        parsed_expression/constant_expression.cpp
        parsed_expression/conjunction_expression.cpp
        parsed_expression/function_expression.cpp
        parsed_expression/targets_expression.cpp



        tableref/basetableref.cpp
        tableref/joinref.cpp
        tableref/subqueryref.cpp

        common/node_id_generator.cpp
        common/constant_value.cpp

        ${BISON_parser_OUTPUTS}
        ${FLEX_lexer_OUTPUTS}

    )

add_executable(lingo-db-parser-main main.cpp)
target_link_libraries(lingo-db-parser-main lingo-db-parser)