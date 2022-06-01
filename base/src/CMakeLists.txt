cmake_minimum_required(VERSION 3.23.1) 

# set the project name 
project(base VERSION 1.0) 

# specify the C++ standard 
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED True) 


#添加cpp文件路径
aux_source_directory(./ src_current)
#aux_source_directory(./lockfree_queue src_lockfree_queue)
#aux_source_directory(./ring_buffer src_ring_buffer)



#add_library(base ${src_current} ${src_client} ${src_common} ${src_server})
add_library(base ${src_current})


target_include_directories(base INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}) 




install(DIRECTORY ./
DESTINATION ${INCLUDE_OUTPUT_PATH}/base/
FILES_MATCHING 
PATTERN "*.h"
PATTERN "build" EXCLUDE
)
