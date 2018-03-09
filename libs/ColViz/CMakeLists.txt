cmake_minimum_required (VERSION 3.5)
project (ColViz_middleware)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC ")

#Protobuf
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

#Force nett path location
include_directories(${CMAKE_SOURCE_DIR}/nett/include)
include_directories(${CMAKE_SOURCE_DIR}/nett/schema)

include_directories(${CMAKE_SOURCE_DIR}/nett/build)
include_directories(${CMAKE_SOURCE_DIR}/nett/build/nett/include)
include_directories(${CMAKE_SOURCE_DIR}/nett/build/nett/schema)

link_directories(${CMAKE_SOURCE_DIR}/nett/build)

include_directories(${CMAKE_SOURCE_DIR}/rapidjson/include)
link_directories(${CMAKE_SOURCE_DIR}/rapidjson/build)

#Code and headers
file (GLOB ColViz_src "*.cpp" "*.cc")
file (GLOB ColViz_h "*.h" "*.hpp")

#protobuf generated objects
file (GLOB Extra_src ${CMAKE_SOURCE_DIR}/nett/build/nett/schema/*.cc)
file (GLOB Extra_hdr ${CMAKE_SOURCE_DIR}/nett/build/nett/schema/*.h)

include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})

include_directories ("/usr/include/cppconn")

#Lobs srcs
add_library (ColViz SHARED 
    ${ColViz_src}
    ${ColViz_h}
    ${Extra_src}
    ${Extra_hdr}
)

#Libs
##g++ -Wall -I/usr/include/cppconn -o testapp test.cpp -L/usr/lib -lmysqlcppconn
target_link_libraries (ColViz  	nett
								${PROTOBUF_LIBRARIES}
								zmq
								mysqlcppconn)

set_target_properties( ColViz
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
