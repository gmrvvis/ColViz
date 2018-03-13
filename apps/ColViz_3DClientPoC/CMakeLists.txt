include_directories(
  ${PROJECT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}/schema
  )

set(COLVIZ3DCLIENT_HEADERS
  libQGLPluginWidget.h
  manipulatedFrameSetConstraint.h
  object.h
  window.h)

set(COLVIZ3DCLIENT_SOURCES
  libQGLPluginWidget.cpp
  main.cpp
  manipulatedFrameSetConstraint.cpp
  object.cpp
  window.cpp)

set(COLVIZ3DCLIENT_LINK_LIBRARIES
  ColViz
  ColVizUtils
  Qt5::Core
  Qt5::Xml
  Qt5::OpenGL
  ${PROTOBUF_LIBRARIES}
  ${LIBZMQ_LIBRARY}
  ${QGLVIEWER_LIBRARY}
  ${OPENGL_gl_LIBRARY}
  ${OPENGL_glu_LIBRARY}
  ${GLUT_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT})

common_application(ColViz3DClient NOHELP)
