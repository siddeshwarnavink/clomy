file(GLOB EXAMPLES_SOURCES "*.c")

foreach(source_file ${EXAMPLES_SOURCES})
	get_filename_component(exec_name ${source_file} NAME_WE)
	add_executable(${exec_name} ${source_file})
	add_dependencies(${exec_name} CLOMY_H)
endforeach()
