include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/utils.cmake)

add_library(${PROJECT_NAME} STATIC)
add_subdirectory(include)
add_subdirectory(src)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_enable_cpplatest(${PROJECT_NAME})
target_treat_all_warnings_as_error(${PROJECT_NAME})

option(CORELIB_TEST_ENABLED OFF)
if (CORELIB_TEST_ENABLED)
	add_subdirectory(tests)
endif()
