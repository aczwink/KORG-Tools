add_executable(oc31 main.cpp)
target_include_directories(oc31 PRIVATE "../../include")
target_link_libraries(oc31 LINK_PUBLIC Std++ Std++Static libkorg)