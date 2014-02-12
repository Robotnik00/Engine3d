
include_directories(${Engine3d_SOURCE_DIR}/include; ${ASSIMP_SRC_DIR}/include; ${GLM_SRC_DIR}; ${SDL_SRC_DIR}/include)

link_directories(${ASSIMP_SRC_DIR}/lib; ${SDL_SRC_DIR}/build)

add_executable(Engine3d main.cpp Model.cpp Shaders.cpp Engine.cpp States.cpp) 


target_link_libraries(Engine3d  SDL2 assimp pthread GL GLU dl rt)
