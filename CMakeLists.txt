cmake_minimum_required(VERSION 3.1)
project (basilisk_hunter C CXX ASM)
add_subdirectory(cpu_features)
#todo: modularize the cmake file
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (VERSION_PATCH 0)

include_directories(include)
include_directories(src)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "src/*.c")

find_package(Git)
find_program(PYTHON_EXE NAMES python3)
set(VERSION_CPP_FILE "${PROJECT_BINARY_DIR}/src/version.cpp")
set(VERSION_GENERATOR_SCRIPT "${PROJECT_SOURCE_DIR}/src/generate_version.py")
set(VERSION_DEPENDS ${VERSION_GENERATOR_SCRIPT} ${SOURCES})
set(GIT_INDEX "${PROJECT_SOURCE_DIR}/.git/index")
if(EXISTS GIT_INDEX)
	set(VERSION_DEPENDS GIT_INDEX ${VERSION_DEPENDS})
endif()
add_custom_command(
	OUTPUT ${VERSION_CPP_FILE}
	COMMAND ${PYTHON_EXE}
	ARGS ${VERSION_GENERATOR_SCRIPT} ${VERSION_CPP_FILE} ${CMAKE_PROJECT_NAME} "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" "${GIT_EXECUTABLE}"
	DEPENDS ${VERSION_DEPENDS})
set(SOURCES ${VERSION_CPP_FILE} ${SOURCES})

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
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
		ARGS ${YASM_ARGS} -o ${YASM_OBJECT} ${YASM_SOURCE}
		MAIN_DEPENDENCY ${YASM_SOURCE} )
endforeach()
endif()

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "aarch64")
set(SOURCES "src/crypto/impl/bits_arm/sha256-armv8-aarch64.S" ${SOURCES})
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
set(CMAKE_CXX_STANDARD 11)

add_executable(basilisk_hunter ${SOURCES} ${YASM_OBJECTS})
target_link_libraries(basilisk_hunter Threads::Threads cpu_features ${CPR_LIBRARIES})
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Werror -flto")
set(CMAKE_EXE_LINKER_FLAGS "-flto")
