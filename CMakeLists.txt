cmake_minimum_required(VERSION 3.12)

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
find_package(absl REQUIRED)
target_link_libraries(
  ${PROJECT_NAME}
  INTERFACE absl::absl)

# Boilerplate cmake library install nonsense

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}_Targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "YatConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/YatConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/YatConfig.cmake" INSTALL_DESTINATION
  ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(
  EXPORT ${PROJECT_NAME}_Targets
  FILE "YatTargets.cmake"
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(
  FILES "${PROJECT_BINARY_DIR}/YatConfig.cmake"
        "${PROJECT_BINARY_DIR}/YatConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(
  DIRECTORY ${PROJECT_SOURCE_DIR}/include/yatlib
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(CPACK_RESOURCE_FILE_LICENSE
    "${PROJECT_SOURCE_DIR}/LICENSE")

include(CPack)
