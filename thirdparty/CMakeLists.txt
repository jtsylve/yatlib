include(FetchContent) 

FetchContent_Declare(ghc
      GIT_REPOSITORY https://github.com/gulrak/filesystem.git
      GIT_TAG        "1d3d5f5d7d86e16592d293d9e72fdb7e44aef51f" # v1.5.8
)

FetchContent_MakeAvailable(ghc)