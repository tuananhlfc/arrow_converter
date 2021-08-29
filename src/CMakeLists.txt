find_package(Arrow REQUIRED)

add_library(arrow_json_converter SHARED json_reader.cpp json_writer.cpp helper.cpp)
target_link_libraries(arrow_json_converter PRIVATE project_options arrow_shared)
target_include_directories(arrow_json_converter PRIVATE ${CMAKE_SOURCE_DIR}/third_party)

FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
#SET(HEADER_FILES type.hpp json_reader.hpp json_writer.hpp)
INSTALL(FILES ${HEADER_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/arrow_json_converter)
