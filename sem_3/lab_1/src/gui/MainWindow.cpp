#include "gui/MainWindow.hpp"
#include "../cmake-build-debug/CompressionApp_autogen/cmake-build-debug/CompressionApp_autogen/include/ui_MainWindow.h"
#include "Compression/HuffmanCompressor.hpp"
#include "Compression/LZWCompressor.hpp"
#include "Compression/RLECompressor.hpp"
#include "Streams/BinaryStream.hpp"
#include "Streams/ReadStream.hpp"
#include "Streams/WriteStream.hpp"
#include <QFileDialog>
#include <QFileInfo>
#include <QTimer>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentFilePath("")
{
    ui->setupUi(this);
    ui->algorithmComboBox->addItems({"Huffman", "LZW", "RLE"});
    ui->progressBar->setValue(0);
    clearError();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectFileButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select File", "", "All Files (*.*)");
    if (!filePath.isEmpty()) {
        currentFilePath = filePath;
        QFileInfo info(filePath);
        ui->statsLabel->setText(QString("Selected: %1\nSize: %2 bytes").arg(info.fileName()).arg(info.size()));
        clearError();
        ui->progressBar->setValue(0);
    }
}

void MainWindow::on_compressButton_clicked()
{
    if (currentFilePath.isEmpty()) {
        showError("Please select a file first!");
        return;
    }

    QString algorithm = ui->algorithmComboBox->currentText();
    QString outputPath = getOutputPath(true);

    if (outputPath.isEmpty()) {
        showError("Failed to determine output path!");
        return;
    }

    QFileInfo inputInfo(currentFilePath);
    QFileInfo outputInfo(outputPath);
    if (outputInfo.absoluteFilePath() == inputInfo.absoluteFilePath()) {
        showError("Output file would overwrite input file!");
        return;
    }

    ui->progressBar->setValue(20);
    bool success = false;

    try {
        if (algorithm == "Huffman") success = compressHuffman(currentFilePath, outputPath);
        else if (algorithm == "LZW") success = compressLZW(currentFilePath, outputPath);
        else if (algorithm == "RLE") success = compressRLE(currentFilePath, outputPath);

        if (success) {
            ui->progressBar->setValue(100);
            clearError();
        }
    } catch (const std::exception& e) {
        showError(QString("Compression failed: %1").arg(e.what()));
        ui->progressBar->setValue(0);
    }
}

void MainWindow::on_decompressButton_clicked()
{
    if (currentFilePath.isEmpty()) {
        showError("Please select a file first!");
        return;
    }

    QString algorithm = ui->algorithmComboBox->currentText();
    QString outputPath = getOutputPath(false);

    if (outputPath.isEmpty()) {
        showError("Failed to determine output path!");
        return;
    }

    ui->progressBar->setValue(20);
    bool success = false;

    try {
        if (algorithm == "Huffman") success = decompressHuffman(currentFilePath, outputPath);
        else if (algorithm == "LZW") success = decompressLZW(currentFilePath, outputPath);
        else if (algorithm == "RLE") success = decompressRLE(currentFilePath, outputPath);

        if (success) {
            ui->progressBar->setValue(100);
            clearError();
        }
    } catch (const std::exception& e) {
        showError(QString("Decompression failed: %1").arg(e.what()));
        ui->progressBar->setValue(0);
    }
}

bool MainWindow::compressHuffman(const QString& inputPath, const QString& outputPath)
{
    ReadStream input(inputPath.toStdString());
    WriteStream output(outputPath.toStdString());

    input.Open();
    output.Open();

    HuffmanCompressor compressor;
    compressor.Compress(input, output);

    input.Close();
    output.Close();

    auto stats = compressor.GetStats();
    ui->statsLabel->setText(QString("Huffman compression complete!\n"
                                   "Original: %1 bytes\n"
                                   "Compressed: %2 bytes\n"
                                   "Ratio: %3%\n"
                                   "Time: %4s")
        .arg(stats.originalSize)
        .arg(stats.compressedSize)
        .arg(stats.ratio, 0, 'f', 2)
        .arg(stats.compressionTime, 0, 'f', 3));

    return true;
}

bool MainWindow::compressLZW(const QString& inputPath, const QString& outputPath)
{
    BinaryStream input(inputPath.toStdString());
    BinaryStream output(outputPath.toStdString());

    input.Open();
    output.Open();

    LZWCompressor compressor;
    compressor.Compress(input, output);

    input.Close();
    output.Close();

    auto stats = compressor.GetStats();
    ui->statsLabel->setText(QString("LZW compression complete!\n"
                                   "Original: %1 bytes\n"
                                   "Compressed: %2 bytes\n"
                                   "Ratio: %3%\n"
                                   "Time: %4s")
        .arg(stats.originalSize)
        .arg(stats.compressedSize)
        .arg(stats.ratio, 0, 'f', 2)
        .arg(stats.compressionTime, 0, 'f', 3));

    return true;
}

