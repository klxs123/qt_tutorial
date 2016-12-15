project(net_protocol)
cmake_minimum_required(VERSION 2.8)
FILE(GLOB_RECURSE INC_ALL "./*.h")
add_custom_target(headers SOURCES ${INC_ALL})
add_definitions(-DPROTOCOL_DEBUG)

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} pthread)
