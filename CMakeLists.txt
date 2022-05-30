cmake_minimum_required(VERSION 3.23.1) 

# set the project name 
project(main VERSION 1.0) 

#

# 指定可执行程序输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY 	${CMAKE_CURRENT_SOURCE_DIR}/out/test)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${CMAKE_CURRENT_SOURCE_DIR}/out/test/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 	${CMAKE_CURRENT_SOURCE_DIR}/out/test/Release)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/out/lib)
set(INCLUDE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/out/include)

add_subdirectory(3rd)
add_subdirectory(base)
add_subdirectory(tcp_base_on_asio)
#add_subdirectory(test/tcp_server)


execute_process(COMMAND pwd)