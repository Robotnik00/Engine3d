
cmake_minimum_required(VERSION 2.6) 
project(Engine3d)


set(GLM_SRC_DIR CACHE PATH "library for matrix math")
set(SDL_SRC_DIR CACHE PATH "library for creating graphics context")
set(ASSIMP_SRC_DIR "not_found" CACHE PATH "library for importing 3d models")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY CACHE PATH "out dir")


add_subdirectory(src)
add_subdirectory(include)

include_directories(${Engine3d_SOURCE_DIR}/include; ${ASSIMP_SRC_DIR}/include; ${GLM_SRC_DIR}; ${SDL_SRC_DIR}/include include)
link_directories(${ASSIMP_SRC_DIR}/lib; ${SDL_SRC_DIR}/build)

add_executable(Engine3d ${sources} ${includes}) 

target_link_libraries(Engine3d  SDL2 assimp pthread GL GLU dl rt)

add_custom_command(TARGET Engine3d PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:Engine3d>)
