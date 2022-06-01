cmake_minimum_required(VERSION 3.23.1) 

# set the project name 
project(testServer VERSION 1.0) 

# specify the C++ standard 
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED True) 

#定义工程根目录; CMAKE_SOURCE_DIR为内建变量，表示工程根目录的CMakeLists.txt文件路径
set(ROOT_DIR ${CMAKE_SOURCE_DIR})

include_directories(${INCLUDE_OUTPUT_PATH})

#link_directories(${PROJECT_SOURCE_DIR}/lib)
# 指定引用的外部库的搜索路径
#LINK_DIRECTORIES(${ROOT_DIR}/out/lib)
link_directories(${LIBRARY_OUTPUT_PATH})

#添加cpp文件路径
aux_source_directory(./ src_all)

#add_executable(testServer calculate.cpp) 
add_executable(testServer ${src_all})

#target_link_libraries(testServer boost_thread-vc140-mt-gd-x64-1_77)
target_link_libraries(testServer base)
target_link_libraries(testServer tcpAsio)
IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    message("testServer SYSTEM is Linux")
	target_link_libraries(testServer pthread)
ENDIF()


#install(TARGETS testServer DESTINATION S{CMAKE_RUNTIME_OUTPUT_DIRECTORY})