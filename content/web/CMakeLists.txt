set(COLVIZ_CONTENT_WEB_FILES
  SimpleWebViz/d3/d3.min.js
  SimpleWebViz/d3/d3.js
  SimpleWebViz/d3/d3.v3.js.txt
  SimpleWebViz/index.html
  SimpleWebViz/style.css
  WebGL/index.html
  WebGL/OBJLoader.js
  WebGL/Color.js
  WebGL/three.js
  WebGL/content/textures/UV_Grid_Sm.jpg
  WebGL/content/meshes/Piram_000.obj
  WebGL/content/meshes/male02.mtl
  WebGL/content/meshes/male02.obj
  WebGL/OrbitControls.js
  )

foreach( FILE ${COLVIZ_CONTENT_WEB_FILES} )
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${FILE}
    ${CMAKE_CURRENT_BINARY_DIR}/${FILE}
    COPYONLY )

  get_filename_component(FILE_PATH ${FILE} DIRECTORY)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${FILE}
    DESTINATION content/${FILE_PATH} COMPONENT lib)

endforeach( )
