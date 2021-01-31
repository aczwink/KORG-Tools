cmake_minimum_required(VERSION 3.6)
project(KORG-Tools)
find_package(XP-CMake REQUIRED) #we need advanced cmake modules

set(CMAKE_CXX_STANDARD 17)

XPC_AutoBuildType()
XPC_OptForHost()
XPC_SetCompileDefinitions()

find_package(StdPlusPlus REQUIRED)

add_subdirectory(tools/ospkg)

add_subdirectory(include)
add_subdirectory(src)
add_library(libkorg SHARED ${SOURCE_FILES})
target_include_directories(libkorg PRIVATE "include")
target_link_libraries(libkorg Std++)



add_subdirectory(src_setinfo)
add_executable(setinfo ${SOURCE_FILES_SETINFO})
target_include_directories(setinfo PRIVATE "include")
target_link_libraries(setinfo LINK_PUBLIC Std++ Std++Static libkorg)


add_subdirectory(src_styconv)
add_executable(styconv ${SOURCE_FILES_STYCONV})
target_include_directories(styconv PRIVATE "include")
target_link_libraries(styconv LINK_PUBLIC Std++ Std++Static libkorg)