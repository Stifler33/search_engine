cmake_minimum_required(VERSION 3.21)
project(search_engine)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_executable(search_engine main.cpp headers.h class_implementation.cpp)
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

add_executable(myGtest myTest/myGtest.cpp headers.h class_implementation.cpp)
target_link_libraries(myGtest gtest_main)


#target_link_libraries(search_engine PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(myGtest)
enable_testing()