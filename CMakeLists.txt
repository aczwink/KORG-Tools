cmake_minimum_required(VERSION 3.6)
project(KORG-Tools)
find_package(XP-CMake REQUIRED) #we need advanced cmake modules

set(CMAKE_CXX_STANDARD 20)

XPC_AutoBuildType()
XPC_OptForHost()
XPC_SetCompileDefinitions()

find_package(StdPlusPlus REQUIRED)

add_subdirectory(tools/korf)
add_subdirectory(tools/oc31)
add_subdirectory(tools/ospkg)
add_subdirectory(tools/resimp)
add_subdirectory(tools/setclean)
add_subdirectory(tools/setconv)
add_subdirectory(tools/setinfo)
add_subdirectory(tools/setexplorer)
add_subdirectory(tools/sndmap)
add_subdirectory(tools/styconv)
add_subdirectory(tools/wavextract)

add_subdirectory(include)
add_subdirectory(src)
add_library(libkorg SHARED ${SOURCE_FILES})
target_include_directories(libkorg PRIVATE "include")
target_link_libraries(libkorg Std++)

add_subdirectory(tests)