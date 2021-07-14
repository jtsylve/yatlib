cmake_minimum_required(VERSION 3.12)

project(
  yat
  VERSION 1.0.0
  LANGUAGES CXX
  DESCRIPTION "Yet Another Template Library for C++"
  HOMEPAGE_URL "https://github.com/jtsylve/yatlib")

# Dependencies
add_subdirectory(thirdparty)

# the yatlib library
add_library(yat INTERFACE)
# add alias so the project can be uses with add_subdirectory
add_library(yat::yat ALIAS yat)
target_compile_features(
  yat
  INTERFACE cxx_std_17)

# Define headers for this library. PUBLIC headers are used for compiling the
# library, and will be added to consumers' build paths.
target_include_directories(
  yat
  INTERFACE $<BUILD_INTERFACE:${yat_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(yat INTERFACE ghcFilesystem::ghc_filesystem)

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

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  # static code analyzers
  include(cmake/StaticAnalyzers.cmake)

  # sanitizer options if supported by compiler
  include(cmake/Sanitizers.cmake)
  enable_sanitizers(yat)

  # standard compiler warnings
  include(cmake/CompilerWarnings.cmake)
  set_project_warnings(yat)

  enable_testing()
  add_subdirectory("tests")
endif()
