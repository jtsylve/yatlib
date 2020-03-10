include(CTest)
include(Catch)

add_library(
  catch_main STATIC
  "catch_main.cpp")
target_link_libraries(catch_main Catch2::Catch2 yat::yat)

add_executable(
  unittests
  "bit_cast_test.cpp" "endian_test.cpp" "yatlib_test.cpp")
target_link_libraries(unittests catch_main)
catch_discover_tests(unittests)