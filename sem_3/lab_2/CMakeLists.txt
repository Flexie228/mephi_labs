cmake_minimum_required(VERSION 3.16)
project(lab_2)

find_package(Catch2 REQUIRED)

# Основная программа
add_executable(lab_2 src/main.cpp
        include/Exceptions.hpp
        include/Hasher.hpp)

add_executable(tests tests/tests.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)