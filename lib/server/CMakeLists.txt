add_subdirectory(shm)
add_subdirectory(ipc)
add_subdirectory(pararef-finder)
add_library(serverLib OBJECT
        StatementHandler.cpp
        StatementExecution.cpp
        Statement.cpp)
target_link_libraries(serverLib PRIVATE syncHelper PRIVATE ipcHelper)
set_target_properties(serverLib PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)