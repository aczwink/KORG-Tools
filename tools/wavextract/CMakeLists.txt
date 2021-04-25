add_executable(wavextract main.cpp)
target_include_directories(wavextract PRIVATE "../../include")
target_link_libraries(wavextract LINK_PUBLIC Std++ Std++Static libkorg)