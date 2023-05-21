# SPDX-FileCopyrightText: © 2023 Joe T. Sylve, Ph.D. <joe.sylve@gmail.com>
#
# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.21)

# Include CPM
include(CPM)

message(STATUS "Adding CBP Modules and Initializing Project")

# Fetch CBP
cpmaddpackage("gh:jtsylve/CBP#main")

# Add CBP modules to the project
list(APPEND CMAKE_MODULE_PATH "${CBP_SOURCE_DIR}/cmake")

# Initialize the project
include(DefaultProjectOptions)
