function(create_sample name sources)
    add_executable(${name})
    target_sources(${name} PRIVATE ${sources})
    target_link_libraries(${name}
        PUBLIC arrow_json_converter arrow_shared
    )
    target_include_directories(${name} PRIVATE ${CMAKE_SOURCE_DIR}/src)
endfunction()

create_sample(convert_json convert_json.cpp)
create_sample(convert_arrow convert_arrow.cpp)
