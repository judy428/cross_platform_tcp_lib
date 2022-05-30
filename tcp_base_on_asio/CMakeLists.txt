cmake_minimum_required(VERSION 3.23.1) 

# set the project name 
project(main VERSION 1.0) 

#
#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#
#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/test)

add_subdirectory(src)
add_subdirectory(src/tcp_client)
add_subdirectory(src/tcp_server)
add_subdirectory(test/tcp_client)
add_subdirectory(test/tcp_server)


execute_process(COMMAND pwd)