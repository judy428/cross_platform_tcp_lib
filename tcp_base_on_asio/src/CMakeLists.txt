cmake_minimum_required(VERSION 3.23.1) 

# set the project name 
project(tcpAsio VERSION 1.0) 

# specify the C++ standard 
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED True) 

#添加cpp文件路径
aux_source_directory(./ src_current)
aux_source_directory(./tcp_client src_client)
aux_source_directory(./tcp_common src_common)
aux_source_directory(./tcp_server src_server)


include_directories(${INCLUDE_OUTPUT_PATH})

#add_executable(tcpAsio calculate.cpp) 
#add_library(tcpAsio ${src_current} ${src_client} ${src_common} ${src_server}  ${src_base})
add_library(tcpAsio ${src_current} ${src_client} ${src_common} ${src_server})

target_include_directories(tcpAsio INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

#install head-file

# install(DIRECTORY ./ ./tcp_client ./tcp_common ./tcp_server
# DESTINATION ${INCLUDE_OUTPUT_PATH}/tcpp_base_on_asio/
# FILES_MATCHING 
# PATTERN "*.h"
# PATTERN "build" EXCLUDE
# )
set(INSTALL_HEAD_FILE ./ ./tcp_client ./tcp_common ./tcp_server)

install(DIRECTORY ${INSTALL_HEAD_FILE}
DESTINATION ${INCLUDE_OUTPUT_PATH}/tcpp_base_on_asio/
FILES_MATCHING 
PATTERN "*.h"
PATTERN "build" EXCLUDE
)