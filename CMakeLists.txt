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