bool MainWindow::compressRLE(const QString& inputPath, const QString& outputPath)
{
    BinaryStream input(inputPath.toStdString());
    BinaryStream output(outputPath.toStdString());

    input.Open();
    output.Open();

    RLECompressor compressor;
    compressor.Compress(input, output);

    input.Close();
    output.Close();

    auto stats = compressor.GetStats();
    ui->statsLabel->setText(QString("RLE compression complete!\n"
                                   "Original: %1 bytes\n"
                                   "Compressed: %2 bytes\n"
                                   "Ratio: %3%\n"
                                   "Time: %4s")
        .arg(stats.originalSize)
        .arg(stats.compressedSize)
        .arg(stats.ratio, 0, 'f', 2)
        .arg(stats.compressionTime, 0, 'f', 3));

    return true;
}

bool MainWindow::decompressHuffman(const QString& inputPath, const QString& outputPath)
{
    ReadStream input(inputPath.toStdString());
    WriteStream output(outputPath.toStdString());

    input.Open();
    output.Open();

    HuffmanCompressor compressor;
    compressor.Decompress(input, output);

    input.Close();
    output.Close();

    auto stats = compressor.GetStats();
    ui->statsLabel->setText(QString("Huffman decompression complete!\n"
                                   "Compressed: %1 bytes\n"
                                   "Decompressed: %2 bytes\n"
                                   "Time: %3s")
        .arg(stats.originalSize)
        .arg(stats.compressedSize)
        .arg(stats.compressionTime, 0, 'f', 3));

    return true;
}

bool MainWindow::decompressLZW(const QString& inputPath, const QString& outputPath)
{
    BinaryStream input(inputPath.toStdString());
    BinaryStream output(outputPath.toStdString());

    input.Open();
    output.Open();

    LZWCompressor compressor;
    compressor.Decompress(input, output);

    input.Close();
    output.Close();

    auto stats = compressor.GetStats();
    ui->statsLabel->setText(QString("LZW decompression complete!\n"
                                   "Compressed: %1 bytes\n"
                                   "Decompressed: %2 bytes\n"
                                   "Time: %3s")
        .arg(stats.originalSize)
        .arg(stats.compressedSize)
        .arg(stats.compressionTime, 0, 'f', 3));

    return true;
}

bool MainWindow::decompressRLE(const QString& inputPath, const QString& outputPath)
{
    BinaryStream input(inputPath.toStdString());
    BinaryStream output(outputPath.toStdString());

    input.Open();
    output.Open();

    RLECompressor compressor;
    compressor.Decompress(input, output);

    input.Close();
    output.Close();

    auto stats = compressor.GetStats();
    ui->statsLabel->setText(QString("RLE decompression complete!\n"
                                   "Compressed: %1 bytes\n"
                                   "Decompressed: %2 bytes\n"
                                   "Time: %3s")
        .arg(stats.originalSize)
        .arg(stats.compressedSize)
        .arg(stats.compressionTime, 0, 'f', 3));

    return true;
}

QString MainWindow::getOutputPath(bool forCompression)
{
    if (currentFilePath.isEmpty()) return "";

    QFileInfo fileInfo(currentFilePath);
    QString baseName = fileInfo.completeBaseName();
    QString path = fileInfo.path();
    QString suffix = fileInfo.suffix();
    QString algorithm = ui->algorithmComboBox->currentText().toLower();

    if (forCompression) {
        return QString("%1/%2.%3").arg(path, fileInfo.fileName(), algorithm);
    } else {
        QString fileName = fileInfo.fileName();

        QString expectedSuffix = "." + algorithm;
        if (fileName.endsWith(expectedSuffix)) {
            fileName = fileName.left(fileName.length() - expectedSuffix.length());
        }

        if (!suffix.isEmpty() && fileName.endsWith("." + suffix)) {
            fileName = fileName.left(fileName.length() - suffix.length() - 1) + "_decompressed." + suffix;
        } else {
            fileName = baseName + "_decompressed." + suffix;
        }

        return QString("%1/%2").arg(path, fileName);
    }
}

void MainWindow::updateProgress(int value)
{
    ui->progressBar->setValue(value);
    QCoreApplication::processEvents();
}

void MainWindow::showStats(const QString& stats)
{
    ui->statsLabel->setText(stats);
}

void MainWindow::showError(const QString& error)
{
    ui->expeptionLabel->setText(error);
    ui->expeptionLabel->setStyleSheet("color: #d4380d; background-color: #fff2e8; border: 1px solid #ffbb96; padding: 5px;");
    QTimer::singleShot(5000, this, &MainWindow::clearError);
}

void MainWindow::clearError()
{
    ui->expeptionLabel->clear();
    ui->expeptionLabel->setStyleSheet("");
}