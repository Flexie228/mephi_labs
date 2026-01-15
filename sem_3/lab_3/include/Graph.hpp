#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "VertexEdge.hpp"
#include "PairWorks.hpp"
#include "VisualizationInfo.hpp"
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <algorithm>
#include <ranges>
#include <utility>

using namespace std;

class Graph {
    unordered_map<size_t, unique_ptr<Vertex>> vertices;
    unordered_map<pair<size_t, size_t>, unique_ptr<Edge>, PairHash, PairEqual> edges;

public:
    Graph() = default;
    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;
    Graph(Graph&&) = default;
    Graph& operator=(Graph&&) = default;

    // Методы для работы с вершинами
    Vertex* addVertex(size_t id) {
        if (vertices.contains(id)) return nullptr;

        auto* newVertexPtr = new Vertex(id);
        vertices[id] = std::move(unique_ptr<Vertex>(newVertexPtr));
        return newVertexPtr;
    }

    bool removeVertex(size_t id) {
        if (!vertices.contains(id)) return false;

        Vertex* vPtr = vertices[id].get();

        auto incidentEdgesCopy = vPtr->incidentEdges;
        for (Edge* edge : incidentEdgesCopy) {
            removeEdge(edge);
        }

        vertices.erase(id);
        return true;
    }

    [[nodiscard]] Vertex* getVertex(size_t id) const {
        const auto it = vertices.find(id);
        return (vertices.contains(id)) ? it->second.get() : nullptr;
    }

    [[nodiscard]] vector<Vertex*> getAllVertices() const {
        vector<Vertex*> result;
        result.reserve(vertices.size());
        for (const auto &vertex: vertices | views::values) {
            result.push_back(vertex.get());
        }
        return result;
    }

    [[nodiscard]] size_t getVertexCount() const { return vertices.size(); }

    // Методы для работы с ребрами
    Edge* addEdge(size_t id1, size_t id2) {
        if (id1 == id2) return nullptr;
        Vertex* v1 = getVertex(id1);
        Vertex* v2 = getVertex(id2);
        if (!v1 || !v2) return nullptr;

        pair<size_t, size_t> edgeKey = make_pair(min(id1, id2), max(id1, id2));

        if (edges.contains(edgeKey)) return edges[edgeKey].get();
        // Создаем новое ребро
        auto* newEdgePtr = new Edge(v1, v2);
        edges[edgeKey] = std::move(unique_ptr<Edge>(newEdgePtr));

        v1->addIncidentEdge(newEdgePtr);
        v2->addIncidentEdge(newEdgePtr);
        return newEdgePtr;
    }

    Edge* addEdge(const Vertex* v1, const Vertex* v2) {
        if (!v1 || !v2) return nullptr;
        return addEdge(v1->data.id, v2->data.id);
    }

    bool removeEdge(size_t id1, size_t id2) {
        if (id1 == id2) return false;

        pair<size_t, size_t> edgeKey = make_pair(min(id1, id2), max(id1, id2));
        auto edgeIt = edges.find(edgeKey);
        if (edgeIt == edges.end()) return false;

        Edge* edge = edgeIt->second.get();
        Vertex* v1 = edge->vertex1;
        Vertex* v2 = edge->vertex2;

        // Удаляем из массивов инцидентных ребер смежных вершин
        if (v1) {
            auto& edges1 = v1->incidentEdges;
            edges1.erase(ranges::remove(edges1, edge).begin(), edges1.end());
        }

        if (v2) {
            auto& edges2 = v2->incidentEdges;
            edges2.erase(ranges::remove(edges2, edge).begin(), edges2.end());
        }

        edges.erase(edgeIt);
        return true;
    }

    bool removeEdge(const Vertex* v1, const Vertex* v2) {
        if (!v1 || !v2) return false;
        return removeEdge(v1->data.id, v2->data.id);
    }

