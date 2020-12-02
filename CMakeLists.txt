cmake_minimum_required(VERSION 3.12)

if(POLICY CMP0077)
  cmake_policy(SET CMP0077 OLD)
endif()

project(
  yat
  VERSION 0.0.0
  LANGUAGES CXX
  DESCRIPTION "Yet Another Template Library for C++"
  HOMEPAGE_URL "https://github.com/jtsylve/yatlib")

# the yatlib library
add_library(${PROJECT_NAME} INTERFACE)
# add alias so the project can be uses with add_subdirectory
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_compile_features(
  ${PROJECT_NAME}
  INTERFACE cxx_std_17)

# static code analyzers
include(cmake/StaticAnalyzers.cmake)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(${PROJECT_NAME})

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(${PROJECT_NAME})

include(GNUInstallDirs)

# Define headers for this library. PUBLIC headers are used for compiling the
# library, and will be added to consumers' build paths.
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# library dependencies

add_subdirectory("thirdparty")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  list(APPEND CMAKE_MODULE_PATH
       "${PROJECT_SOURCE_DIR}/thirdparty/catch2/contrib")
  add_subdirectory("tests")
endif()
