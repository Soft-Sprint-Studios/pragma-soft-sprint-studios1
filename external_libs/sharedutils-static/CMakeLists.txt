include(${CMAKE_SOURCE_DIR}/cmake/pr_common.cmake)

set(PROJ_NAME sharedutils-static)
pr_add_library(${PROJ_NAME} STATIC)

pr_add_dependency(${PROJ_NAME} glm MODULE)
pr_add_dependency(${PROJ_NAME} mathutil LIBRARY)

set(BASE_DIR "${CMAKE_CURRENT_LIST_DIR}/../sharedutils/")
pr_add_headers(${PROJ_NAME} "${CMAKE_CURRENT_LIST_DIR}/../sharedutils/include/" BASE_DIRS "${BASE_DIR}")
pr_add_sources(${PROJ_NAME} "${CMAKE_CURRENT_LIST_DIR}/../sharedutils/src/" BASE_DIRS "${BASE_DIR}")

pr_add_compile_definitions(
	${PROJ_NAME}
		-DSHUTIL_DLL
		-DGLM_FORCE_DEPTH_ZERO_TO_ONE
		-URI_STATIC_BUILD
)

pr_finalize(${PROJ_NAME})
