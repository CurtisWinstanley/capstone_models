message(STATUS "Downloading GenericSharedMemoryModel main.")

include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
	generic_shared_memory_model
	GIT_REPOSITORY https://github.com/jwehorner/generic-shared-memory-model.git
	GIT_TAG "main"
	GIT_PROGRESS TRUE
)

FetchContent_GetProperties(generic_shared_memory_model)
if(NOT generic_shared_memory_model_POPULATED)
  FetchContent_Populate(generic_shared_memory_model)
endif()

message(STATUS "GenericSharedMemoryModel main downloaded.")

set(generic_shared_memory_model_INCLUDE_DIR 
	"${FETCHCONTENT_BASE_DIR}/generic_shared_memory_model-src" 
	CACHE STRING "generic_shared_memory_model Include File Location"
)
