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