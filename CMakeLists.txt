cmake_minimum_required(VERSION 3.1)

project(MUC-Clementine)

set(CMAKE_CXX_STANDARD 17)

message(STATUS "Your operation system is ${CMAKE_SYSTEM_NAME} - ${CMAKE_SYSTEM}")

add_subdirectory(windmill)
add_subdirectory(image)
add_subdirectory(armor)

add_executable(windmill_detect_demo windmill_detect_demo.cpp)
target_link_libraries(windmill_detect_demo windmill_detect)

add_executable(armor_detect_demo armor_detect_demo.cpp)
target_link_libraries(armor_detect_demo armor_detect image)
