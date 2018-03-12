include_directories(
  ${PROJECT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}/schema)

set(COLVIZWEBCLIENT_SOURCES
  main.cpp
  webWidget.cpp
  window.cpp)

set(COLVIZWEBCLIENT_HEADERS
  webWidget.h
  window.h)

set(COLVIZWEBCLIENT_LINK_LIBRARIES
  ColViz
  ColVizUtils
  Qt5::Core
  Qt5::Widgets
  Qt5::WebEngine
  Qt5::WebEngineWidgets
  ${PROTOBUF_LIBRARIES}
  ${libzmq_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT})

common_application(ColVizWebClient NOHELP)
