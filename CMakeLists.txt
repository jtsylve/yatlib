cmake_minimum_required(VERSION 3.12)

project(yatlib CXX)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_feature(project_options INTERFACE cxx_std_17)
set_property(TARGET project_options PROPERTY CXX_STANDARD 20)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# static code analyzers
include(cmake/StaticAnalyzers.cmake)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# the main yatlib library
add_library(yatlib)
target_link_libraries(yatlib PRIVATE project_options project_warnings)

# Define headers for this library. PUBLIC headers are used for compiling the
# library, and will be added to consumers' build paths.
target_include_directories(
  yatlib
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include>
  PRIVATE src)

add_subdirectory(src)
