cmake_minimum_required (VERSION 3.2)
project (ColViz VERSION 0.0.1)
set( ColViz_VERSION_ABI 1 )

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

# Disable in source building
if( "${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}" )
 message( FATAL "no in source building allowed." )
endif( )

list( APPEND CMAKE_MODULE_PATH
 ${PROJECT_SOURCE_DIR}/CMake
 ${CMAKE_SOURCE_DIR}/CMake/common )

if( NOT EXISTS ${CMAKE_SOURCE_DIR}/CMake/common/Common.cmake )
 message( FATAL_ERROR "CMake/common missing, run: git submodule update --init")
endif( )

if( NOT EXISTS ${CMAKE_SOURCE_DIR}/deps/rapidjson/include/rapidjson/rapidjson.h )
 message( FATAL_ERROR "rapidjson missing, run: git submodule update --init")
endif( )
include_directories( ${CMAKE_SOURCE_DIR}/deps/rapidjson/include/ )

include( GitExternal )

set( COLVIZ_DESCRIPTION "ColViz - Collaborative Visualization Middleware" )
set( COLVIZ_LICENSE LGPL )
set( COLVIZ_MAINTAINER "Juan Pedro Brito <juanpedro.brito@upm.es>" )
set( COMMON_PROJECT_DOMAIN es.gmrv )


include( Common )
common_find_package(OpenMesh SYSTEM REQUIRED)
common_find_package(MySQLConnectorCPP SYSTEM REQUIRED)
common_find_package(nett SYSTEM REQUIRED)
common_find_package(libzmq SYSTEM REQUIRED)
common_find_package(Protobuf SYSTEM REQUIRED)
common_find_package(Qt5Widgets SYSTEM REQUIRED)
common_find_package(Qt5Core SYSTEM REQUIRED)
common_find_package(Qt5Gui SYSTEM REQUIRED)
common_find_package(Qt5WebEngine SYSTEM REQUIRED)
common_find_package(Qt5WebEngineWidgets SYSTEM REQUIRED)
common_find_package(Threads SYSTEM REQUIRED)
common_find_package(QGLViewer SYSTEM REQUIRED)
common_find_package(OpenGL SYSTEM REQUIRED)
common_find_package(GLUT SYSTEM REQUIRED)

common_find_package_post( )

add_subdirectory( colviz )
add_subdirectory( colviz/utils )
add_subdirectory( utils/dbtest )
add_subdirectory( content )
add_subdirectory( apps/ColViz_WebClientPoC )
add_subdirectory( apps/ColViz_3DClientPoC )

set(DOCS README.md LICENSE.txt)
install(FILES ${DOCS} DESTINATION share/ColViz COMPONENT dev)

set(DOXYGEN_MAINPAGE_MD README.md)
set(DOXYGEN_EXTRA_INPUT ${PROJECT_SOURCE_DIR}/README.md)
include(DoxygenRule)

include(CPackConfig)
include(CTest)
