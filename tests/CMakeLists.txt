file(GLOB TESTS_SOURCES "*.c")

set(TESTS_TARGETS "")

foreach(source_file ${TESTS_SOURCES})
	get_filename_component(exec_name ${source_file} NAME_WE)
	add_executable(${exec_name} ${source_file})
	add_dependencies(${exec_name} CLOMY_H)
	list(APPEND TESTS_TARGETS ${exec_name})
endforeach()

add_custom_target(tests-all DEPENDS ${TESTS_TARGETS})
set(TESTS_TARGETS ${TESTS_TARGETS} PARENT_SCOPE)
