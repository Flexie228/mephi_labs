#ifndef MAINWINDOW
#define MAINWINDOW

#include <QMainWindow>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QMessageBox>
#include <QTabWidget>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QInputDialog>
#include <QFont>
#include <QApplication>
#include <QProgressDialog>
#include <QSplitter>
#include <QTableWidget>
#include <QHeaderView>
#include <QComboBox>
#include <chrono>
#include "DatabaseStudent.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void addStudent();
    void searchStudent();
    void removeStudent();
    void generateStudents();
    void removeSelectedFromResults();
    void searchInResults();

private:
    void setupUI();
    static void setupSearchTab(QWidget* tab, const QString& label, QWidget* input, QPushButton* button);

    void showSearchResults(const std::vector<Student>& students, const std::vector<std::string>& ids, const QString& title, qint64 searchTime);
    void refreshStudentList() const;
    void refreshCache() const;
    void displayResultsInTable(const std::vector<Student>& students, const std::vector<std::string>& ids);

    DataBaseStudent database;
    QTableWidget *studentTable;
    QTableWidget *cacheTable;
    QTableWidget *resultsTable;
    QLineEdit *searchInResultsEdit;
    QComboBox *searchInResultsField;
    QLabel *resultsLabel;
    std::vector<Student> currentResults;
    std::vector<std::string> currentResultIDs;
};

#endif