add_mlir_dialect_library(MLIRUtil
        UtilDialect.cpp
        UtilOps.cpp
        UtilTypes.cpp
        FunctionHelper.cpp
        ADDITIONAL_HEADER_DIRS
        ../../../../include/lingodb/compiler/Dialect/util

        DEPENDS
        MLIRUtilOpsIncGen

        LINK_LIBS PUBLIC
        MLIRIR
)
