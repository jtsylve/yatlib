target_sources(
    yat
    INTERFACE FILE_SET HEADERS
                       BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(yatlib)
