include(FetchContent) 

FetchContent_Declare(ghc
      GIT_REPOSITORY https://github.com/gulrak/filesystem.git
      GIT_TAG        "1d3d5f5d7d86e16592d293d9e72fdb7e44aef51f" # v1.5.8
)
FetchContent_MakeAvailable(ghc)

FetchContent_Declare(GSL
      GIT_REPOSITORY https://github.com/microsoft/GSL.git
      GIT_TAG        "0f6dbc9e2915ef5c16830f3fa3565738de2a9230" # v3.1.0
)
FetchContent_MakeAvailable(GSL)