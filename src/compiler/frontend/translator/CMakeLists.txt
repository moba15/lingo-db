include_directories(lingo-db-sql-translator ${GEN_OUTPUT_DIR})
add_library(lingo-db-sql-translator
        sql_mlir_translator.cpp

        context/translation_context.cpp


        ${BISON_parser_OUTPUTS}
        ${FLEX_lexer_OUTPUTS}

)
target_link_libraries(lingo-db-sql-translator PUBLIC lingo-db-parser lingo-db-analyzer)

add_executable(lingo-db-sql-translator-main main.cpp)
target_link_libraries(lingo-db-sql-translator-main lingo-db-sql-translator)
target_link_libraries(lingo-db-sql-translator-main runner utility runtime mlir-support scheduler)
set_target_properties(lingo-db-sql-translator-main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
