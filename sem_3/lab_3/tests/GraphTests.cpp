#include "../include/libs/catch2/catch_amalgamated.hpp"
#include "../include/Graph.hpp"

void BuildGraph(Graph& g) {
    g.addVertex(1);
    g.addVertex(2);
    g.addVertex(3);
    g.addVertex(4);
    g.addVertex(5);
    g.addVertex(6);
    g.addVertex(7);

    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(1, 5);
    g.addEdge(2, 3);
    g.addEdge(2, 6);
    g.addEdge(3, 7);
    g.addEdge(4, 5);
    g.addEdge(4, 6);
}

TEST_CASE("Graph basic functionality", "[graph]") {
    Graph g;
    SECTION("Vertex management") {
        auto* v1 = g.addVertex(1);
        REQUIRE(v1 != nullptr);
        REQUIRE(v1->data.id == 1);
        REQUIRE(g.getVertexCount() == 1);

        REQUIRE(g.addVertex(1) == nullptr);

        REQUIRE(g.removeVertex(1) == true);
        REQUIRE(g.getVertexCount() == 0);
        REQUIRE(g.getVertex(1) == nullptr);
    }
    SECTION("Edge management") {
        g.addVertex(1);
        g.addVertex(2);
        
        auto* e = g.addEdge(1, 2);
        REQUIRE(e != nullptr);
        REQUIRE(g.getEdgeCount() == 1);

        REQUIRE(g.findEdge(2, 1) == e);

        REQUIRE(g.removeEdge(1, 2) == true);
        REQUIRE(g.getEdgeCount() == 0);
    }
}

TEST_CASE("Graph Algorithms with sample image", "[algorithms]") {
    Graph g;
    BuildGraph(g);
    SECTION("BFS Verification (Distances)") {
        Vertex* start = g.getVertex(1);
        auto log = g.BFS(start);
        REQUIRE(log.size() == 7);
        // (0) у вершины 1, (1) у 2,3,4,5, (2) у 6,7
        for (const auto& step : log) {
            size_t id = step.vertex->data.id;
            if (id == 1) REQUIRE(step.discoveryTime == 0);
            else if (id >= 2 && id <= 5) REQUIRE(step.discoveryTime == 1);
            else if (id == 6 || id == 7) REQUIRE(step.discoveryTime == 2);
        }

        start = g.getVertex(7);
        log = g.BFS(start);
        REQUIRE(log.size() == 7);
        for (const auto& step : log) {
            size_t id = step.vertex->data.id;
            if (id == 3) REQUIRE(step.discoveryTime == 1);
            else if (id == 1 || id == 2) REQUIRE(step.discoveryTime == 2);
            else if (id == 4 || id == 5 || id == 6) REQUIRE(step.discoveryTime == 3);

        }
    }
    SECTION("DFS Verification (Connectivity and Finish Time)") {
        Vertex* start = g.getVertex(1);
        Vertex* target = g.getVertex(7);
        
        auto log = g.DFS(start, target);

        REQUIRE(!log.empty());
        bool foundTarget = false;
        for (const auto& step : log) {
            if (step.vertex->data.id == 7) {
                foundTarget = true;
                break;
            }
        }
        REQUIRE(foundTarget);
        foundTarget = false;

        // Другая КСС - Даже если начать искать в другой КСС поиск должен продолжиться исходя из презентации.
        g.addVertex(8);
        g.addVertex(9);
        g.addEdge(8, 9);
        target = g.getVertex(9);
        log = g.DFS(start, target);
        for (const auto& step : log) {
            if (step.vertex->data.id == 7) {
                foundTarget = true;
                break;
            }
        }
        REQUIRE(foundTarget);
    }
}

TEST_CASE("Graph Edge Cases", "[edge-cases]") {
    Graph g;
    SECTION("Empty graph BFS/DFS") {
        REQUIRE(g.BFS(nullptr).empty());
        REQUIRE(g.DFS(nullptr, nullptr).empty());
    }
    SECTION("Isolated vertex") {
        g.addVertex(1);
        g.addVertex(2);
        
        auto log = g.BFS(g.getVertex(1));
        REQUIRE(log.size() == 1); // BFS не должен найти на 2, если нет ребра
    }
    SECTION("Self-loops are prohibited") {
        g.addVertex(1);
        REQUIRE(g.addEdge(1, 1) == nullptr);
    }
}