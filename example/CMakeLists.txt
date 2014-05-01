cmake_minimum_required(VERSION 2.6)

project(example)

set(ENGINE_SRC_DIR ${example_SOURCE_DIR}/../)

message(${ENGINE_SRC_DIR})
include_directories(${ENGINE_SRC_DIR}/include /usr/local/include ${example_SOURCE_DIR}/.)

link_directories(${ENGINE_SRC_DIR}/build/)



add_executable(example example.cpp DebugState.h DebugState.cpp)

target_link_libraries(example Engine3d)

add_custom_command(TARGET example PRE_BUILD 
		   COMMAND ${CMAKE_COMMAND} -E copy_directory 
		   ${ENGINE_SRC_DIR}/resources $<TARGET_FILE_DIR:example>)

