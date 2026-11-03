cmake_minimum_required(VERSION 3.16)

project(OpenCVScheme1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(
    OpenCV REQUIRED
    COMPONENTS core geometry imgproc imgcodecs
)

add_executable(opencv_scheme1
    main.cpp
    image_processing.cpp
)

target_include_directories(
    opencv_scheme1
    PRIVATE
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(
    opencv_scheme1
    PRIVATE
    ${OpenCV_LIBS}
)
