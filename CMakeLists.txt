cmake_minimum_required(VERSION 3.13)
project(opencv_test)

set(CMAKE_CXX_STANDARD 11)

set(EXEC_NAME main)
add_executable(${EXEC_NAME} ${EXEC_NAME}.cpp)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(${EXEC_NAME} ${OpenCV_LIBS})


set(EXEC_NAME HoughLinesP_whole)
add_executable(${EXEC_NAME} ${EXEC_NAME}.cpp)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(${EXEC_NAME} ${OpenCV_LIBS})


set(EXEC_NAME HoughLinesP_part)
add_executable(${EXEC_NAME} ${EXEC_NAME}.cpp)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(${EXEC_NAME} ${OpenCV_LIBS})


set(EXEC_NAME Canny_avg)
add_executable(${EXEC_NAME} ${EXEC_NAME}.cpp)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(${EXEC_NAME} ${OpenCV_LIBS})

