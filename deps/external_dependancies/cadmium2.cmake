include(FetchContent)

FetchContent_Declare(
  cadmium2
  GIT_REPOSITORY https://github.com/CurtisWinstanley/async_inputs_cadmium_v2.git
  GIT_TAG dev-rt
  GIT_PROGRESS TRUE
  GIT_SUBMODULES ""  # Do not initialize submodules
)

FetchContent_GetProperties(cadmium2)
if(NOT cadmium2_POPULATED)
  FetchContent_Populate(cadmium2)
endif()

set(CADMIUM2_INCLUDE_DIR ${FETCHCONTENT_BASE_DIR}/cadmium2-src/include CACHE STRING "Cadmium2 Include File Location")
