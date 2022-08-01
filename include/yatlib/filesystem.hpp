/*
 * Copyright 2021 Joe T. Sylve, Ph.D.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#if defined(_LIBCPP_FILESYSTEM) && !defined(__clang_analyzer__)
#error "This header must be imported prior to any <filesystem> imports!"
#endif

#include "features.hpp"

#if __has_include("filesystem") && !defined(YAT_APPLE_CXX17FS_TYPES_UNAVAILABLE)
#include <filesystem>
#endif

// Check to see if stdlib support is available
#if !defined(YAT_APPLE_CXX17FS_TYPES_UNAVAILABLE) && \
    defined(__cpp_lib_filesystem) && __cpp_lib_filesystem >= 201703

#define YAT_INTERNAL_USE_STD_FS

#endif  // FS Support check

#ifdef YAT_INTERNAL_USE_STD_FS

namespace yat {

namespace filesystem = std::filesystem;

using ifstream = std::ifstream;
using ofstream = std::ofstream;
using fstream = std::fstream;

}  // namespace yat

#else

#include <ghc/filesystem.hpp>

namespace yat {

namespace filesystem = ghc::filesystem;

using ifstream = ghc::filesystem::ifstream;
using ofstream = ghc::filesystem::ofstream;
using fstream = ghc::filesystem::fstream;

}  // namespace yat

#endif  // YAT_INTERNAL_USE_STD_FS

namespace yat {

/// Creates a copy of the path, but with a new extension
inline filesystem::path copy_with_extension(const filesystem::path &path,
                                            const filesystem::path &ext) {
  filesystem::path new_path{path};
  new_path.replace_extension(ext);
  return new_path;
}

}  // namespace yat

#undef YAT_INTERNAL_USE_STD_FS
