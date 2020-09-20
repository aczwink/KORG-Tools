cmake_minimum_required(VERSION 3.6)
project(KORG-Tools)
find_package(XP-CMake REQUIRED) #we need advanced cmake modules

set(CMAKE_CXX_STANDARD 17)

XPC_AutoBuildType()
XPC_OptForHost()
XPC_SetCompileDefinitions()

find_package(StdPlusPlus REQUIRED)

set(LIBS Std++ Std++Static)

add_subdirectory(src_styconv)

add_executable(styconv ${SOURCE_FILES_STYCONV})
target_link_libraries(styconv ${LIBS})