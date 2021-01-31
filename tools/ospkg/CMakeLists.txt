add_executable(ospkg main.cpp)
target_include_directories(ospkg PRIVATE "../../include")
target_link_libraries(ospkg LINK_PUBLIC Std++ Std++Static libkorg)