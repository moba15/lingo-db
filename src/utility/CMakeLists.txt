add_library(utility Tracer.cpp Setting.cpp Serialization.cpp PythonUtility.cpp)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
target_link_libraries(utility PRIVATE Python3::Python)