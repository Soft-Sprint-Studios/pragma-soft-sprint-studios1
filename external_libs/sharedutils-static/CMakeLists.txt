include(${CMAKE_SOURCE_DIR}/cmake/pr_common.cmake)

set(PROJ_NAME sharedutils-static)
pr_add_library(${PROJ_NAME} STATIC)

# We have to link mathutil as an external library here to avoid a cyclic dependency
pr_add_external_dependency(${PROJ_NAME} mathutil HEADER_ONLY)
pr_add_module_reference(${PROJ_NAME} glm TARGET_NAME glm_module PRIVATE)

set(BASE_DIR "${CMAKE_CURRENT_LIST_DIR}/../sharedutils/")
pr_add_headers(${PROJ_NAME} "${CMAKE_CURRENT_LIST_DIR}/../sharedutils/include/" BASE_DIRS "${BASE_DIR}")
pr_add_sources(${PROJ_NAME} "${CMAKE_CURRENT_LIST_DIR}/../sharedutils/src/" BASE_DIRS "${BASE_DIR}")

pr_add_compile_definitions(${PROJ_NAME} -DSHUTIL_DLL)

pr_finalize(${PROJ_NAME})
