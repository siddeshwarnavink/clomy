file(GLOB TESTS_SOURCES "*.c")

foreach(source_file ${TESTS_SOURCES})
	get_filename_component(exec_name ${source_file} NAME_WE)
	add_executable(${exec_name} ${source_file} ../clomy.h)
endforeach()
