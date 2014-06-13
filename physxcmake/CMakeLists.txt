cmake_minimum_required(VERSION 2.6)
project(PHYSX)


FILE(GLOB_RECURSE header_files1 ${PHYSX_SOURCE_DIR}/Include/*.h)
SET(HEADER_DIRECTORIES1 "")
FOREACH(file_path ${header_files1})
  GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
  SET(HEADER_DIRECTORIES1 ${HEADER_DIRECTORIES1} ${dir_path})
ENDFOREACH()
LIST(REMOVE_DUPLICATES HEADER_DIRECTORIES1)

FILE(GLOB_RECURSE header_files2 ${PHYSX_SOURCE_DIR}/Source/*.h)
SET(HEADER_DIRECTORIES2 "")
FOREACH(file_path ${header_files2})
  GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
  SET(HEADER_DIRECTORIES2 ${HEADER_DIRECTORIES2} ${dir_path})
ENDFOREACH()
LIST(REMOVE_DUPLICATES HEADER_DIRECTORIES2)



include_directories(${HEADER_DIRECTORIES1} ${HEADER_DIRECTORIES2})

SET(HEADERS ${HEADER_DIRECTORIES1} ${HEADER_DIRECTORIES2})

#message("first list")

#message("${HEADER_DIRECTORIES1}")

#message("second list")

#message("${HEADER_DIRECTORIES2}")


file(GLOB_RECURSE sources ${PHYSX_SOURCE_DIR}/Source/*.cpp)

file(GLOB physx_libs
	${PHYSX_SOURCE_DIR}/Lib/linux64/libPhysX3.a  
	${PHYSX_SOURCE_DIR}/Lib/linux64/libSimulationController.a 
	${PHYSX_SOURCE_DIR}/Lib/linux64/libLowLevel.a
	${PHYSX_SOURCE_DIR}/Lib/linux64/libPhysXProfileSDK.a
	${PHYSX_SOURCE_DIR}/Lib/linux64/libPhysX3Extensions.a
	${PHYSX_SOURCE_DIR}/Lib/linux64/libSceneQuery.a
	${PHYSX_SOURCE_DIR}/Lib/linux64/libPhysX3Common.a
	${PHYSX_SOURCE_DIR}/Lib/linux64/libPhysX3CharacterKinematic.a 
	${PHYSX_SOURCE_DIR}/Lib/linux64/libPhysX3Vehicle.a
	${PHYSX_SOURCE_DIR}/Lib/linux64/libPhysX3Cooking.a
	${PHYSX_SOURCE_DIR}/Lib/linux64/libPvdRuntime.a
	${PHYSX_SOURCE_DIR}/Lib/linux64/libPxTask.a
	${PHYSX_SOURCE_DIR}/Lib/linux64/libLowLevelCloth.a 




















#	${PHYSX_SOURCE_DIR}/Lib/linux64/*.a
   )


#message("${physx_libs}")













#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -DNDEBUG" )




#add_library(physx SHARED ${sources} ${headers} )
 
