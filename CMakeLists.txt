cmake_minimum_required(VERSION 2.8)
project (basilisk_hunter)
add_subdirectory(cpu_features)

include_directories(src)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "src/*.c")

find_program(YASM_EXE NAMES yasm)
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
	set(YASM_ARGS "-f" "x64" "-f" "elf64" "-X" "gnu" "-g" "dwarf2" "-D" "LINUX")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(YASM_ARGS "-f" "x64" "-f" "macho64" "-D" "MANGLE" "-D" "LINUX")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(YASM_ARGS "-Xvc" "-f" "x64" "-rnasm" "-pnasm" "-g" "cv8")
else()
	message( FATAL_ERROR "TODO: don't know how to compile yasm on ${CMAKE_SYSTEM_NAME}." )
endif()

set(YASM_OBJECTS)
file(GLOB_RECURSE YASM_SOURCES "src/*.yasm")
foreach(YASM_SOURCE ${YASM_SOURCES})
	get_filename_component(YASM_SOURCE_NAME ${YASM_SOURCE} NAME)
	set(YASM_OBJECT ${CMAKE_CURRENT_BINARY_DIR}/${YASM_SOURCE_NAME}.o)
	set(YASM_OBJECTS ${YASM_OBJECTS} ${YASM_OBJECT})
	add_custom_command(
		OUTPUT ${YASM_OBJECT}
		COMMAND ${YASM_EXE}
		ARGS ${YASM_ARGS} -o ${YASM_OBJECT} ${YASM_SOURCE} )
endforeach()

add_executable(basilisk_hunter ${SOURCES} ${YASM_OBJECTS})
target_link_libraries(basilisk_hunter cpu_features)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Werror -flto")
set(CMAKE_EXE_LINKER_FLAGS "-flto")
