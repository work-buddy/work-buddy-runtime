cmake_minimum_required(VERSION 3.28)
project(WorkBuddy LANGUAGES CXX CUDA)

set(CMAKE_CXX_COMPILER /usr/bin/c++)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /home/cjft/opencv-4.10.0/cmake)

set(CUDA_TOOLKIT_ROOT_DIR /usr/local/cuda-12.5)

find_package(CUDA 12.5 REQUIRED)
find_package(OpenCV REQUIRED)

# Print diagnostic messages
message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "CXX Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CUDA Toolkit Root Dir: ${CUDA_TOOLKIT_ROOT_DIR}")
message(STATUS "CUDA Version: ${CUDA_VERSION}")
message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(/usr/include/yaml-cpp)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/FPSCounter)
include_directories(${CMAKE_SOURCE_DIR}/src/YoloDetector)
include_directories(${CMAKE_SOURCE_DIR}/src/TextTyper)

set(SOURCES
    src/WorkBuddy.cpp
    src/FPSCounter/FPSCounter.cpp
    src/YoloDetector/YoloDetector.cpp
    src/TextTyper/TextTyper.cpp
)

add_executable(WorkBuddy ${SOURCES})
target_link_libraries(WorkBuddy ${OpenCV_LIBS} yaml-cpp)
