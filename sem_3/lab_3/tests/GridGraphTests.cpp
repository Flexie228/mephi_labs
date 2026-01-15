#include "../include/libs/catch2/catch_amalgamated.hpp"
#include "../include/GridGraph.hpp"

TEST_CASE("GridGraph: Basic Functionality", "[grid]") {
    constexpr size_t W = 5;
    constexpr size_t H = 5;
    GridGraph gg(W, H);

    SECTION("Grid construction and initial state") {
        for (size_t y = 0; y < H; ++y) {
            for (size_t x = 0; x < W; ++x) {
                VertexData data = gg.getVertexData(x, y);
                REQUIRE(data.x == static_cast<int>(x));
                REQUIRE(data.y == static_cast<int>(y));
                REQUIRE(data.weight == Ground);
            }
        }
    }
    SECTION("Cell management (Weights and Restoration)") {
        // Изменение веса (Земля -> Песок)
        REQUIRE(gg.setVertexWeight(1, 1, Sand) == true);
        REQUIRE(gg.getVertexData(1, 1).weight == Sand);

        // Удаление клетки (создание стены)
        REQUIRE(gg.removeCell(2, 2) == true);
        REQUIRE(gg.getVertexData(2, 2).weight == Wall);

        // Восстановление через setVertexWeight
        REQUIRE(gg.setVertexWeight(2, 2, Water) == true);
        REQUIRE(gg.getVertexData(2, 2).weight == Water);
    }
}

TEST_CASE("GridGraph: Algorithms", "[grid_algorithms]") {
    GridGraph gg(3, 3);

    // (0,2) (1,2) (2,2)
    // (0,1) (1,1) (2,1)
    // (0,0) (1,0) (2,0)

    SECTION("Dijkstra: Weighted path cost") {
        //(Гора - вес 5)
        gg.setVertexWeight(1, 1, Mountain);

        // Путь из (0,0) в (2,2)
        auto path = gg.Dijkstra(0, 0, 2, 2);

        // Кратчайший по весам путь пойдет в обход (через 1,0 -> 2,0 -> 2,1 -> 2,2)
        // Стоимость: 1 + 1 + 1 + 1 = 4 (без учета старта)
        // Если бы пошел через центр: 1 (1,0) + 5 (1,1) + 1 (2,1) + 1 (2,2) = 8
        REQUIRE(!path.empty());
        REQUIRE(path.back().discoveryTime == 4);
    }
    SECTION("A* vs Dijkstra") {
        GridGraph largeGG(10, 10);
        auto dijkstraLog = largeGG.Dijkstra(0, 0, 9, 9);
        auto astarLog = largeGG.AStar(0, 0, 9, 9);

        REQUIRE(astarLog.size() < dijkstraLog.size());
    }
    SECTION("BFS: Connectivity and Walls") {
        // Создаем стену, перекрывающую доступ к правой части
        gg.removeCell(1, 0);
        gg.removeCell(1, 1);
        gg.removeCell(1, 2);

        auto log = gg.BFS(0, 0);
        // Должны быть найдены только 3 клетки левого столбца
        REQUIRE(log.size() == 3);
    }
}

TEST_CASE("GridGraph: Edge Cases", "[grid_edge-cases]") {
    GridGraph gg(3, 3);

    SECTION("Out of bounds access") {
        // getVertexData должна вернуть Wall для координат вне сетки
        REQUIRE(gg.getVertexData(10, 10).weight == Wall);
        REQUIRE(gg.setVertexWeight(5, 5, Ground) == false);
        REQUIRE(gg.removeCell(3, 3) == false);
    }
    SECTION("Unreachable target") {
        // Полностью изолируем старт
        gg.removeCell(0, 1);
        gg.removeCell(1, 0);

        auto path = gg.Dijkstra(0, 0, 2, 2);
        // В логе должен быть только сам старт
        REQUIRE(path.size() == 1);

        path = gg.AStar(0, 0, 2, 2);
        REQUIRE(path.size() == 1);
    }
    SECTION("Reset") {
        gg.removeCell(1, 1);
        gg.setVertexWeight(0, 0, Mountain);

        gg.resetGrid();

        REQUIRE(gg.getVertexData(1, 1).weight == Ground);
        REQUIRE(gg.getVertexData(0, 0).weight == Ground);
    }
}