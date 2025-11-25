cmake_minimum_required(VERSION 3.13.4)
if (POLICY CMP0116)
    cmake_policy(SET CMP0116 OLD)
endif ()
project(lingodb LANGUAGES CXX C)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard to conform to")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -DTRACER")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Silence warning about default label in switch which covers all enumeration values (false positive)
    add_compile_options(-Wno-covered-switch-default)
    # Silence warning about dangling assignment in GSL (false positive)
    add_compile_options(-Wno-dangling-assignment-gsl)
endif ()

# Define ASAN build type if not already set
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif ()

# Add custom ASAN build type
set(CMAKE_CXX_FLAGS_ASAN "-fsanitize=address -fno-omit-frame-pointer -g -O1 -DASAN_ACTIVE")
set(CMAKE_C_FLAGS_ASAN "-fsanitize=address -fno-omit-frame-pointer -g -O1 -DASAN_ACTIVE")
set(CMAKE_EXE_LINKER_FLAGS_ASAN "-fsanitize=address")
# Apply the flags if the build type is ASAN
if (CMAKE_BUILD_TYPE STREQUAL "ASAN")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_ASAN}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_ASAN}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_ASAN}")
endif ()

include(FindZLIB)
message("Using ZLIB: ${ZLIB_INCLUDE_DIRS}")
message("Using Python3: ${Python3_EXECUTABLE}")

find_package(Arrow 21 REQUIRED)
find_package(ArrowCompute 21 REQUIRED)
option(ENABLE_GPU_BACKEND "enable GPU backend" OFF)
option(ENABLE_BASELINE_BACKEND "enable tpde baseline backend" OFF)
option(ENABLE_TESTS "enable tests" ON)


find_package(Clang 20.1 REQUIRED)
find_package(LLVM 20.1 REQUIRED)
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
set(MLIR_DIR "${LLVM_DIR}/../mlir")
message(STATUS "Using MLIRConfig.cmake in: ${MLIR_DIR}")
find_package(MLIR 20.1 REQUIRED)

set(LLVM_RUNTIME_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/bin)
set(LLVM_LIBRARY_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/lib)
set(MLIR_BINARY_DIR ${CMAKE_BINARY_DIR})

list(APPEND CMAKE_MODULE_PATH "${MLIR_CMAKE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${CLANG_CMAKE_DIR}")
include(TableGen)
include(AddLLVM)
include(AddMLIR)
include(AddClang)
include(HandleLLVMOptions)
set("CLANG_VERSION" ${LLVM_VERSION_MAJOR}.${LLVM_VERSION_MINOR}.${LLVM_VERSION_PATCH})
include_directories(${ARROW_INCLUDE_DIR})

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${MLIR_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/vendored)

add_subdirectory(${PROJECT_SOURCE_DIR}/vendored/linenoise-ng/ EXCLUDE_FROM_ALL)


