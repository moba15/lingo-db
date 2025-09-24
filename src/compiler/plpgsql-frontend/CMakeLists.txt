set(GEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/lingodb/compiler/plpgsql-frontend/sql-parser/gen )
set(BISON_INPUT_FILE ${CMAKE_SOURCE_DIR}/src/compiler/plpgsql-frontend/parser.y)
set(FLEX_INPUT_FILE ${CMAKE_SOURCE_DIR}/src/compiler/plpgsql-frontend/lexer.l)
BISON_TARGET(parser ${BISON_INPUT_FILE} ${GEN_OUTPUT_DIR}/parser.cpp)
FLEX_TARGET(lexer ${FLEX_INPUT_FILE} ${GEN_OUTPUT_DIR}/lexer.cpp)


set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.83.0 REQUIRED COMPONENTS context)
include_directories(${Boost_INCLUDE_DIRS})

ADD_FLEX_BISON_DEPENDENCY(lexer parser)
add_library(MLIRSQLplpgsqlFrontend
        pl_driver.cpp

        ${BISON_parser_OUTPUTS}
        ${FLEX_lexer_OUTPUTS}

)

add_dependencies(MLIRSQLplpgsqlFrontend ec-rt-defs)
target_link_libraries(MLIRSQLplpgsqlFrontend PUBLIC ${dialect_libs}
        ${conversion_libs}
        ${translation_libs}
        runtime_funcs_ptr
        utility
        catalog
        Boost::context


)