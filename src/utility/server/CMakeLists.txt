
add_library(serverLib STATIC
        StatementHandler.cpp
        StatementExecution.cpp
        Statement.cpp ipc/IPCHelper.cpp shm/SyncHelper.cpp parser/ParaParser.cpp)

set_target_properties(serverLib PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)