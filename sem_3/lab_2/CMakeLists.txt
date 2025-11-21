cmake_minimum_required(VERSION 3.16)
project(lab_2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройка Qt
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Ручное включение MOC
qt6_wrap_cpp(MOC_SOURCES include/MainWindow.hpp)

# Исполняемый файл основной программы
add_executable(lab_2
        src/main.cpp
        src/MainWindow.cpp
        src/DatabaseStudent.cpp
        ${MOC_SOURCES}
)

# Подключение Qt к целевому исполняемому файлу
target_link_libraries(lab_2 Qt6::Core Qt6::Widgets)

# Включение директорий
target_include_directories(lab_2 PRIVATE include)