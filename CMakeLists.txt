cmake_minimum_required(VERSION 2.8)
project (basilisk_hunter)

include_directories(src)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(basilisk_hunter ${SOURCES})

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Werror -flto")
set(CMAKE_EXE_LINKER_FLAGS "-flto")
