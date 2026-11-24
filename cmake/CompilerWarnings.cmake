function(set_project_warnings project_name)
    if(MSVC)
        target_compile_options(${project_name} PRIVATE
            /W4
            /permissive-
            /utf-8
        )
    else()
        target_compile_options(${project_name} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
        )
    endif()
endfunction()
