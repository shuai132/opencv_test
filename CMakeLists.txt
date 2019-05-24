cmake_minimum_required(VERSION 3.13)
project(opencv_test)

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} main.cpp)

#set(OpenCV_DIR /usr/local/Cellar/opencv@2/2.4.13.7_3/share/OpenCV)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
#link_directories(${OpenCV_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

