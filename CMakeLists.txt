cmake_minimum_required(VERSION 3.1)

project(MUC-Clementine)

set(CMAKE_CXX_STANDARD 17)

message(STATUS "Your operation system is ${CMAKE_SYSTEM_NAME} - ${CMAKE_SYSTEM}")

find_package(OpenCV REQUIRED)
message(STATUS "Found OpenCV Version: ${OpenCV_VERSION}")

include_directories(image/src)
include_directories(image/3rdparty/MVSDK/include)
include_directories(windmill/src)
include_directories(message/src)
include_directories(armor/src)

add_executable(windmill_detect_demo
        image/src/MVCameraInput.cpp
        image/src/VideoInput.cpp
        windmill/src/WindmillDetect.cpp
        windmill_detect_demo.cpp)

add_executable(armor_detect_demo
        image/src/MVCameraInput.cpp
        image/src/VideoInput.cpp
        armor/src/ArmorDetect.cpp
        armor_detect_demo.cpp)

if(WIN32)
    target_link_libraries(windmill_detect_demo 
        ${OpenCV_LIBS} 
        ${PROJECT_SOURCE_DIR}/image/3rdparty/MVSDK/lib/MVCAMSDK_X64.lib)

    target_link_libraries(armor_detect_demo 
        ${OpenCV_LIBS}
        ${PROJECT_SOURCE_DIR}/image/3rdparty/MVSDK/lib/MVCAMSDK_X64.lib)

elseif(UNIX)
    target_link_libraries(run ${OpenCV_LIBS} MVSDK)

    target_link_libraries(armor_detect_demo ${OpenCV_LIBS} MVSDK)

endif()
