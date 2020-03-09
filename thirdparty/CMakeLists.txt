set(BUILD_TESTING
    OFF)
set(CMAKE_CXX_STANDARD
    17)
add_subdirectory("abseil-cpp" EXCLUDE_FROM_ALL)
add_subdirectory("catch2" EXCLUDE_FROM_ALL)
