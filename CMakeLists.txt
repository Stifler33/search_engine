cmake_minimum_required(VERSION 3.21)
project(search_engine)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)
add_executable(search_engine main.cpp headers.h class_implementation.cpp)
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)
