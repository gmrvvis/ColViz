cmake_minimum_required (VERSION 3.2)
project (Project_ColVizWebClient)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

#Qt
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC ")
mark_as_advanced(CMAKE_BUILD_TYPE)

find_package (Threads)

#OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})

find_package(GLUT REQUIRED) 
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})

find_package (Qt5Widgets)
find_package (Qt5Core)
find_package (Qt5Gui)
find_package (Qt5WebEngine)
find_package (Qt5WebEngineWidgets)

link_directories(${CMAKE_SOURCE_DIR}/nett/build)

#Apli srcs
file (GLOB MyProjectLib_src "*.cpp")
file (GLOB MyProjectLib_hdr "*.h" "*.hpp")

set (MyProjectLib_ui  ${PROJECT_SOURCE_DIR}/window.ui)

#Apli
set (MyProjectBin_src ${PROJECT_SOURCE_DIR}/main.cxx)

#Protobuf
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/libs/ColVizUtils)
include_directories(${CMAKE_SOURCE_DIR}/libs/ColViz)

include_directories(${CMAKE_SOURCE_DIR}/nett/include)
include_directories(${CMAKE_SOURCE_DIR}/nett/schema)
include_directories(${CMAKE_SOURCE_DIR}/nett/build)
include_directories(${CMAKE_SOURCE_DIR}/nett/build/nett/include)
include_directories(${CMAKE_SOURCE_DIR}/nett/build/nett/schema)

link_directories(${CMAKE_SOURCE_DIR}/nett/build)

include_directories(${CMAKE_SOURCE_DIR}/rapidjson/include)
link_directories(${CMAKE_SOURCE_DIR}/rapidjson/build)

include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})

include_directories(${QT_INCLUDES}  ${CMAKE_CURRENT_BINARY_DIR}
                                    ${CMAKE_CURRENT_SOURCE_DIR}
                                    )   

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

#MOC invocation
qt5_wrap_cpp(MyProjectLib_hdr_moc ${MyProjectLib_hdr})
qt5_wrap_ui(MyProjectLib_ui_moc  ${MyProjectLib_ui})

include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})


#Lib
add_library (ColVizWebClientLib SHARED 
    ${MyProjectLib_src}
    ${MyProjectLib_hdr_moc}
    ${MyProjectLib_ui_moc}
)

target_link_libraries ( ColVizWebClientLib	ColViz
											nett
											${PROTOBUF_LIBRARIES}
											zmq
											ColVizUtils
											Qt5::Widgets
											Qt5::WebEngineWidgets
											${OPENGL_LIBRARIES}
											${GLUT_LIBRARY}
											${CMAKE_THREAD_LIBS_INIT}
								)

#bins
set(CMAKE_AUTORCC ON)
add_executable(ColVizWebClient ${MyProjectBin_src} jquery.qrc)
target_link_libraries (ColVizWebClient ColVizWebClientLib)

set_target_properties( ColVizWebClientLib
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set_target_properties( ColVizWebClient
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
