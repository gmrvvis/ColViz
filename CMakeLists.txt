cmake_minimum_required (VERSION 3.2)
project (ColVizMiddelware VERSION 0.0.1)
set( ColVizMiddelware_VERSION_ABI 1 )

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

#To avoid conflicts
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC ")
mark_as_advanced(CMAKE_BUILD_TYPE)

## Disable in source building
#if( "${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}" )
#  message( FATAL "no in source building allowed." )
#endif( )
#
#list( APPEND CMAKE_MODULE_PATH
#  ${PROJECT_SOURCE_DIR}/CMake
#  ${CMAKE_SOURCE_DIR}/CMake/common )
#
#if( NOT EXISTS ${CMAKE_SOURCE_DIR}/CMake/common/Common.cmake )
#  message( FATAL_ERROR "CMake/common missing, run: git submodule update --init")
#endif( )
#
#include( GitExternal )
#set( COMMON_PROJECT_DOMAIN gmrv.es )
#include( Common )
#
#common_find_package( nett REQUIRED )
#common_find_package( rapidjson REQUIRED )
#
#list( APPEND OpenGL GLEW GLUT nett rapidjson )
#common_find_package_post( )
#set( PROJECT_INCLUDE_NAME ColViz )

#Build libraries aux libs
add_subdirectory( libs/ColVizUtils )
add_subdirectory( libs/ColViz )

add_subdirectory( Utils/dbtest )
add_subdirectory( apps/ColViz_WebClientPoC )
add_subdirectory( apps/ColViz_3DClientPoC )
