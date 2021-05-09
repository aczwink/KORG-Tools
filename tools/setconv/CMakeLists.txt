add_executable(setconv main.cpp SetConverter.hpp SetConverter.cpp SoundSelection.hpp SoundSelection.cpp)
target_include_directories(setconv PRIVATE "../../include")
target_link_libraries(setconv LINK_PUBLIC Std++ Std++Static libkorg)