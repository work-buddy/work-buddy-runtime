cmake_minimum_required(VERSION 3.28)
project( WorkBuddy )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

include_directories( ${CMAKE_SOURCE_DIR}/src )
include_directories( ${CMAKE_SOURCE_DIR}/src/FPSCounter )

set(SOURCES
    src/WorkBuddy.cpp
    src/FPSCounter/FPSCounter.cpp
)

add_executable( WorkBuddy ${SOURCES} )
target_link_libraries(WorkBuddy ${OpenCV_LIBS})
