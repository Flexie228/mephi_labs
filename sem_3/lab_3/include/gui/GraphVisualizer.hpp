#ifndef GRAPH_VISUALIZER_HPP
#define GRAPH_VISUALIZER_HPP

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPoint>
#include <vector>
#include "../GridGraph.hpp"

class GraphVisualizer : public QWidget {
    Q_OBJECT
    enum AlgorithmType { BFS = 2, DFS = 3, Dijkstra = 1, AStar = 0 };
public:
    explicit GraphVisualizer(QWidget *parent = nullptr);
    ~GraphVisualizer() override;

    void initGrid(int w, int h);
    void setCurrentTool(int terrainType);
    QPoint getStartNode() const { return startNode; }
    QPoint getFinishNode() const { return finishNode; }
    void setPath(const std::vector<QPoint>& newPath) { path = newPath; update(); }
    void clearPath() { path.clear(); update(); }
    void runAlgorithm(int algIndex);

    void clearResultOnly();


protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    // Логика перевода координат
    void processClick(QPointF pos);

    int gridW = 0;
    int gridH = 0;
    int currentTool = Ground; // По умолчанию трава (вес 1)

    GridGraph* grid = nullptr; // Объект данных

    QPoint startNode = QPoint(-1, -1); // (-1,-1) означает, что точка не поставлена
    QPoint finishNode = QPoint(-1, -1);

    void setNode(QPoint pos); // Логика установки старта/финиша
    std::vector<QPoint> path; // Список точек от старта до финиша

    AlgorithmType currentAlg = AStar;
    vector<VisualStep> lastLog;

    QColor getColorForTerrain(int weight);

    struct RenderParams {
        float cellSize;
        float offsetX;
        float offsetY;
    };
    RenderParams getRenderParams() const;
    void reconstructPath();
};

#endif