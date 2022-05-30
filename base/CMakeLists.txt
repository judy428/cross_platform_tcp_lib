cmake_minimum_required(VERSION 3.23.1) 

# set the project name 
project(base VERSION 1.0) 

# specify the C++ standard 
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED True) 
#
#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(src)
add_subdirectory(test)
#add_subdirectory(test/tcp_server)


execute_process(COMMAND pwd)