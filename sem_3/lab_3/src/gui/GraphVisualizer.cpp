#include "../../include/gui/GraphVisualizer.hpp"
#include <algorithm>

GraphVisualizer::GraphVisualizer(QWidget *parent) : QWidget(parent) {
    setMouseTracking(true);
    // Чтобы mouseMoveEvent срабатывал при зажатой кнопке
}

GraphVisualizer::~GraphVisualizer() {
    delete grid;
}

void GraphVisualizer::initGrid(int w, int h) {
    gridW = w;
    gridH = h;

    if (grid) delete grid;
    grid = new GridGraph(w, h);

    update(); // Перерисовать виджет
}

void GraphVisualizer::setCurrentTool(int terrainType) {
    currentTool = terrainType;
}

GraphVisualizer::RenderParams GraphVisualizer::getRenderParams() const {
    // Рассчитываем размер клетки, чтобы сетка влезла в окно с отступами
    float cellSize = std::min((float)(width() - 40) / gridW, (float)(height() - 40) / gridH);
    float offsetX = (width() - cellSize * gridW) / 2.0f;
    float offsetY = (height() - cellSize * gridH) / 2.0f;
    return {cellSize, offsetX, offsetY};
}

void GraphVisualizer::paintEvent(QPaintEvent *) {
    if (!grid || gridW <= 0) return;

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    auto params = getRenderParams();

    // --- 1. ФОН (СЕТКА И ЛАНДШАФТ) ---
    for (int y = 0; y < (int)gridH; ++y) {
        for (int x = 0; x < (int)gridW; ++x) {
            QRectF rect(params.offsetX + x * params.cellSize,
                        params.offsetY + y * params.cellSize,
                        params.cellSize, params.cellSize);

            int weight = grid->getVertexWeight(x, y);
            if (weight == INF) p.setBrush(Qt::black);
            else p.setBrush(getColorForTerrain(weight));

            p.setPen(QPen(Qt::lightGray, 0.5));
            p.drawRect(rect);
        }
    }

    // --- 2. ПУТЬ (ЖЕЛТАЯ ЛИНИЯ) ---
    // Рисуем под цифрами
    if ((currentAlg == Dijkstra || currentAlg == AStar) && path.size() > 1) {
        QPen pathPen(Qt::yellow, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        p.setPen(pathPen);
        for (size_t i = 0; i < path.size() - 1; ++i) {
            QPointF p1(params.offsetX + path[i].x() * params.cellSize + params.cellSize / 2,
                       params.offsetY + path[i].y() * params.cellSize + params.cellSize / 2);
            QPointF p2(params.offsetX + path[i+1].x() * params.cellSize + params.cellSize / 2,
                       params.offsetY + path[i+1].y() * params.cellSize + params.cellSize / 2);
            p.drawLine(p1, p2);
        }
    }

    // --- 3. ТЕКСТ И МАРКЕРЫ (ПОВЕРХ ПУТИ) ---
    std::map<std::pair<int, int>, VisualStep> stepMap;
    for (const auto& step : lastLog) {
        if (step.vertex) stepMap[{step.vertex->data.x, step.vertex->data.y}] = step;
    }

    for (int y = 0; y < (int)gridH; ++y) {
        for (int x = 0; x < (int)gridW; ++x) {
            QRectF rect(params.offsetX + x * params.cellSize,
                        params.offsetY + y * params.cellSize,
                        params.cellSize, params.cellSize);

            // Отрисовка текста шагов
            auto it = stepMap.find({x, y});
            if (it != stepMap.end()) {
                p.setPen(Qt::white);
                QFont font = p.font();
                font.setPixelSize(params.cellSize * 0.35);
                font.setBold(true);
                p.setFont(font);

                QString text = (currentAlg == DFS && it->second.finishTime > 0)
                    ? QString("%1/%2").arg(it->second.discoveryTime).arg(it->second.finishTime)
                    : QString("%1").arg(it->second.discoveryTime);

                p.drawText(rect, Qt::AlignHCenter, text);
            }

            // Отрисовка S и F (самый верхний слой)
            if (QPoint(x, y) == startNode) {
                p.setBrush(QColor(0, 120, 215));
                p.setPen(Qt::white);
                p.drawEllipse(rect.adjusted(4, 4, -4, -4));
                p.drawText(rect, Qt::AlignCenter, "S");
            } else if (QPoint(x, y) == finishNode) {
                p.setBrush(QColor(232, 17, 35));
                p.setPen(Qt::white);
                p.drawEllipse(rect.adjusted(4, 4, -4, -4));
                p.drawText(rect, Qt::AlignCenter, "F");
            }
        }
    }
}

void GraphVisualizer::processClick(QPointF pos) {
    if (!grid) return;
    auto params = getRenderParams();
    int x = (pos.x() - params.offsetX) / params.cellSize;
    int y = (pos.y() - params.offsetY) / params.cellSize;

    if (x >= 0 && x < gridW && y >= 0 && y < gridH) {
        // currentTool должен приходить из ComboBox (0 - Стена, 1 - Трава и т.д.)
        if (currentTool == Wall) {
            // Если выбран инструмент "Стена", мы ТОЛЬКО удаляем
            grid->removeCell(x, y);
        } else {
            // Если выбран любой другой инструмент, вершина создается (если ее нет)
            // и ей присваивается вес ландшафта
            grid->setVertexWeight(x, y, currentTool);
        }

        // Обязательно сбрасываем старые результаты поиска,
        // так как топология графа изменилась
        lastLog.clear();
        path.clear();
        update();
    }
}

void GraphVisualizer::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        processClick(event->position());
    }
    else if (event->button() == Qt::RightButton) {
        // Логика для правой кнопки
        auto params = getRenderParams();
        int x = static_cast<int>((event->position().x() - params.offsetX) / params.cellSize);
        int y = static_cast<int>((event->position().y() - params.offsetY) / params.cellSize);

        if (x >= 0 && x < gridW && y >= 0 && y < gridH) {
            setNode(QPoint(x, y));
            update();
        }
    }
}

