cmake_minimum_required(VERSION 2.8)

set(PROJECT CameraPoseEstimator)
set(PROJECT_TEST ${PROJECT}-TEST)
project(${PROJECT})


# Setting
SET(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_DIR}/bin)


# Sources
file(
    GLOB_RECURSE
    SOURCES
    ${PROJECT_DIR}/src/camera_calibrator.cpp
)


# Libraries
find_package(OpenCV REQUIRED)


# Main
file(GLOB_RECURSE MAIN ${PROJECT_DIR}/src/main.cpp)
add_executable(${PROJECT} ${MAIN} ${SOURCES})
target_include_directories(${PROJECT} PUBLIC ${PROJECT_DIR}/src)
target_include_directories(${PROJECT} PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT} ${OpenCV_LIBS})
target_link_libraries(${PROJECT} INTERFACE ${PROJECT_DIR}/3rdlibs/argparse/include)


# Test
#file(
#    GLOB_RECURSE
#    TEST
#    test/*.cpp
#)
#add_executable(${PROJECT_TEST} ${TEST} ${SOURCES})
#target_include_directories(${PROJECT_TEST} PUBLIC ${PROJECT_DIR}/src)
#target_include_directories(${PROJECT_TEST} PUBLIC ${OpenCV_INCLUDE_DIRS})
#target_link_libraries(${PROJECT_TEST} ${OpenCV_LIBS})
#target_link_libraries(${PROJECT} INTERFACE ${PROJECT_DIR}/3rdlibs/argparse/include)
