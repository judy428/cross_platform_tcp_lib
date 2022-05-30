cmake_minimum_required(VERSION 3.23.1) 

# set the project name 
project(tcpClient VERSION 1.0) 

# specify the C++ standard 
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED True) 

#添加cpp文件路径
aux_source_directory(./ src_all)

#add_executable(tcpClient calculate.cpp) 
add_library(tcpClient ${src_all})

set_target_properties(tcpClient PROPERTIES OUTPUT_NAME "tClient")
set_target_properties(tcpClient	PROPERTIES VERSION 1.0 SOVERSION 1)

target_include_directories(tcpClient INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}) 

include_directories(${INCLUDE_OUTPUT_PATH})
link_directories(${LIBRARY_OUTPUT_PATH})


