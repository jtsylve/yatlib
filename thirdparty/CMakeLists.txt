include(CheckCXXSourceCompiles)
include(FetchContent) 

check_cxx_source_compiles(
      "
      #include <filesystem>
      static_assert(__cpp_lib_filesystem >= 201703);

      int main() {
        std::filesystem::path p{};
        return 0;
      }
      " 
      YATLIB_HAS_STDLIB_FILESYSTEM
)

if (NOT YATLIB_HAS_STDLIB_FILESYSTEM)

FetchContent_Declare(ghc
      GIT_REPOSITORY https://github.com/gulrak/filesystem.git
      GIT_TAG        "cd6805e94dd5d6346be1b75a54cdc27787319dd2" # v1.5.12
)
FetchContent_MakeAvailable(ghc)

set(YATLIB_REQUIRES_GHC true PARENT_SCOPE)

endif()

check_cxx_source_compiles(
      "
      #include <span>
      static_assert(__cpp_lib_span >= 202002L);

      int main() {
        return 0;
      }
      " 
      YATLIB_HAS_STDLIB_SPAN
)

if (NOT YATLIB_HAS_STDLIB_SPAN)

FetchContent_Declare(GSL
      GIT_REPOSITORY https://github.com/microsoft/GSL.git
      GIT_TAG        "a3534567187d2edc428efd3f13466ff75fe5805c" # v4.0.0
)
FetchContent_MakeAvailable(GSL)

set(YATLIB_REQUIRES_GSL true PARENT_SCOPE)

endif()

check_cxx_source_compiles(
      "
      #include <ranges>
      static_assert(__cpp_lib_ranges >= 201911L);

      int main() {
        return 0;
      }
      " 
      YATLIB_HAS_STDLIB_RANGES
)

check_cxx_source_compiles(
      "
      #include <ranges>
      static_assert(__cpp_lib_ranges_zip >= 202110L);

      int main() {
        return 0;
      }
      " 
      YATLIB_HAS_STDLIB_RANGES_ZIP
)

check_cxx_source_compiles(
      "
      #if defined(__cpp_concepts) && __cpp_concepts >= 201907L
      #include <concepts>
      static_assert(__cpp_lib_concepts >= 202002L);
      #endif

      int main() {
        return 0;
      }
      " 
      YATLIB_HAS_STDLIB_CONCEPTS
)


if (NOT (YATLIB_HAS_STDLIB_RANGES AND
         YATLIB_HAS_STDLIB_RANGES_ZIP AND    
         YATLIB_HAS_STDLIB_CONCEPTS))
FetchContent_Declare(rangev3
      GIT_REPOSITORY https://github.com/ericniebler/range-v3
      GIT_TAG        "0fa54d7de5dc00655e38a08e87cda61f7aa6d5b9"
)

set(RANGES_BUILD_CALENDAR_EXAMPLE OFF CACHE BOOL "")
set(RANGES_ASSERTIONS OFF CACHE BOOL "")
set(RANGES_DEBUG_INFO OFF CACHE BOOL "")
set(RANGES_NATIVE OFF CACHE BOOL "")
set(RANGES_ENABLE_WERROR OFF CACHE BOOL "")
set(RANGE_V3_EXAMPLES OFF CACHE BOOL "")
set(RANGE_V3_TESTS OFF CACHE BOOL "")
set(RANGE_V3_DOCS OFF CACHE BOOL "")

FetchContent_MakeAvailable(rangev3)

set(YATLIB_REQUIRES_RANGEV3 true PARENT_SCOPE)

endif()

check_cxx_source_compiles(
      "
      #include <chrono>
      static_assert(__cpp_lib_chrono >= 201907L);
     
      int main() {
        return 0;
      }
      " 
      YATLIB_HAS_STDLIB_CHRONO
)

if (NOT YATLIB_HAS_STDLIB_CHRONO)

FetchContent_Declare(date
      GIT_REPOSITORY https://github.com/HowardHinnant/date.git
      GIT_TAG        "e6f4aed4d11d4e50687e9aa8630c7f0db7d31dba"
)

set(BUILD_TZ_LIB ON CACHE BOOL "")
set(MANUAL_TZ_DB ON CACHE BOOL "")

FetchContent_MakeAvailable(date)

set(YATLIB_REQUIRES_DATE true PARENT_SCOPE)

endif()
