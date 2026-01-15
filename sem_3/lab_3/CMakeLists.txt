cmake_minimum_required(VERSION 4.0)
project(lab_3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_PREFIX_PATH "C:/Qt/6.10.1/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOUIC_SEARCH_PATHS src/gui)

add_executable(
        lab_3 src/main.cpp

        include/Graph.hpp
        include/VertexEdge.hpp
        include/PairWorks.hpp
        include/GridGraph.hpp
        include/VisualizationInfo.hpp

        include/gui/SizePicker.hpp
        include/gui/MainWindow.hpp

        src/gui/SizePicker.cpp
        src/gui/MainWindow.cpp

        src/gui/MainWindow.ui
        include/gui/GraphVisualizer.hpp
        src/gui/GraphVisualizer.cpp)
target_link_libraries(lab_3 PRIVATE Qt6::Widgets)

add_executable(
        GraphTests tests/GraphTests.cpp
        include/libs/catch2/catch_amalgamated.cpp)

add_executable(
        GridGraphTests tests/GridGraphTests.cpp
        include/libs/catch2/catch_amalgamated.cpp)