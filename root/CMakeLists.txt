# CMakeList.txt : CMake project , include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

option(PROFILING "Whether to activate profiling in the project" ON)

# Add source to this project's executable.
add_executable (${CMAKE_PROJECT_NAME})

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
	"main.cpp"
  "utils/profiler.h"
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
"${PROJECT_SOURCE_DIR}/"
"${PROJECT_SOURCE_DIR}/root/"
"${PROJECT_SOURCE_DIR}/root/Clay"
)

target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE
"utils/profiler.h"
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
	raylib
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

if(PROFILING)
add_compile_definitions(PROFILE=1)
endif()

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        ${PROJECT_BINARY_DIR}/resources)