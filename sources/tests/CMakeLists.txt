project(corelib_test)

add_subdirectory(gtest)

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE
	gtest
	gtest_main
	corelib
)
target_enable_cpplatest(${PROJECT_NAME})
target_treat_all_warnings_as_error(${PROJECT_NAME})
target_precompile_headers(${PROJECT_NAME} REUSE_FROM corelib)

add_subdirectory(src)
add_subdirectory(include)
