add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC arrow_json_converter arrow_shared)
target_link_libraries(catch_main PRIVATE project_options)
target_include_directories(catch_main PUBLIC ${CMAKE_SOURCE_DIR}/third_party)

add_executable(json_reader_test json_reader_test.cpp)
target_link_libraries(json_reader_test PRIVATE project_options catch_main)
target_include_directories(json_reader_test PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(json_writer_test json_writer_test.cpp)
target_link_libraries(json_writer_test PRIVATE project_options catch_main)
target_include_directories(json_writer_test PRIVATE ${CMAKE_SOURCE_DIR}/src)
