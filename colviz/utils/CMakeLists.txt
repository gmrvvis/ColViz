set(COLVIZUTILS_SOURCES
  BaseMeshContainer.cpp
  BaseMesh.cpp
  BaseMeshRenderer.cpp
  defs.cpp
  Distribution.cpp
  IndexedBaseMeshContainer.cpp
  SWCImporter.cpp )

set(COLVIZUTILS_PUBLIC_HEADERS
  BaseMeshContainer.h
  BaseMesh.h
  BaseMeshRenderer.h
  defs.h
  Distribution.h
  IndexedBaseMeshContainer.h
  log.hpp
  MeshDef.hpp
  SWCImporter.h )

set(COLVIZUTILS_LINK_LIBRARIES
  ${OPENMESH_LIBRARIES}
	Qt5::Widgets )

set(COLVIZUTILS_INCLUDE_NAME colviz/utils)
set(COLVIZUTILS_NAMESPACE colvizutils)
add_definitions(-DCOLVIZ_SHARED)

common_library(ColVizUtils)
