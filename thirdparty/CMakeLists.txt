include(FetchContent) 

FetchContent_Declare(ghc
      GIT_REPOSITORY https://github.com/gulrak/filesystem.git
      GIT_TAG        "cd6805e94dd5d6346be1b75a54cdc27787319dd2" # v1.5.12
)
FetchContent_MakeAvailable(ghc)

FetchContent_Declare(GSL
      GIT_REPOSITORY https://github.com/microsoft/GSL.git
      GIT_TAG        "a3534567187d2edc428efd3f13466ff75fe5805c" # v4.0.0
)
FetchContent_MakeAvailable(GSL)

FetchContent_Declare(rangev3
      GIT_REPOSITORY https://github.com/ericniebler/range-v3
      GIT_TAG        "4989f3e9ff2efee1852942bb9328ef121369ba02" # v0.11.0
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
