# SPDX-FileCopyrightText: © 2020-2023 Joe T. Sylve, Ph.D. <joe.sylve@gmail.com>
#
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.23)

project(
    yatlib
    VERSION 2.0.0
    DESCRIPTION "Yet Another Template Library for C++"
    HOMEPAGE_URL "https://github.com/jtsylve/yatlib"
    LANGUAGES CXX)

# Add cmake directory to module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Initialize CBP
include(CBP)

# Add the library
add_library(yat INTERFACE)
add_library(yat::yat ALIAS yat)

# Require at least C++20
target_compile_features(
    yat
    INTERFACE cxx_std_20)

# Add the include directory
target_include_directories(
    yat
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDE_DIR}>)

add_subdirectory(include)
