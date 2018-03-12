include(colviz_nett_schema_generator)

list(APPEND NETT_SCHEMA_FILES
  "${PROJECT_SOURCE_DIR}/content/schema/string_message.proto")

set(COLVIZ_GENERATED_SCHEMA_DIR "${PROJECT_BINARY_DIR}/schema/colviz/")
include_directories(BEFORE SYSTEM "${PROJECT_BINARY_DIR}/schema" )
colviz_add_event(COLVIZ "${NETT_SCHEMA_FILES}" ${COLVIZ_GENERATED_SCHEMA_DIR})

set(COLVIZ_PUBLIC_HEADERS
  ${COMMON_INCLUDES}
  ColViz.h
  DBManager.h
  helpers.hpp)

set(COLVIZ_HEADERS
  ${COLVIZ_PROTO_HEADERS})

set(COLVIZ_SOURCES
  ${COMMON_SOURCES}
  ${COLVIZ_PROTO_SOURCES}
  ColViz.cpp
  DBManager.cpp)

set(COLVIZ_LINK_LIBRARIES
  ${NETT_LIBRARIES}
  ${MYSQLCONNECTORCPP_LIBRARIES})

common_library(ColViz)
