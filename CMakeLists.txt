
cmake_minimum_required(VERSION 2.6) 
project(Engine3d)

set(GLM_SRC_DIR CACHE PATH "library for matrix math")
set(SDL_SRC_DIR CACHE PATH "library for creating graphics context")
set(ASSIMP_SRC_DIR "not_found" CACHE PATH "library for importing 3d models")
set(PHYSX_SRC_DIR "not_found" CACHE PATH "physics api")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY CACHE PATH "out dir")

file(GLOB sourcecode ${Engine3d_SOURCE_DIR}/include/*.h
                     ${Engine3d_SOURCE_DIR}/src/*.cpp
                     ${Engine3d_SOURCE_DIR}/resources/shaders/*.vert
                     ${Engine3d_SOURCE_DIR}/resources/shaders/*.frag)

add_subdirectory(${PHYSX_SRC_DIR}/ . )


get_directory_property(physx_headers DIRECTORY ${PHYSX_SRC_DIR} DEFINITION HEADERS)

get_directory_property(physx_libraries DIRECTORY ${PHYSX_SRC_DIR} DEFINITION physx_libs)

message("headers")

message("${physx_headers}")

message("libs")

message("${physx_libraries}")

include_directories(${physx_headers}
                    ${Engine3d_SOURCE_DIR}/include
                    ${ASSIMP_SRC_DIR}/include
                    ${GLM_SRC_DIR}
                    ${SDL_SRC_DIR}/include
                    include)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" ) 

add_library(Engine3d SHARED ${sourcecode})



target_link_libraries(Engine3d SDL2 assimp GL GLU ${physx_libraries} pthread)

add_subdirectory(example)

