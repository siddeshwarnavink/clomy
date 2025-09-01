cmake_minimum_required(VERSION 3.5)
project(clomy)

find_package(Python3 REQUIRED)

set(HEADER_TEMPLATE ${CMAKE_SOURCE_DIR}/clomy.h.tmpl)
set(HEADER_GENERATED ${CMAKE_BINARY_DIR}/clomy.h)

add_custom_command(
	OUTPUT ${HEADER_GENERATED}
	COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/gen.py
		${HEADER_TEMPLATE}
		${HEADER_GENERATED}
	COMMAND ${CMAKE_COMMAND} -E copy ${HEADER_GENERATED} ${CMAKE_SOURCE_DIR}
	DEPENDS ${CMAKE_SOURCE_DIR}/gen.py ${HEADER_TEMPLATE}
	COMMENT "Generating clomy.h"
)

add_custom_target(CLOMY_H DEPENDS ${HEADER_GENERATED})
set(CLOMY_HEADER ${HEADER_GENERATED} CACHE INTERNAL "Path to clomy.h")

add_subdirectory(tests)
add_subdirectory(examples)

add_custom_target(run-tests
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/run_tests.cmake
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMENT "Running all tests"
	VERBATIM
)
add_dependencies(run-tests tests-all)
