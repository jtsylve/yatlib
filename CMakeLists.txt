cmake_minimum_required(VERSION 3.12)

project(
  yat
  VERSION 1.0.0
  LANGUAGES CXX
  DESCRIPTION "Yet Another Template Library for C++"
  HOMEPAGE_URL "https://github.com/jtsylve/yatlib")

# the yatlib library
add_library(yat INTERFACE)
# add alias so the project can be uses with add_subdirectory
add_library(yat::yat ALIAS yat)
target_compile_features(
  yat
  INTERFACE cxx_std_17)

# static code analyzers
include(cmake/StaticAnalyzers.cmake)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(yat)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(yat)

# Define headers for this library. PUBLIC headers are used for compiling the
# library, and will be added to consumers' build paths.
target_include_directories(
  yat
  INTERFACE $<BUILD_INTERFACE:${yat_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

include(GNUInstallDirs)

install(
  TARGETS yat
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(
  DIRECTORY include/yatlib
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING
  PATTERN *.hpp)

# library dependencies

add_subdirectory("thirdparty")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  list(APPEND CMAKE_MODULE_PATH
       "${PROJECT_SOURCE_DIR}/thirdparty/catch2/contrib")
  enable_testing()
  add_subdirectory("tests")
endif()
