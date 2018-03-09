cmake_minimum_required (VERSION 3.2)

project (ColVizUtils)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC ")
mark_as_advanced(CMAKE_BUILD_TYPE)

#Qt
find_package (Qt5Widgets) #Required only for QStrings

#Code and headers
file (GLOB UniViUtils_src "*.cpp")
file (GLOB UniViUtils_h "*.h")
							
include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR}
					${CMAKE_CURRENT_SOURCE_DIR}
					)
	
#Construccion de la librería
add_library (ColVizUtils SHARED 
    ${UniViUtils_src}
    ${UniViUtils_h}
)

target_link_libraries (ColVizUtils 	OpenMeshCore 
									OpenMeshTools 
									Qt5::Widgets
						)

set_target_properties( ColVizUtils
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
