add_library(mlirTypes MLIRTypes.cpp)
target_link_libraries(mlirTypes
        PUBLIC ${dialect_libs}
        ${conversion_libs}
        ${translation_libs}
)

add_library(catalog Catalog.cpp Types.cpp Defs.cpp TableCatalogEntry.cpp IndexCatalogEntry.cpp MetaData.cpp Column.cpp FunctionCatalogEntry.cpp UDFImplementer.cpp)
target_link_libraries(catalog PUBLIC mlirTypes)
set(COMPILE_DEFS "SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\"")
target_compile_definitions(catalog PRIVATE ${COMPILE_DEFS})