include_directories(${PROJECT_BINARY_DIR}/include)
link_directories(${LLVM_BUILD_LIBRARY_DIR})
add_definitions(${LLVM_DEFINITIONS})
set(LLVM_LINK_COMPONENTS
        Support
        Core
        nativecodegen
        OrcJIT
)
if (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    set(LLVM_LINK_COMPONENTS ${LLVM_LINK_COMPONENTS} AArch64)
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    set(LLVM_LINK_COMPONENTS ${LLVM_LINK_COMPONENTS} X86)
else ()
    message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif ()

add_custom_target(build_includes)
add_custom_target(build_dialect_includes)
add_dependencies(build_includes build_dialect_includes)
set(runtime_funcs_ptr_libs "")


# Add WAMR
# https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/doc/build_wamr.md#wamr-vmcore-cmake-building-configurations
if (APPLE)
    set (WAMR_BUILD_PLATFORM "darwin")
    set (WAMR_BUILD_TARGET "AARCH64")
else ()
    set (WAMR_BUILD_PLATFORM "linux")
    set (WAMR_BUILD_TARGET "X86_64")
endif ()
set (WAMR_BUILD_INTERP 0)
set (WAMR_BUILD_FAST_INTERP 0)
set (WAMR_BUILD_AOT 1)
set (WAMR_BUILD_LIBC_BUILTIN 1)
set (WAMR_BUILD_LIBC_WASI 1)
set (WAMR_BUILD_SIMD 1)
set (WAMR_BUILD_REF_TYPES 1)
set (WAMR_ROOT_DIR ${PROJECT_SOURCE_DIR}/vendored/wasm-micro-runtime)
include (${WAMR_ROOT_DIR}/build-scripts/runtime_lib.cmake)
add_library(vmlib ${WAMR_RUNTIME_LIB_SOURCE})
include (${SHARED_DIR}/utils/uncommon/shared_uncommon.cmake)
# /Add WAMR

# Check if python.wasm exists
if (NOT DEFINED PYTHON_BUILD_DIR)
    set(PYTHON_BUILD_DIR "${CMAKE_SOURCE_DIR}/build/cpython-wasm/Python-3.14.0/cross-build/wasm32-wasip1" CACHE PATH "Path to the Python build directory")
    message(WARNING "PYTHON_BUILD_DIR was not set. Defaulting to ${PYTHON_BUILD_DIR}")
endif()

if (NOT EXISTS "${PYTHON_BUILD_DIR}/python.wasm")
    message(FATAL_ERROR "${PYTHON_BUILD_DIR}/python.wasm does not exist. Run lingo-db/tools/scripts/build_cpython_wasm.sh before cmake configure.")
endif()

## Compile python.wasm to python.aot using wamrc
# Define the build directory for wamr-compiler
set(WAMR_COMPILER_DIR "${PROJECT_SOURCE_DIR}/vendored/wasm-micro-runtime/wamr-compiler")
set(WAMR_COMPILER_BUILD_DIR "${WAMR_COMPILER_DIR}/build")
set(WAMR_COMPILER_EXECUTABLE "${WAMR_COMPILER_BUILD_DIR}/wamrc")

# Add custom command to build wamr-compiler
add_custom_command(
        OUTPUT "${WAMR_COMPILER_EXECUTABLE}"
        COMMAND ${CMAKE_COMMAND} -S "${WAMR_COMPILER_DIR}" -B "${WAMR_COMPILER_BUILD_DIR}"
        COMMAND make -C "${WAMR_COMPILER_BUILD_DIR}" -j4
        COMMENT "Building wamr-compiler using cmake and make"
)

# Add custom command to generate python.aot from python.wasm
add_custom_command(
        OUTPUT "${PYTHON_BUILD_DIR}/python.aot"
        COMMAND ${WAMR_COMPILER_EXECUTABLE} -o "${PYTHON_BUILD_DIR}/python.aot" "${PYTHON_BUILD_DIR}/python.wasm"
        DEPENDS "${PYTHON_BUILD_DIR}/python.wasm" "${WAMR_COMPILER_EXECUTABLE}"
        COMMENT "Generating python.aot from python.wasm using wamrc"
)

# Our runtime code depends on reading this file, make sure it exists
add_custom_target(
        GeneratePythonAOT ALL
        DEPENDS "${PYTHON_BUILD_DIR}/python.aot"
)


add_subdirectory(include)
add_subdirectory(tools/build-tools)
gen_rt_def(dump-rt-defs "DumpRuntime.h")
gen_rt_def(string-rt-defs "StringRuntime.h")
gen_rt_def(date-rt-defs "DateRuntime.h")
gen_rt_def(decimal-rt-defs "DecimalRuntime.h")
gen_rt_def(integer-rt-defs "IntegerRuntime.h")
gen_rt_def(float-rt-defs "FloatRuntime.h")
gen_rt_def(udf-rt-defs "PythonRuntime.h")
gen_rt_def(timing-rt-defs "Timing.h")
gen_rt_def(tb-arrow-column-defs "ArrowColumn.h")
gen_rt_def(tb-arrow-table-defs "ArrowTable.h")
gen_rt_def(ds-it-rt-defs "DataSourceIteration.h")
gen_rt_def(join-ht-rt-defs "LazyJoinHashtable.h")
gen_rt_def(ht-rt-defs "Hashtable.h")
gen_rt_def(ec-rt-defs "ExecutionContext.h")
gen_rt_def(db-rt-defs "RelationHelper.h")
gen_rt_def(grow-buf-rt-defs "GrowingBuffer.h")
gen_rt_def(buf-rt-defs "Buffer.h")
gen_rt_def(stv-rt-defs "SegmentTreeView.h")
gen_rt_def(heap-rt-defs "Heap.h")
gen_rt_def(idx-rt-defs "LingoDBHashIndex.h")
gen_rt_def(hmm-rt-defs "HashMultiMap.h")
gen_rt_def(tls-rt-defs "ThreadLocal.h")
gen_rt_def(st-rt-defs "SimpleState.h")
gen_rt_def(paht-rt-defs "PreAggregationHashtable.h")
gen_rt_def(lock-rt-defs "EntryLock.h")
gen_rt_def(tracing-rt-defs "Tracing.h")
message("cpp files ${runtime_funcs_ptr_libs}")
string(REPLACE " " ";" runtime_funcs_ptr_libs "${runtime_funcs_ptr_libs}")

add_library(runtime_funcs_ptr ${runtime_funcs_ptr_libs})
add_dependencies(runtime_funcs_ptr build_includes)
add_subdirectory(src/utility)
add_subdirectory(src/scheduler)
add_subdirectory(src/compiler/mlir-support)

add_subdirectory(src/compiler/Dialect/RelAlg)
add_subdirectory(src/compiler/Dialect/TupleStream)
add_subdirectory(src/compiler/Dialect/SubOperator)

add_subdirectory(src/compiler/Dialect/DB)
add_subdirectory(src/compiler/Dialect/Arrow)
add_subdirectory(src/compiler/Dialect/util)

add_subdirectory(src/compiler/Conversion/DBToStd)
add_subdirectory(src/compiler/Conversion/ArrowToStd)
add_subdirectory(src/compiler/Conversion/RelAlgToSubOp)
add_subdirectory(src/compiler/Conversion/SubOpToControlFlow)

add_subdirectory(src/compiler/Conversion/UtilToLLVM)

get_property(dialect_libs GLOBAL PROPERTY MLIR_DIALECT_LIBS)
get_property(conversion_libs GLOBAL PROPERTY MLIR_CONVERSION_LIBS)
get_property(translation_libs GLOBAL PROPERTY MLIR_TRANSLATION_LIBS)
get_property(extension_libs GLOBAL PROPERTY MLIR_EXTENSION_LIBS)
set(LIBS
        ${dialect_libs}
        ${extension_libs}
        ${conversion_libs}
        ${translation_libs}
        MLIROptLib
        MLIRRelAlg
        MLIRTupleStream
        MLIRSubOperator
        MLIRUtil
        MLIRDB
        MLIRArrow
        MLIRIR
        MLIRParser
        MLIRPass
        MLIRSupport
        MLIRLLVMToLLVMIRTranslation
        MLIRTargetLLVMIRExport
        MLIRSupport
        MLIRAnalysis
        MLIRCallInterfaces
        MLIRSideEffectInterfaces
        MLIRTransforms
        MLIRMathTransforms
)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
set(PRECOMPILED_CC_PATH "${CMAKE_CURRENT_BINARY_DIR}/precompiled_bitcode.cc")
add_subdirectory(src/catalog)
add_subdirectory(src/runtime)
add_subdirectory(src/execution)
add_subdirectory(src/compiler/frontend)
add_subdirectory(src/tools)
add_subdirectory(tools)
if (ENABLE_TESTS)
    add_subdirectory(test)
endif ()
