cmake_minimum_required(VERSION 3.5)

if(WIN32)
	set(POSSIBLE_DIRS
		"${CMAKE_BINARY_DIR}/tests/Debug"
		"${CMAKE_BINARY_DIR}/tests/Release"
		"${CMAKE_BINARY_DIR}/tests/RelWithDebInfo"
		"${CMAKE_BINARY_DIR}/tests/MinSizeRel"
		"${CMAKE_BINARY_DIR}/tests"
	)
	set(EXECUTABLE_EXTENSION ".exe")
else()
	set(POSSIBLE_DIRS "${CMAKE_BINARY_DIR}/tests")
	set(EXECUTABLE_EXTENSION "")
endif()

set(TESTS_DIR "")
foreach(dir ${POSSIBLE_DIRS})
	if(EXISTS "${dir}")
		set(TESTS_DIR "${dir}")
		break()
	endif()
endforeach()

if(NOT TESTS_DIR)
	message(FATAL_ERROR "Could not find tests directory.")
endif()

message(STATUS "Looking for executables in: ${TESTS_DIR}")

file(GLOB EXECUTABLES "${TESTS_DIR}/*${EXECUTABLE_EXTENSION}")

set(VALID_EXECUTABLES "")
foreach(exec ${EXECUTABLES})
	get_filename_component(exec_name "${exec}" NAME)
	get_filename_component(exec_ext "${exec}" EXT)
	if(WIN32)
		if(exec_ext STREQUAL ".exe")
			list(APPEND VALID_EXECUTABLES "${exec}")
		endif()
	else()
		if(NOT exec_name MATCHES "\\.(so|a|dylib|dll)$" AND
				NOT exec_name MATCHES "\\.(cmake|txt|log)$" AND
				NOT exec_name MATCHES "^(Makefile|makefile|CMakeCache\\.txt|
				cmake_install\\.cmake)$")
				if(EXISTS "${exec}")
				if(NOT IS_DIRECTORY "${exec}")
					list(APPEND VALID_EXECUTABLES "${exec}")
				endif()
			endif()
		endif()
	endif()
endforeach()

if(NOT VALID_EXECUTABLES)
	message(WARNING "No executables found in ${TESTS_DIR}")
	return()
endif()

set(PASSED_TESTS, 0)
set(FAILED_TESTS, 0)

foreach(executable ${VALID_EXECUTABLES})
	get_filename_component(exec_name "${executable}" NAME)
	message(STATUS "")
	message(STATUS "========================================")
	message(STATUS "Running: ${exec_name}")
	message(STATUS "========================================")

	execute_process(
		COMMAND "${executable}"
		WORKING_DIRECTORY "${TESTS_DIR}"
		RESULT_VARIABLE result
		OUTPUT_VARIABLE output
		ERROR_VARIABLE error
	)

	if(output)
		message(STATUS "Output:\n${output}")
		math(EXPR PASSED_TESTS "${PASSED_TESTS} + 1")
	endif()

	if(error)
		message(STATUS "Error output:\n${error}")
		math(EXPR FAILED_TESTS "${FAILED_TESTS} + 1")
	endif()

	if(NOT result EQUAL 0)
		message(WARNING "${exec_name} exited with code: ${result}")
	else()
		message(STATUS "${exec_name} completed successfully")
	endif()
endforeach()

message(STATUS "")
message(STATUS "========================================")
message(STATUS "Finished running all test")
message(STATUS "${PASSED_TESTS} passed. ${FAILED_TESTS} failed.")
message(STATUS "========================================")
