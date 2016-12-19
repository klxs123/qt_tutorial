project(dataservice_server)
#设置输出目录
set(OUT_DIR /mnt/linux_lab/examples/qt_tutorial/bin/)
#头文件路径
include_directories(../net_protocol /usr/include/mysql)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
#设置库路径,链接自定义库文件dataservice_pub
link_directories(${OUT_DIR})
find_library(PUB_LIB libdataservice_pub.so HINS ${OUT_DIR})

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME}  ${PUB_LIB} pthread mysqlclient)
#mv目标文件到指定目录
execute_process(COMMAND mv ${CMAKE_BINARY_DIR}/${PROJECT_NAME} ${OUT_DIR}/server)
