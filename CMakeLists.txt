cmake_minimum_required(VERSION 3.28)
project(WorkBuddy LANGUAGES CXX CUDA)

set(CMAKE_CXX_COMPILER /usr/bin/c++)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /home/cjft/opencv-4.10.0/cmake)

set(CUDA_TOOLKIT_ROOT_DIR /usr/local/cuda-12.5)

find_package(CUDA 12.5 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "CXX Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CUDA Toolkit Root Dir: ${CUDA_TOOLKIT_ROOT_DIR}")
message(STATUS "CUDA Version: ${CUDA_VERSION}")
message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")
message(STATUS "CURL_INCLUDE_DIRS = ${CURL_INCLUDE_DIRS}")
message(STATUS "CURL_LIBRARIES = ${CURL_LIBRARIES}")

add_executable(WorkBuddy
    src/WorkBuddy.cpp
    src/FPSCounter/FPSCounter.cpp
    src/YoloDetector/YoloDetector.cpp
    src/TextTyper/TextTyper.cpp
    src/ORBTracker/ORBTracker.cpp
    src/OpenAI/OpenAI.cpp
    src/ChatOverlay/ChatOverlay.cpp
    src/utils/Base64/Base64.cpp
)

target_include_directories(WorkBuddy PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
    /usr/include/yaml-cpp
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/FPSCounter
    ${CMAKE_SOURCE_DIR}/src/YoloDetector
    ${CMAKE_SOURCE_DIR}/src/TextTyper
    ${CMAKE_SOURCE_DIR}/src/ORBTracker
    ${CMAKE_SOURCE_DIR}/src/OpenAI
    ${CMAKE_SOURCE_DIR}/src/ChatOverlay
    ${CMAKE_SOURCE_DIR}/src/utils/Base64
)

target_link_libraries(WorkBuddy PRIVATE
    ${OpenCV_LIBS}
    yaml-cpp
    jsoncpp_lib
    ${CURL_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
)
