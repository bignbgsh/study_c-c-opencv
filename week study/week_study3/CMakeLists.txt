cmake_minimum_required(VERSION 3.16)

project(OpenCVBasic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(opencv_basic
    main.cpp
    image_basic.cpp
)

target_include_directories(
    opencv_basic PRIVATE ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(
    opencv_basic PRIVATE ${OpenCV_LIBS}
)