    bool removeEdge(const Edge* edge) {
        if (!edge) return false;
        if (!edge->vertex1 || !edge->vertex2) return false;
        return removeEdge(edge->vertex1->data.id, edge->vertex2->data.id);
    }

    [[nodiscard]] Edge* findEdge(size_t id1, size_t id2) const {
        if (id1 == id2) return nullptr;

        pair<size_t, size_t> edgeKey = make_pair(min(id1, id2), max(id1, id2));
        auto it = edges.find(edgeKey);
        return (it != edges.end()) ? it->second.get() : nullptr;
    }

    [[nodiscard]] Edge* findEdge(const Vertex* v1, const Vertex* v2) const {
        if (!v1 || !v2) return nullptr;
        return findEdge(v1->data.id, v2->data.id);
    }

    [[nodiscard]] vector<Edge*> getAllEdges() const {
        vector<Edge*> result;
        result.reserve(edges.size());
        for (const auto &edge: edges | views::values) {
            result.push_back(edge.get());
        }
        return result;
    }

    [[nodiscard]] size_t getEdgeCount() const { return edges.size(); }

    vector<VisualStep> BFS(Vertex* start) const {
        if (!start || !vertices.contains(start->data.id)) return {};

        queue<Vertex*> q;
        unordered_map<Vertex*, int> distances;
        unordered_map<Vertex*, Vertex*> parents;  // (ключ: вершина, значение: вершина, из которой в нее пришли)
        vector<VisualStep> log;

        q.push(start);
        distances[start] = 0;
        parents[start] = nullptr;

        while (!q.empty()) {
            Vertex* current = q.front();
            q.pop();

            log.push_back({current, distances[current], 0, parents[current]});

            for (Edge* edge : current->incidentEdges) {
                Vertex* neighbor = edge->getOtherVertex(current);
                if (!distances.contains(neighbor)) {
                    distances[neighbor] = distances[current] + 1;
                    parents[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }
        return log;
    }

    vector<VisualStep> DFS(Vertex* start, const Vertex* end) const {
        if (!start || !end) return {};

        vector<VisualStep> log;
        unordered_set<Vertex*> visited;
        unordered_map<Vertex*, int> entryTimes; // Чтобы помнить время входа при записи выхода
        int timer = 0;

        // Стек будет хранить пару: (вершина, родитель)
        // Чтобы реализовать время выхода, мы будем использовать вспомогательный set "processed"
        unordered_set<Vertex*> processed;
        stack<pair<Vertex*, Vertex*>> s;

        auto pushComponent = [&](Vertex* componentStart) { s.emplace(componentStart, nullptr); };
        pushComponent(start);

        while (true) {
            if (s.empty()) {
                // Если стек пуст, ищем новую КСС
                bool foundNew = false;
                for (auto &vPtr: vertices | views::values) {
                    if (!visited.contains(vPtr.get())) {
                        pushComponent(vPtr.get());
                        foundNew = true;
                        break;
                    }
                }
                if (!foundNew) break; // Обошли всё - конец
            }

            auto [current, parent] = s.top();

            if (!visited.contains(current)) {
                // --- СОБЫТИЕ: ВХОД ---
                visited.insert(current);
                int inTime = ++timer;
                entryTimes[current] = inTime;
                log.push_back({current, inTime, 0, parent});

                if (current == end) return log;

                // Не выкидываем current из стека
                // Он останется внизу, чтобы мы могли "выйти" из него в другую КСС позже.
                for (Edge* edge : current->incidentEdges) {
                    Vertex* neighbor = edge->getOtherVertex(current);
                    if (!visited.contains(neighbor))
                        s.emplace(neighbor, current);

                }
            } else {
                // --- СОБЫТИЕ: ВЫХОД ---
                // Если вершина уже в visited, и она оказывается на вершине стека
                // То мы вернулись в нее
                if (!processed.contains(current)) {
                    log.push_back({current, entryTimes[current], ++timer, parent});
                    processed.insert(current);
                }
                s.pop();
            }
        }

        return log;
    }

};

#endif