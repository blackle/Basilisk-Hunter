cmake_minimum_required(VERSION 2.8)
project (basilisk_hunter)

include_directories(src)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "src/*.c")

find_program(YASM_EXE NAMES yasm)
set(YASM_OBJECTS)
file(GLOB_RECURSE YASM_SOURCES "src/*.yasm")
foreach(YASM_SOURCE ${YASM_SOURCES})
	get_filename_component(YASM_SOURCE_NAME ${YASM_SOURCE} NAME)
	set(YASM_OBJECT ${CMAKE_CURRENT_BINARY_DIR}/${YASM_SOURCE_NAME}.o)
	set(YASM_OBJECTS ${YASM_OBJECTS} ${YASM_OBJECT})
	add_custom_command(
		OUTPUT ${YASM_OBJECT}
		COMMAND ${YASM_EXE}
		ARGS -f x64 -f elf64 -X gnu -g dwarf2 -D LINUX -o ${YASM_OBJECT} ${YASM_SOURCE} )
endforeach()

add_executable(basilisk_hunter ${SOURCES} ${YASM_OBJECTS})

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Werror -flto")
set(CMAKE_EXE_LINKER_FLAGS "-flto")
