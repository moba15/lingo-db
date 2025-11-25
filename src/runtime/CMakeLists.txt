add_library(runtime OBJECT
        helpers.cpp
        Tracing.cpp
        ArrowColumn.cpp
        ArrowTable.cpp
        ArrowView.cpp
        ThreadLocal.cpp
        Heap.cpp
        DumpRuntime.cpp
        DataSourceIteration.cpp
        Sorting.cpp
        GrowingBuffer.cpp
        Buffer.cpp
        SimpleState.cpp
        LazyJoinHashtable.cpp
        SegmentTreeView.cpp
        Hashtable.cpp
        PreAggregationHashtable.cpp
        HashMultiMap.cpp
        StringRuntime.cpp
        FloatRuntime.cpp
        DecimalRuntime.cpp
        IntegerRuntime.cpp
        Hash.cpp
        Timing.cpp
        DateRuntime.cpp
        ExecutionContext.cpp
        RelationHelper.cpp
        EntryLock.cpp
        PythonRuntime.cpp
        #Database.cpp
        #ArrowDirDatabase.cpp
        #ExternalArrowDatabase.cpp
        #MetaDataOnlyDatabase.cpp
        #ExternalHashIndex.cpp
        LingoDBHashIndex.cpp
        Session.cpp
        storage/LingoDBTable.cpp


        WASM.cpp

        ${UNCOMMON_SHARED_SOURCE}
)

set(COMPILE_DEFS "")
if (ENABLE_GPU_BACKEND)
    list(APPEND COMPILE_DEFS "GPU_ENABLED=1")
else ()
    list(APPEND COMPILE_DEFS "GPU_ENABLED=0")
endif (ENABLE_GPU_BACKEND)
list(APPEND COMPILE_DEFS "SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\"")
target_compile_definitions(runtime PUBLIC ${COMPILE_DEFS})
target_link_libraries(runtime PRIVATE Arrow::arrow_static)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
target_link_libraries(runtime PRIVATE Python3::Python)
add_subdirectory(GPU)
target_link_libraries(runtime PUBLIC GPU-rt scheduler)
target_link_libraries(runtime PUBLIC vmlib -lm)


add_dependencies(runtime GeneratePythonAOT)


