
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(test_proj)

############################
###  Configured Headers  ###
############################


#####################################
###  Global Compiler Definitions  ###
#####################################


##########################
###  Dependency Setup  ###
##########################
include(deps/external_dependancies/cadmium2.cmake) 						# Downloads cadmium and sets up variables for includes
include(deps/external_dependancies/generic_shared_memory_model.cmake)	# Downloads the generic shared memory model and sets up variables for includes



##########################
###  Global Variables  ###
##########################
set(includes_list 
	"${CADMIUM2_INCLUDE_DIR}"
	"${generic_shared_memory_model_INCLUDE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/deps"
	"${CMAKE_CURRENT_SOURCE_DIR}/deps/internal_dependancies"
	${CMAKE_CURRENT_SOURCE_DIR}/include
	CACHE STRING "Test program Includes List"
)

#################################
###  Compiler Specific Fixes  ###
#################################


########################
###  Subdirectories  ###
########################

add_subdirectory(test)
add_subdirectory(src)

