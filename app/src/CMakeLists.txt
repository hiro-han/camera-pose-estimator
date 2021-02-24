cmake_minimum_required(VERSION 2.8)
project(CameraPoseEstimator)

# Create Main project
set(SOURCES
    main.cpp
    camera_calibrator.cpp
)

set(HEADERS
    camera_calibrator.h
)

add_executable(CameraPoseEstimator
    ${SOURCES}
    ${HEADERS}
)

find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    target_include_directories(CameraPoseEstimator PUBLIC ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(CameraPoseEstimator ${OpenCV_LIBS})
endif()
