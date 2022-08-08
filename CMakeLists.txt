cmake_minimum_required(VERSION 3.22)

project(json_packer)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(nlohmann_json)
add_executable(json_packer BinaryWriter.cpp main.cpp)

target_include_directories(json_packer PRIVATE "nlohmann_json/single_include")

target_link_libraries(json_packer PRIVATE nlohmann_json::nlohmann_json)
