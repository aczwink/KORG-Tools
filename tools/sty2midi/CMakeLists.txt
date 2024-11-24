add_executable(sty2midi main.cpp)
target_include_directories(sty2midi PRIVATE "../../include")
target_link_libraries(sty2midi LINK_PUBLIC Std++ Std++Static libkorg)