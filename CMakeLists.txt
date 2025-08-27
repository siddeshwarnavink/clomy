cmake_minimum_required(VERSION 2.4)
project(clomy)

find_package(Python3 REQUIRED)

add_custom_command(
	OUTPUT clomy.h
	COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/gen.py
	DEPENDS ${CMAKE_SOURCE_DIR}/gen.py
	COMMENT "Generating clomy.h"
)
add_custom_target(clomy_h DEPENDS clomy.h.tmpl)

add_subdirectory(tests)
add_subdirectory(examples)
