#ifndef VISUALIZATION_HPP
#define VISUALIZATION_HPP

#include "VertexEdge.hpp"

struct VisualStep {
    Vertex* vertex{};     // Какая вершина обрабатывается
    int discoveryTime{0};  // Время входа (или расстояние для BFS/Dijkstra)
    int finishTime{0};  // Время выхода (только для DFS)
    Vertex* parent{nullptr}; // Откуда пришли (чтобы рисовать стрелки пути)
};

#endif
