message(STATUS "Adding Raylib")
add_subdirectory("raylib")

message(STATUS "Adding Flecs")
option(FLECS_STATIC "Build static flecs lib" ON)
option(FLECS_SHARED "Build shared flecs lib" OFF)
option(FLECS_PIC "Compile static flecs lib with position independent code (PIC)" ON)
option(FLECS_TESTS "Build flecs tests" OFF)
add_subdirectory("flecs")