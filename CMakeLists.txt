cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(MidiArp VERSION 0.1.0 LANGUAGES CXX)

# add 3rd party libraries
add_subdirectory(thirdparty)

# use c++23
set(CMAKE_CXX_STANDARD 23)

add_executable(MidiArp src/main.cpp)

target_link_libraries(MidiArp PRIVATE libremidi)