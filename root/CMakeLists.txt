# CMakeList.txt : CMake project , include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.28)

# Define executable
add_executable (${CMAKE_PROJECT_NAME})

# executable options
option(PROFILING "Whether to activate profiling in the project" ON)

# Define sources by group
# source files
file(GLOB ROOT_SRC
	"src/*.cpp"
)
# header files
file(GLOB ROOT_HDR
	"src/*.h"
)

# Management scripts
file(GLOB_RECURSE SCRIPTS
	"scripts/*.bat"
)

# Utils
file(GLOB_RECURSE UTILS
	"utils/*.h"
	"utils/*.cpp"
)

# Add sources to target
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
	${ROOT_SRC}
	${ROOT_HDR}
	${SCRIPTS}
	${UTILS}

	# PRIVATE FILE_SET CXX_MODULES FILES # Needed for modules, there's none in the project yet
)

# Separate source groups
source_group("src" FILES
${ROOT_SRC}
)
source_group("hdr" FILES
${ROOT_HDR}
)
source_group("utils" FILES
${UTILS}
)
source_group("scripts" FILES
${SCRIPTS}
)

# Set target include directories
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
"${PROJECT_SOURCE_DIR}/"
"${PROJECT_SOURCE_DIR}/root"
"${PROJECT_SOURCE_DIR}/root/src"
"${PROJECT_SOURCE_DIR}/ext/flecs/include"
)

# Set target PCH
target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE
"utils/profiler.h"
)

# Link libraries to .exe
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
	raylib
	flecs_static
)

# set CPP version
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

# If PROFILING is requested add it to the project
if(PROFILING)
add_compile_definitions(PROFILE=1)
endif()

# Change working directory
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

# Copy resources to the right folder
add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        ${PROJECT_BINARY_DIR}/resources)