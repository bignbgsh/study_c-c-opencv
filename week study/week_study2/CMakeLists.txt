cmake_minimum_required(VERSION 3.16)

project(ContactBook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(contact_book
    main.cpp
    contact_manager.cpp
)