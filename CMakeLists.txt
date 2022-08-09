cmake_minimum_required(VERSION 3.18)

project(json_packer)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_subdirectory(nlohmann_json)
add_executable(json_packer BinaryWriter.cpp Worker.cpp main.cpp)

target_include_directories(json_packer PRIVATE "nlohmann_json/single_include")

target_link_libraries(json_packer PRIVATE nlohmann_json::nlohmann_json)


enable_testing()

add_executable(
  tests
  BinaryWriter.cpp
  tests.cc
)
target_link_libraries(
  tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)

