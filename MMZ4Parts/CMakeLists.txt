# CMakeList.txt : CMake project for MMZ4Parts, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Add source to this project's executable.
add_executable (MMZ4Parts)

target_sources(MMZ4Parts PRIVATE
	"main.cpp"
#  "Clay/clay_renderer_raylib.cpp"
 )

target_include_directories(MMZ4Parts PRIVATE
"${PROJECT_SOURCE_DIR}/"
"${PROJECT_SOURCE_DIR}/MMZ4Parts/"
"${PROJECT_SOURCE_DIR}/MMZ4Parts/Clay"
)

target_link_libraries(MMZ4Parts PRIVATE
	raylib
)

#target_link_libraries(MMZ4Parts PRIVATE
#clay
#)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MMZ4Parts PROPERTY CXX_STANDARD 20)
endif()

add_custom_command(
        TARGET MMZ4Parts POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources
        ${CMAKE_CURRENT_BINARY_DIR}/Resources)