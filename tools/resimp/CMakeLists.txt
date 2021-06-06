add_executable(resimp main.cpp ResourceImporter.hpp ResourceImporter.cpp)
target_include_directories(resimp PRIVATE "../../include")
target_link_libraries(resimp LINK_PUBLIC Std++ Std++Static libkorg)