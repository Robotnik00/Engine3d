
cmake_minimum_required(VERSION 2.6) 
project(Engine3d)


set(GLM_SRC_DIR CACHE PATH "library for matrix math")
set(SDL_SRC_DIR CACHE PATH "library for creating graphics context")
set(ASSIMP_SRC_DIR "not_found" CACHE PATH "library for importing 3d models")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY CACHE PATH "out dir")

file(GLOB sourcecode ${Engine3d_SOURCE_DIR}/include/*.h ${Engine3d_SOURCE_DIR}/src/*.cpp ${Engine3d_SOURCE_DIR}/resources/shaders/*.vert ${Engine3d_SOURCE_DIR}/resources/shaders/*.frag)
include_directories(${Engine3d_SOURCE_DIR}/include; ${ASSIMP_SRC_DIR}/include; ${GLM_SRC_DIR}; ${SDL_SRC_DIR}/include include)
link_directories(${ASSIMP_SRC_DIR}/lib; ${SDL_SRC_DIR}/build)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" ) 


add_library(Engine3d SHARED ${sourcecode}) 

target_link_libraries(Engine3d  SDL2 assimp GL GLU)

add_subdirectory(example)
