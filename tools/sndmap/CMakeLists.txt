add_executable(sndmap main.cpp)
target_include_directories(sndmap PRIVATE "../../include")
target_link_libraries(sndmap LINK_PUBLIC Std++ Std++Static libkorg)