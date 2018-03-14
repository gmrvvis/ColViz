macro(colviz_add_event domain_name input_files output_dir)

  foreach(file ${input_files})
    get_filename_component(filepath ${file} DIRECTORY)
    get_filename_component(filename ${file} NAME_WE)

    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${output_dir})
    add_custom_command(
    COMMAND "${PROTOBUF_PROTOC_EXECUTABLE}"
    ARGS "${file}" "--proto_path=${filepath}" "--cpp_out=${output_dir}"
    COMMENT "Building Protobuf C++ headers for ${file} in ${output_dir}"
    DEPENDS "${file}"
    OUTPUT
    "${output_dir}/${filename}.pb.h" "${output_dir}/${filename}.pb.cc" )

    list(APPEND ${domain_name}_PROTO_HEADERS "${output_dir}/${filename}.pb.h")
    list(APPEND ${domain_name}_PROTO_SOURCES "${output_dir}/${filename}.pb.cc")

    set_source_files_properties(
      "${output_dir}/${filename}.pb.h"
      "${output_dir}/${filename}.pb.cc"
      PROPERTIES COMPILE_FLAGS -Wno-error=unused-parameter )


  endforeach()

  set(${domain_name}_PROTO_HEADERS ${${domain_name}_PROTO_HEADERS} PARENT_SCOPE)
  set(${domain_name}_PROTO_SOURCES ${${domain_name}_PROTO_SOURCES} PARENT_SCOPE)

endmacro()
