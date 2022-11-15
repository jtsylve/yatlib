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
      URL         ${PROJECT_SOURCE_DIR}/vendor/filesystem-1.5.12.tar.gz
      URL_HASH    SHA1=57db44da760eeca784caf58e0158949713452cac
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
      URL         ${PROJECT_SOURCE_DIR}/vendor/GSL-4.0.0.tar.gz
      URL_HASH    SHA1=909c9540a76fe4b4f71dbbd24126cab3925fb78e
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
      URL         ${PROJECT_SOURCE_DIR}/vendor/range-v3-0.12.0.tar.gz
      URL_HASH    SHA1=c7893c255a437aadae3b46661c7a814a19012746
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
      URL         ${PROJECT_SOURCE_DIR}/vendor/date-22ceabf.tar.gz
      URL_HASH    SHA1=af5a4c8a770578298974c0263d7d2628953f9749
)

set(BUILD_TZ_LIB ON CACHE BOOL "")
set(MANUAL_TZ_DB ON CACHE BOOL "")

FetchContent_MakeAvailable(date)

set(YATLIB_REQUIRES_DATE true PARENT_SCOPE)

endif()
