
cmake_minimum_required(VERSION 3.16)

project(ArmorPlateDetection LANGUAGES CXX)

# 使用 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 OpenCV
find_package(
    OpenCV REQUIRED
    COMPONENTS core imgproc imgcodecs
)

# 指定参与编译的源文件
add_executable(armor_detect
    main.cpp
    armor_detector.cpp
)

# 设置头文件搜索目录
target_include_directories(
    armor_detect PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 链接 OpenCV 库
target_link_libraries(
    armor_detect PRIVATE
    ${OpenCV_LIBS}
)
