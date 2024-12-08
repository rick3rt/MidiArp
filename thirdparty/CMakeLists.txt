
# include(FetchContent)
# FetchContent_Declare(
#     libremidi
#     GIT_REPOSITORY https://github.com/celtera/libremidi.git
#     GIT_TAG v4.5.0
#     SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libremidi
# )
# FetchContent_MakeAvailable(libremidi)
add_subdirectory(libremidi)