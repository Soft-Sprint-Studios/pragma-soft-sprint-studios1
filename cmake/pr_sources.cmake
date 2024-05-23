function(pr_add_header_list TARGET_NAME HEADER_LIST)
	set(options)
	set(oneValueArgs TARGET_NAME HEADER_LIST)
	set(multiValueArgs BASE_DIRS)
	cmake_parse_arguments(PARSE_ARGV 0 PA "${options}" "${oneValueArgs}" "${multiValueArgs}")
	if(NOT DEFINED PA_BASE_DIRS)
		set(PA_BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}")
	endif()

	foreach(source IN LISTS HEADER_LIST)
		get_filename_component(source_path "${source}" PATH)
		string(REPLACE "${CMAKE_CURRENT_LIST_DIR}" "" source_path_relative "${source_path}")
		string(REPLACE "/" "\\" source_path_msvc "${source_path_relative}")
		source_group("${source_path_msvc}" FILES "${source}")
	endforeach()

	target_sources(${TARGET_NAME}
		PUBLIC
			FILE_SET headers TYPE HEADERS BASE_DIRS ${PA_BASE_DIRS} FILES ${HEADER_LIST}
	)
endfunction()

function(pr_add_headers TARGET_NAME HEADER_LOCATION)
	message("[PR] Adding include directory \"${HEADER_LOCATION}\" to target ${TARGET_NAME}")
	target_include_directories(${TARGET_NAME} PRIVATE "${HEADER_LOCATION}")

	set(options)
	set(oneValueArgs TARGET_NAME HEADER_LOCATION)
	set(multiValueArgs BASE_DIRS)
	cmake_parse_arguments(PARSE_ARGV 0 PA "${options}" "${oneValueArgs}" "${multiValueArgs}")
	if(NOT DEFINED PA_BASE_DIRS)
		set(PA_BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}")
	endif()

	file(GLOB_RECURSE HEADER_LIST "${HEADER_LOCATION}/*.h" "${HEADER_LOCATION}/*.hpp")
	pr_add_header_list(${TARGET_NAME} "${HEADER_LIST}" "${BASE_DIRS}" BASE_DIRS "${PA_BASE_DIRS}")
endfunction()

function(pr_add_module_list TARGET_NAME MODULE_LIST)
	foreach(source IN LISTS MODULE_LIST)
		get_filename_component(source_path "${source}" PATH)
		string(REPLACE "${CMAKE_CURRENT_LIST_DIR}" "" source_path_relative "${source_path}")
		string(REPLACE "/" "\\" source_path_msvc "${source_path_relative}")
		source_group("${source_path_msvc}" FILES "${source}")
	endforeach()

	target_sources(${TARGET_NAME}
		PUBLIC
			FILE_SET cxx_modules TYPE CXX_MODULES FILES ${MODULE_LIST}
	)
endfunction()

function(pr_add_modules TARGET_NAME MODULE_LOCATION)
	file(GLOB_RECURSE MODULE_LIST "${MODULE_LOCATION}/*.cppm")
	pr_add_module_list(${TARGET_NAME} "${MODULE_LIST}")
endfunction()

function(pr_add_source_list TARGET_NAME SOURCE_LIST)
	foreach(source IN LISTS SOURCE_LIST)
		get_filename_component(source_path "${source}" PATH)
		string(REPLACE "${CMAKE_CURRENT_LIST_DIR}" "" source_path_relative "${source_path}")
		string(REPLACE "/" "\\" source_path_msvc "${source_path_relative}")
		source_group("${source_path_msvc}" FILES "${source}")
	endforeach()

	target_sources(${TARGET_NAME}
		PRIVATE
			${SOURCE_LIST}
	)
endfunction()

function(pr_add_sources TARGET_NAME SOURCE_LOCATION)
	message("[PR] Adding include directory \"${SOURCE_LOCATION}\" to target ${TARGET_NAME}")
	target_include_directories(${TARGET_NAME} PRIVATE "${SOURCE_LOCATION}")

	file(GLOB_RECURSE SOURCE_LIST "${SOURCE_LOCATION}/*.cpp")
	pr_add_source_list(${TARGET_NAME} "${SOURCE_LIST}")
endfunction()

function(pr_add_compile_definitions TARGET_NAME)
    message("[PR] Adding compile definitions ${ARGN} to target ${TARGET_NAME}")
    target_compile_definitions(${TARGET_NAME} PUBLIC ${ARGN})
endfunction()
