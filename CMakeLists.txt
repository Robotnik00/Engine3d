
cmake_minimum_required(VERSION 2.6) 
project(Engine3d)


set(GLM_SRC_DIR CACHE PATH "library for matrix math")
set(SDL_SRC_DIR CACHE PATH "library for creating graphics context")
set(ASSIMP_SRC_DIR "not_found" CACHE PATH "library for importing 3d models")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY CACHE PATH "out dir")


add_subdirectory(src)

