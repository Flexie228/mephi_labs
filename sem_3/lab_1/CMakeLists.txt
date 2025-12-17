cmake_minimum_required(VERSION 3.28)
project(lab_1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH "C:/Qt/6.10.1/mingw_64")

# Библиотека (только заголовки)
add_library(lab_1_lib INTERFACE)
target_include_directories(lab_1_lib INTERFACE include)

# Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Основное приложение с GUI
add_executable(CompressionApp
        src/main.cpp
        src/gui/MainWindow.cpp
        src/gui/MainWindow.ui
        src/gui/moc_MainWindow.cpp
)

add_custom_command(TARGET CompressionApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Qt/6.10.1/mingw_64/bin/Qt6Core.dll"
        $<TARGET_FILE_DIR:CompressionApp>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Qt/6.10.1/mingw_64/bin/Qt6Gui.dll"
        $<TARGET_FILE_DIR:CompressionApp>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Qt/6.10.1/mingw_64/bin/Qt6Widgets.dll"
        $<TARGET_FILE_DIR:CompressionApp>
        COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:CompressionApp>/platforms"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Qt/6.10.1/mingw_64/plugins/platforms/qwindows.dll"
        "$<TARGET_FILE_DIR:CompressionApp>/platforms"
)

target_link_libraries(CompressionApp Qt6::Widgets lab_1_lib)

target_link_libraries(CompressionApp
        Qt6::Widgets
        lab_1_lib
)

target_include_directories(CompressionApp PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/CompressionApp_autogen/include
)

# Тесты
add_executable(testsStreams
        tests/testsStreams.cpp
        libs/catch2/catch_amalgamated.cpp
)

add_executable(tests
        tests/tests.cpp
        libs/catch2/catch_amalgamated.cpp
)