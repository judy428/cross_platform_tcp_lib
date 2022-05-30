cmake_minimum_required(VERSION 3.23.1) 

# set the project name 
project(tcpServer VERSION 1.0) 

# specify the C++ standard 
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED True) 

#添加cpp文件路径
aux_source_directory(./ src_all)

#add_executable(tcpServer calculate.cpp) 
add_library(tcpServer ${src_all})

set_target_properties(tcpServer PROPERTIES OUTPUT_NAME "tServer")
set_target_properties(tcpServer	PROPERTIES VERSION 1.0 SOVERSION 1)

target_include_directories(tcpServer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}) 

include_directories(${INCLUDE_OUTPUT_PATH})
link_directories(${LIBRARY_OUTPUT_PATH})



