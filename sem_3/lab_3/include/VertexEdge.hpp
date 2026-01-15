#ifndef VERTEX_EDGE_HPP
#define VERTEX_EDGE_HPP

#include <cmath>
#include <vector>
using namespace std;

constexpr int INF = std::numeric_limits<int>::max();

enum TerrainType {
    Ground = 1,     // Обычная земля
    Sand   = 2,     // Песок
    Hill   = 3,     // Холм
    Water  = 4,     // Вода
    Mountain = 5,   // Гора
    DeepWater = 6,   // Глубоководье
    Wall = INF  // Стена (физически удаляется, но вес для логики)
};

struct Vertex;
struct Edge;

struct VertexData {
    size_t id;
    int x{0}, y{0};
    int weight{Ground};

    void setData(int x_, int y_, int w) { this->x = x_; this->y = y_; this->weight = w; }
    void setCoord(int x_, int y_) { this->x = x_; this->y = y_; }
    void setWeight(int w) { this->weight = w; }
};

struct Vertex {
    std::vector<Edge*> incidentEdges;
    VertexData data;

    explicit Vertex(size_t id) : data{id} {}

    void addIncidentEdge(Edge* edge) {
        incidentEdges.push_back(edge);
    }
    // Манхеттенское расстояние
    size_t heuristic(const Vertex* other) const {
        if (!other) return 0;
        size_t dx = abs(data.x - other->data.x);
        size_t dy = abs(data.y - other->data.y);
        return dx + dy;
    }
};

struct EdgeData {
};

struct Edge {
    Vertex* vertex1;
    Vertex* vertex2;
    EdgeData data;

    Edge(Vertex* v1, Vertex* v2) : vertex1(v1), vertex2(v2) {}

    Vertex* getOtherVertex(const Vertex* vertex) const {
        if (vertex == vertex1) return vertex2;
        if (vertex == vertex2) return vertex1;
        return nullptr;
    }
};

#endif