#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "GraphVisualizer.hpp"
#include "../../cmake-build-debug/cmake-build-debug/lab_3_autogen/include/ui_MainWindow.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Слот, который примет сигнал от SizePicker
    void onSizeChosen(int w, int h);
    // Слот для кнопки "Назад"
    void onBackToMenu();

private:
    Ui::MainWindow *ui;
};

#endif