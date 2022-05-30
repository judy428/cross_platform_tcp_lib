cmake_minimum_required(VERSION 3.23.1) 

# set the project name 
project(testBase VERSION 1.0) 

# specify the C++ standard 
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED True) 


#定义工程根目录; CMAKE_SOURCE_DIR为内建变量，表示工程根目录的CMakeLists.txt文件路径
set(ROOT_DIR ${CMAKE_SOURCE_DIR})


#link_directories(${PROJECT_SOURCE_DIR}/lib)
# 指定引用的外部库的搜索路径
LINK_DIRECTORIES(${ROOT_DIR}/out/lib)

INCLUDE_DIRECTORIES(${ROOT_DIR}/out/include)
#link_directories("E:/code/c++/test/cross_platform_lib/tcp_base_on_asio/lib/Debug")

file(GLOB ALL_TEST_SOURCE_FILES ./*.cpp)

foreach(cppFile ${ALL_TEST_SOURCE_FILES})
    
    get_filename_component(mainname ${cppFile} NAME_WE)
    add_executable(${mainname} ${cppFile})
	target_link_libraries(${mainname} base)
	IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
		target_link_libraries(${mainname} pthread)
	ENDIF()
endforeach()




