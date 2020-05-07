include(CTest)
include(Catch)

add_library(
  catch_main STATIC
  "catch_main.cpp")
target_link_libraries(catch_main Catch2::Catch2 yat::yat)

add_executable(
  unittests
  "any_test.cpp"
  "array_test.cpp"
  "bit_cast_test.cpp"
  "byteswap_test.cpp"
  "common.hpp"
  "endian_test.cpp"
  "memory_test.cpp"
  "optional_test.cpp"
  "type_traits_test.cpp"
  "utility_test.cpp"
  "variant_test.cpp"
  "yatlib_test.cpp")
target_link_libraries(unittests catch_main)
catch_discover_tests(unittests)
