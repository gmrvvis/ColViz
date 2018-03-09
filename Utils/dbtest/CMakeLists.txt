cmake_minimum_required (VERSION 3.2)
project (Project_dbTest)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC ")
mark_as_advanced(CMAKE_BUILD_TYPE)

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/libs/ColVizUtils)
include_directories(${CMAKE_SOURCE_DIR}/libs/ColViz)

include_directories(${CMAKE_SOURCE_DIR}/nett/build)
include_directories(${CMAKE_SOURCE_DIR}/nett/build/nett/include)
include_directories(${CMAKE_SOURCE_DIR}/nett/build/nett/schema)

link_directories(${CMAKE_SOURCE_DIR}/nett/build)

include_directories(${CMAKE_SOURCE_DIR}/rapidjson/include)
link_directories(${CMAKE_SOURCE_DIR}/rapidjson/build)

#Apli
add_executable(dbTest main.cxx)

include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})

target_link_libraries (dbTest 	ColViz
								nett
								${PROTOBUF_LIBRARIES}
								zmq)

set_target_properties( dbTest
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
