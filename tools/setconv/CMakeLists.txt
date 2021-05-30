add_executable(setconv main.cpp SetConverter.hpp SetConverter.cpp ResourceSelector.hpp ResourceSelector.cpp BankAllocator.hpp BankAllocator.cpp)
target_include_directories(setconv PRIVATE "../../include")
target_link_libraries(setconv LINK_PUBLIC Std++ Std++Static libkorg)