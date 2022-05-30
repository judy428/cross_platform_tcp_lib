cmake_minimum_required(VERSION 3.23.1) 

# set the project name 
project(main VERSION 1.0) 

#
file(GLOB mySpdlog "./spdlog/include/*")
file(COPY ${mySpdlog}  DESTINATION ${INCLUDE_OUTPUT_PATH}/)

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
	message("3rd SYSTEM is Linux")
	
	file(GLOB boostInclude "./boost_1_77_0/linux/include/*")
	file(COPY ${boostInclude}  DESTINATION ${INCLUDE_OUTPUT_PATH}/)
	
	#file(GLOB boostLib "./boost_1_77_0/linux/lib/*")
	#file(COPY ${boostLib}  DESTINATION ${LIBRARY_OUTPUT_PATH}/)
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	message("3rd SYSTEM is Windows")
	
	file(GLOB boostInclude "./boost_1_77_0/windows/include/boost-1_77/*")
	file(COPY ${boostInclude}  DESTINATION ${INCLUDE_OUTPUT_PATH}/)
	
	#file(GLOB boostLib "./boost_1_77_0/windows/lib/*")
	#file(COPY ${boostLib}  DESTINATION ${LIBRARY_OUTPUT_PATH}/)
ENDIF()



execute_process(COMMAND pwd)