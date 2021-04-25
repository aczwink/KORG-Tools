add_executable(korf main.cpp DumpChunks.cpp)
target_include_directories(korf PRIVATE "../../include")
target_link_libraries(korf LINK_PUBLIC Std++ Std++Static libkorg)