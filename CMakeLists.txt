cmake_minimum_required(VERSION 2.6)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/project/cmake")

project(bezier)

string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE)

if(NOT CMAKE_BUILD_TYPE MATCHES "DEBUG")
	set(CMAKE_BUILD_TYPE "RELEASE")
else()
	add_definitions(-DDEBUG)
endif()

include(SublimeText)

find_package(Boost REQUIRED)
find_package(LibSDL2 REQUIRED)

add_definitions(${LIBSDL2_DEFINITIONS})
include_directories(${LIBSDL2_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
link_directories(${LIBSDL2_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS})

if(CMAKE_BUILD_TYPE MATCHES "DEBUG")
	link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG})
else()
	link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE})
endif()

set(COMMON_CXX_FLAGS "-std=c++11 -stdlib=libc++ -ferror-limit=0 -Wall -Werror -Wno-deprecated-implementations")
set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_CXX_FLAGS} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O3")

set(NR_BINARIES_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)

file(MAKE_DIRECTORY ${NR_BINARIES_OUTPUT_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${NR_BINARIES_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${NR_BINARIES_OUTPUT_DIR})

file(GLOB HDRS "src/**.h")
file(GLOB SRCS "src/**.cc")

message("-- SRCS: ${SRCS}")
message("-- HDRS: ${HDRS}")

add_executable(bezier ${SRCS} ${HDRS})
target_link_libraries(bezier sdl2)

###
### Generate Sublime Text project file
###

# get includes directories list
get_property(DIRS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
get_directory_property(DEFS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMPILE_DEFINITIONS)

# only transform COMMON_CXX_FLAGS into a list
string(REGEX MATCHALL "[^ ]+" FLAGS ${COMMON_CXX_FLAGS})

if(${CMAKE_GENERATOR} MATCHES "Unix Makefiles")
	generate_sublime_text_project("${FLAGS}" "${DEFS}" "${DIRS}")
endif()
