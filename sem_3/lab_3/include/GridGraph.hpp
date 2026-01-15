#ifndef GRID_GRAPH_HPP
#define GRID_GRAPH_HPP

#include "VertexEdge.hpp"
#include "Graph.hpp"
#include <vector>
#include <queue>

class GridGraph {
    Graph graph;
    size_t width;
    size_t height;

public:
    explicit GridGraph(size_t w, size_t h) : width(w), height(h) {
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                size_t id = y * width + x;
                if (Vertex* v = graph.addVertex(id)) {
                    v->data.x = static_cast<int>(x);
                    v->data.y = static_cast<int>(y);
                }
            }
        }

        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                size_t currentId = y * width + x;

                if (x + 1 < width) {
                    graph.addEdge(currentId, y * width + (x + 1));
                }
                if (y + 1 < height) {
                    graph.addEdge(currentId, (y + 1) * width + x);
                }
            }
        }
    }

    bool setVertexInfo(size_t id, int x, int y, int w) {
        Vertex* v = graph.addVertex(id);
        if (!v) return false;
        v->data.setData(x, y, w);
        return true;
    }
    bool setVertexWeight(size_t x, size_t y, int w) {
        if (x >= width || y >= height) return false;

        Vertex* v = getVertexAt(x, y);
        if (!v) v = addCell(x, y);

        v->data.weight = w;
        return true;
    }
    bool removeCell(size_t x, size_t y) {
        Vertex* v = getVertexAt(x, y);
        if (!v) return false;
        return graph.removeVertex(y * width + x);
    }

    [[nodiscard]] int getVertexWeight(size_t x, size_t y) const {
        Vertex* v = getVertexAt(x, y);

        if (!v) return INF; // Стена

        return v->data.weight;
    }
    [[nodiscard]] VertexData getVertexData(size_t x, size_t y) const {
        Vertex* v = getVertexAt(x, y);
        if (!v) {
            VertexData wallData;
            wallData.setData(static_cast<int>(x), static_cast<int>(y), Wall);
            return wallData;
        }
        return v->data;
    }

    void resetGrid() {
        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width; ++x) {
                setVertexWeight(x, y, 1);
            }
        }
    }

    vector<VisualStep> BFS(size_t startX, size_t startY) const {
        return graph.BFS(getVertexAt(startX, startY));
    }
    vector<VisualStep> DFS(size_t startX, size_t startY, size_t endX, size_t endY) const {
        return graph.DFS(getVertexAt(startX, startY), getVertexAt(endX, endY));
    }
    vector<VisualStep> Dijkstra(size_t startX, size_t startY, size_t endX, size_t endY) const {
        Vertex* start = getVertexAt(startX, startY);
        Vertex* end = getVertexAt(endX, endY);
        if (!start || !end) return {};

        // Двоичная куча: {накопленный_вес, вершина}
        priority_queue<pair<int, Vertex*>, vector<pair<int, Vertex*>>, greater<>> pq;

        unordered_map<Vertex*, int> min_dist;
        unordered_map<Vertex*, Vertex*> parents;
        vector<VisualStep> log;

        auto relax = [&](Vertex* u, Vertex* v, int w) {
            int distV = min_dist.contains(v) ? min_dist[v] : INF;

            if (distV > min_dist[u] + w) {
                min_dist[v] = min_dist[u] + w;
                parents[v] = u;
                pq.emplace(min_dist[v], v);
            }
        };

        pq.emplace(0, start);
        min_dist[start] = 0;
        parents[start] = nullptr;

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (d > min_dist[u]) continue;

            log.push_back({u, d, 0, parents[u]});
            if (u == end) break;

            for (Edge* edge : u->incidentEdges) {
                Vertex* v = edge->getOtherVertex(u);
                relax(u, v, v->data.weight);
            }
        }
        return log;
    }
    vector<VisualStep> AStar(size_t startX, size_t startY, size_t endX, size_t endY) const {
        Vertex* start = getVertexAt(startX, startY);
        Vertex* end = getVertexAt(endX, endY);
        if (!start || !end) return {};

        // В очереди храним {f_score, вершина}, но в min_dist храним только g_score (реальный путь)
        priority_queue<pair<int, Vertex*>, vector<pair<int, Vertex*>>, greater<>> pq;

        unordered_map<Vertex*, int> g_score;
        unordered_map<Vertex*, Vertex*> parents;
        vector<VisualStep> log;

        auto relax = [&](Vertex* u, Vertex* v, int w) {
            int distV = g_score.contains(v) ? g_score[v] : INF;

            if (distV > g_score[u] + w) {
                g_score[v] = g_score[u] + w;
                parents[v] = u;

                // f_score = g_score + h_score
                int f_score = g_score[v] + static_cast<int>(v->heuristic(end));
                pq.emplace(f_score, v);
            }
        };

        g_score[start] = 0;
        parents[start] = nullptr;
        // Для старта f_score равен просто эвристике
        pq.emplace(static_cast<int>(start->heuristic(end)), start);

        while (!pq.empty()) {
            auto [f, u] = pq.top();
            pq.pop();

            // Важно: для лога и проверки пропуска используем g_score
            if (g_score.contains(u) && f > g_score[u] + static_cast<int>(u->heuristic(end))) continue;

            log.push_back({u, g_score[u], 0, parents[u]});
            if (u == end) break;

            for (Edge* edge : u->incidentEdges) {
                Vertex* v = edge->getOtherVertex(u);
                relax(u, v, v->data.weight);
            }
        }
        return log;
    }

private:
    Vertex* getVertexAt(size_t x, size_t y) const {
        if (x >= width || y >= height) return nullptr;
        return graph.getVertex(y * width + x);
    }
    Vertex* addCell(size_t x, size_t y) {
        size_t id = y * width + x;

        if (Vertex* existing = graph.getVertex(id)) return existing;
        Vertex* v = graph.addVertex(id);
        if (!v) return nullptr;

        v->data.x = static_cast<int>(x);
        v->data.y = static_cast<int>(y);

        // Восстанавливаем связи с 4 соседями
        // Проверяем каждого соседа: если он существует в графе, создаем ребро
        if (y > 0) { // Верх
            size_t up = (y - 1) * width + x;
            if (graph.getVertex(up)) graph.addEdge(id, up);
        }
        if (y < height - 1) { // Низ
            size_t down = (y + 1) * width + x;
            if (graph.getVertex(down)) graph.addEdge(id, down);
        }
        if (x > 0) { // Лево
            size_t left = y * width + (x - 1);
            if (graph.getVertex(left)) graph.addEdge(id, left);
        }
        if (x < width - 1) { // Право
            size_t right = y * width + (x + 1);
            if (graph.getVertex(right)) graph.addEdge(id, right);
        }

        return v;
    }
};

#endif