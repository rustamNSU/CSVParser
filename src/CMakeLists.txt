cmake_minimum_required (VERSION 3.8)

add_library(MyCSVParser INTERFACE)

target_include_directories(MyCSVParser 
       INTERFACE
       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
       $<INSTALL_INTERFACE:include>)

file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

install(TARGETS MyCSVParser EXPORT MyCSVParserConfig DESTINATION lib)

install(FILES ${HEADERS} DESTINATION include)