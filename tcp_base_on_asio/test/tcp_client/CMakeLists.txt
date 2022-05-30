cmake_minimum_required(VERSION 3.23.1) 

# set the project name 
project(testClient VERSION 1.0) 

# specify the C++ standard 
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED True) 

#定义工程根目录; CMAKE_SOURCE_DIR为内建变量，表示工程根目录的CMakeLists.txt文件路径
#set(ROOT_DIR ${CMAKE_SOURCE_DIR})


#link_directories(${PROJECT_SOURCE_DIR}/lib)
# 指定引用的外部库的搜索路径
link_directories(${LIBRARY_OUTPUT_PATH})
include_directories(${INCLUDE_OUTPUT_PATH})
#link_directories("E:/code/c++/test/cross_platform_lib/tcp_base_on_asio/lib/Debug")

#添加cpp文件路径
aux_source_directory(./ src_all)

#add_executable(testClient calculate.cpp) 
add_executable(testClient ${src_all})

target_link_libraries(testClient base)
target_link_libraries(testClient tcpAsio)
IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    message("testClient SYSTEM is Linux")
	target_link_libraries(testClient pthread)
ENDIF()


