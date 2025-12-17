#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <memory>
#include <QString>

namespace Ui { class MainWindow; }

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_selectFileButton_clicked();
    void on_compressButton_clicked();
    void on_decompressButton_clicked();

private:
    Ui::MainWindow *ui;
    QString currentFilePath;

    // Методы для операций сжатия/распаковки
    bool compressHuffman(const QString& inputPath, const QString& outputPath);
    bool compressLZW(const QString& inputPath, const QString& outputPath);
    bool compressRLE(const QString& inputPath, const QString& outputPath);

    bool decompressHuffman(const QString& inputPath, const QString& outputPath);
    bool decompressLZW(const QString& inputPath, const QString& outputPath);
    bool decompressRLE(const QString& inputPath, const QString& outputPath);

    // Вспомогательные
    void updateProgress(int value);
    void showStats(const QString& stats);
    void showError(const QString& error);
    void clearError();
    QString getOutputPath(bool forCompression);
    QString getAlgorithmExtension(const QString& algorithm);
};

#endif