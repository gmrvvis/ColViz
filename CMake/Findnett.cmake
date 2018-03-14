find_path(_nett_INCLUDE_DIR nett/nett.h
  HINTS ${NETT_ROOT}/include $ENV{NETT_ROOT}/include
  PATHS /usr/include /usr/local/include /opt/local/include)

find_library(_nett_LIBRARY NAMES nett
  HINTS ${NETT_ROOT}/lib $ENV{NETT_ROOT}/lib
  PATHS /usr/lib/nett /usr/local/lib/nett /opt/local/lib/nett)

find_path(_nett_CMAKE_HELPERS_DIR NAMES CMakeHelpers.cmake
  HINTS ${NETT_ROOT}/share/nett/CMake/ $ENV{NETT_ROOT}/share/nett/CMake
  PATHS /usr/lib/share/nett/CMake/ /usr/local/share/nett/CMake/ /opt/local/share/nett/CMake/
)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(nett
  "nett NOT FOUND. Please provide NETT_ROOT (via cmake or env. var)"
  _nett_INCLUDE_DIR _nett_LIBRARY _nett_CMAKE_HELPERS_DIR )

set(NETT_INCLUDE_DIRS ${_nett_INCLUDE_DIR})
set(NETT_LIBRARIES ${_nett_LIBRARY})
set(NETT_CMAKE_HELPERS_DIR ${_nett_CMAKE_HELPERS_DIR} )

if(NETT_FOUND AND NOT NETT_FIND_QUIETLY)
  message(STATUS "Found nett in ${NETT_INCLUDE_DIRS};${NETT_LIBRARIES}")
endif()
