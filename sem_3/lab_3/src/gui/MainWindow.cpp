#include "../../include/gui/MainWindow.hpp"
#include "../../include/gui/SizePicker.hpp"
#include "../../include/gui/GraphVisualizer.hpp"
#include <QPushButton>
#include <QComboBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(qobject_cast<SizePicker*>(ui->sizePicker), &SizePicker::sizeSelected, this, &MainWindow::onSizeChosen);

    connect(ui->backToMenuBtn, &QPushButton::clicked, this, &MainWindow::onBackToMenu);

    connect(ui->ToolComboBox, &QComboBox::currentIndexChanged, [this](int index){
    int weight = 1;
    switch(index) {
        case 0: weight = Ground; break; // 1
        case 1: weight = Sand;   break; // 2
        case 2: weight = Hill;   break; // 3
        case 3: weight = Water;  break; // 4
        case 4: weight = Mountain; break; // 5
        case 5: weight = DeepWater; break; // 6
        case 6: weight = Wall; break; // INF
        default: weight = Ground;
    }
    ui->graphVisualizer->setCurrentTool(weight);
});
    connect(ui->runAlgoBtn, &QPushButton::clicked, this, [this]() {
    // Получаем индекс алгоритма: 0-BFS, 1-DFS, 2-Dijkstra, 3-AStar
    int algIndex = ui->AlgoComboBox->currentIndex();
    ui->graphVisualizer->runAlgorithm(algIndex);
});

    connect(ui->clearPathBtn, &QPushButton::clicked, this, [this]() {
    ui->graphVisualizer->clearResultOnly();
});
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onSizeChosen(int w, int h) {
    qDebug() << "Selected size:" << w << "x" << h;
    ui->graphVisualizer->initGrid(w, h);
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::onBackToMenu() {
    ui->stackedWidget->setCurrentIndex(0);
}