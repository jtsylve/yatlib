# SPDX-FileCopyrightText: © 2020-2023 Joe T. Sylve, Ph.D. <joe.sylve@gmail.com>
#
# SPDX-License-Identifier: Apache-2.0

target_sources(
    yat
    INTERFACE FILE_SET HEADERS
                       FILES "__compat.hpp"
                             "__config.hpp"
                             "array.hpp"
                             "concepts.hpp"
                             "type_traits.hpp"
                             "utility.hpp"
                             "variant.hpp"
                             "yatlib.hpp")
