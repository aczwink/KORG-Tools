add_executable(styconv main.cpp)
target_include_directories(styconv PRIVATE "../../include")
target_link_libraries(styconv LINK_PUBLIC Std++ Std++Static libkorg)