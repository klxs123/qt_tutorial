project(dataservice_pub)
cmake_minimum_required(VERSION 2.8)
set(OUT_DIR /mnt/linux_lab/examples/qt_tutorial/bin/)
set(LIBRARY_OUTPUT_NAME lib${PROJECT_NAME}.so)
FILE(GLOB_RECURSE INC_ALL "./*.h")
include_directories(/usr/include/mysql)
add_custom_target(headers SOURCES ${INC_ALL} )
#模块调试的预编译开关
add_definitions(-DPROTOCOL_DEBUG)
add_definitions(-DDATASOURCE_DEBUG)

aux_source_directory(. SRC_LIST)
#设置输出目标文件为共享库文件(原来为可执行文件)
#add_executable(${PROJECT_NAME} ${SRC_LIST})
add_library(${PROJECT_NAME} SHARED  ${SRC_LIST})
target_link_libraries(${PROJECT_NAME})
##mv目标文件到指定目录
execute_process(COMMAND mv  ${CMAKE_BINARY_DIR}/${LIBRARY_OUTPUT_NAME} ${OUT_DIR}/libdataservice_pub.so)
