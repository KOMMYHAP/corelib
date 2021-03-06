macro(target_enable_cpplatest project)
	if(MSVC)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
	else()
		target_compile_features(${project} PRIVATE cxx_std_17)
	endif(MSVC)
endmacro()

function(target_set_binary_output project dir)
	set_target_properties(${project} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${dir})
	set_target_properties(${project} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${dir})
	set_target_properties(${project} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${dir})
endfunction()

function(target_treat_all_warnings_as_error project)
	target_compile_definitions(${project} PRIVATE _CRT_SECURE_NO_WARNINGS)
	target_compile_options(${project} PRIVATE -W4 -WX /permissive-)
endfunction()