void GraphVisualizer::setNode(const QPoint pos) {
    // Если старт не задан — ставим старт
    if (startNode == QPoint(-1, -1)) {
        startNode = pos;
    }
    // Если старт уже есть, а финиша нет — ставим финиш
    else if (finishNode == QPoint(-1, -1)) {
        if (pos != startNode) finishNode = pos;
    }
    // Если оба есть — сбрасываем и ставим старт заново
    else {
        startNode = pos;
        finishNode = QPoint(-1, -1);
    }
}

void GraphVisualizer::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        processClick(event->position());
    }
}

QColor GraphVisualizer::getColorForTerrain(int weight) {
    // Теперь switch идет по реальным весам из VertexEdge.hpp
    switch(weight) {
        case Ground:    return QColor(34, 139, 34);     // Трава
        case Sand:      return QColor(240, 230, 140);   // Песок
        case Hill:      return QColor(0, 100, 0);       // Холм
        case Water:     return QColor(135, 206, 235);   // Вода
        case Mountain:  return QColor(105, 105, 105);   // Горы
        case DeepWater: return QColor(0, 0, 139);       // Глубоководье
        case Wall:      return Qt::black;               // Стена
        default:        return Qt::white;
    }
}

void GraphVisualizer::runAlgorithm(int algIndex) {
    if (!grid || startNode == QPoint(-1, -1)) return;

    currentAlg = static_cast<AlgorithmType>(algIndex);
    path.clear();
    lastLog.clear();

    // Вызов твоих методов из GridGraph
    if (currentAlg == BFS) {
        lastLog = grid->BFS(startNode.x(), startNode.y());
    }
    else if (currentAlg == DFS) {
        if (finishNode != QPoint(-1, -1))
            lastLog = grid->DFS(startNode.x(), startNode.y(), finishNode.x(), finishNode.y());
    }
    else if (currentAlg == Dijkstra) {
        if (finishNode != QPoint(-1, -1)) {
            lastLog = grid->Dijkstra(startNode.x(), startNode.y(), finishNode.x(), finishNode.y());
            reconstructPath(); // Восстановление желтой линии
        }
    }
    else if (currentAlg == AStar) {
        if (finishNode != QPoint(-1, -1)) {
            lastLog = grid->AStar(startNode.x(), startNode.y(), finishNode.x(), finishNode.y());
            reconstructPath();
        }
    }
    update();
}

void GraphVisualizer::reconstructPath() {
    if (lastLog.empty()) return;

    // Поиск финишной точки в логе для восстановления пути по parent
    std::unordered_map<Vertex*, Vertex*> parents;
    Vertex* endVertex = nullptr;

    for (const auto& step : lastLog) {
        parents[step.vertex] = step.parent;
        if (step.vertex->data.x == finishNode.x() && step.vertex->data.y == finishNode.y()) {
            endVertex = step.vertex;
        }
    }

    Vertex* curr = endVertex;
    while (curr) {
        path.push_back(QPoint(curr->data.x, curr->data.y));
        curr = parents[curr];
    }
}

void GraphVisualizer::clearResultOnly() {
    lastLog.clear();    // Удаляем цифры
    path.clear();       // Удаляем желтую линию
    startNode = QPoint(-1, -1);  // Удаляем S
    finishNode = QPoint(-1, -1); // Удаляем F
    update();           // Перерисовываем (ландшафт останется, так как мы не трогаем grid)
}