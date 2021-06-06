add_executable(setclean main.cpp SetCleaner.hpp SetCleaner.cpp)
target_include_directories(setclean PRIVATE "../../include")
target_link_libraries(setclean LINK_PUBLIC Std++ Std++Static libkorg)