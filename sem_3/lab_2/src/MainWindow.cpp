#include "../include/MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
}

void MainWindow::setupUI() {
    setWindowTitle("База данных студентов");
    setMinimumSize(1800, 1000);

    QFont font = QApplication::font();
    font.setPointSize(12);
    QApplication::setFont(font);

    auto *centralWidget = new QWidget(this); // NOLINT
    setCentralWidget(centralWidget);

    auto *mainLayout = new QVBoxLayout(centralWidget); // NOLINT

    // Панель управления
    auto *controlLayout = new QHBoxLayout(); // NOLINT
    auto *addBtn = new QPushButton("Добавить студента", centralWidget); // NOLINT
    auto *searchBtn = new QPushButton("Поиск", centralWidget); // NOLINT
    auto *removeBtn = new QPushButton("Удалить студента", centralWidget); // NOLINT
    auto *generateBtn = new QPushButton("Сгенерировать студентов", centralWidget); // NOLINT

    QFont buttonFont = addBtn->font();
    buttonFont.setPointSize(11);

    addBtn->setFont(buttonFont);
    searchBtn->setFont(buttonFont);
    removeBtn->setFont(buttonFont);
    generateBtn->setFont(buttonFont);

    addBtn->setMinimumHeight(40);
    searchBtn->setMinimumHeight(40);
    removeBtn->setMinimumHeight(40);
    generateBtn->setMinimumHeight(40);

    controlLayout->addWidget(addBtn);
    controlLayout->addWidget(searchBtn);
    controlLayout->addWidget(removeBtn);
    controlLayout->addWidget(generateBtn);
    controlLayout->addStretch();

    // Основной разделитель: студенты + кэш | результаты поиска
    auto *mainSplitter = new QSplitter(Qt::Horizontal, centralWidget); // NOLINT

    // Левая часть: студенты и кэш
    auto *leftWidget = new QWidget(mainSplitter); // NOLINT
    auto *leftLayout = new QVBoxLayout(leftWidget); // NOLINT

    // Таблица студентов
    auto *studentsLabel = new QLabel("Все студенты:", leftWidget); // NOLINT
    QFont labelFont = studentsLabel->font();
    labelFont.setPointSize(14);
    labelFont.setBold(true);
    studentsLabel->setFont(labelFont);
    leftLayout->addWidget(studentsLabel);

    studentTable = new QTableWidget();
    studentTable->setColumnCount(5);
    studentTable->setHorizontalHeaderLabels({"ID", "Имя", "Фамилия", "Возраст", "Телефон"});
    // Делаем поле ID шире, а поле Возраст уже
    studentTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    studentTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    studentTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    studentTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    studentTable->setColumnWidth(0, 300);
    studentTable->setColumnWidth(1, 100);
    studentTable->setColumnWidth(3, 80);
    studentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    studentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    leftLayout->addWidget(studentTable);

    // Кэш - таблица
    auto *cacheLabel = new QLabel("Кэш:", leftWidget); // NOLINT
    cacheLabel->setFont(labelFont);
    leftLayout->addWidget(cacheLabel);

    cacheTable = new QTableWidget();
    cacheTable->setColumnCount(5);
    cacheTable->setHorizontalHeaderLabels({"ID", "Имя", "Фамилия", "Возраст", "Телефон"});
    cacheTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    cacheTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    cacheTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    cacheTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    cacheTable->setColumnWidth(0, 300);
    cacheTable->setColumnWidth(1, 100);
    cacheTable->setColumnWidth(3, 80);
    cacheTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    cacheTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    leftLayout->addWidget(cacheTable);

    // Правая часть: результаты поиска
    auto *rightWidget = new QWidget(mainSplitter); // NOLINT
    auto *rightLayout = new QVBoxLayout(rightWidget); // NOLINT

    resultsLabel = new QLabel("Результаты поиска:");
    resultsLabel->setFont(labelFont);
    rightLayout->addWidget(resultsLabel);

    // Панель управления для результатов
    auto *resultsControlLayout = new QHBoxLayout(); // NOLINT
    searchInResultsEdit = new QLineEdit();
    searchInResultsEdit->setPlaceholderText("Поиск в результатах...");

    searchInResultsField = new QComboBox();
    searchInResultsField->addItem("Все поля");
    searchInResultsField->addItem("Имя");
    searchInResultsField->addItem("Фамилия");
    searchInResultsField->addItem("Телефон");
    searchInResultsField->addItem("Возраст");

    auto *searchInResultsBtn = new QPushButton("Найти в результатах", rightWidget); // NOLINT
    auto *removeFromResultsBtn = new QPushButton("Удалить выбранных", rightWidget); // NOLINT

    searchInResultsBtn->setFont(buttonFont);
    removeFromResultsBtn->setFont(buttonFont);

    resultsControlLayout->addWidget(new QLabel("Поиск:"));
    resultsControlLayout->addWidget(searchInResultsEdit);
    resultsControlLayout->addWidget(new QLabel("в:"));
    resultsControlLayout->addWidget(searchInResultsField);
    resultsControlLayout->addWidget(searchInResultsBtn);
    resultsControlLayout->addWidget(removeFromResultsBtn);
    resultsControlLayout->addStretch();

    rightLayout->addLayout(resultsControlLayout);

    // Таблица результатов
    resultsTable = new QTableWidget();
    resultsTable->setColumnCount(5);
    resultsTable->setHorizontalHeaderLabels({"ID", "Имя", "Фамилия", "Возраст", "Телефон"});
    resultsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    resultsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    resultsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    resultsTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    cacheTable->setColumnWidth(0, 300);
    cacheTable->setColumnWidth(1, 100);
    resultsTable->setColumnWidth(3, 80);
    resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultsTable->setSelectionMode(QAbstractItemView::MultiSelection);
    resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    rightLayout->addWidget(resultsTable);

    mainSplitter->addWidget(leftWidget);
    mainSplitter->addWidget(rightWidget);
    mainSplitter->setSizes(QList<int>() << 1000 << 800);

    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(mainSplitter);

    // Подключение кнопок
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::addStudent);
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::searchStudent);
    connect(removeBtn, &QPushButton::clicked, this, &MainWindow::removeStudent);
    connect(generateBtn, &QPushButton::clicked, this, &MainWindow::generateStudents);
    connect(searchInResultsBtn, &QPushButton::clicked, this, &MainWindow::searchInResults);
    connect(removeFromResultsBtn, &QPushButton::clicked, this, &MainWindow::removeSelectedFromResults);

    refreshStudentList();
    refreshCache();
}

void MainWindow::displayResultsInTable(const std::vector<Student>& students, const std::vector<std::string>& ids) {
    resultsTable->setRowCount(0);
    currentResults.clear();
    currentResultIDs.clear();

    for (size_t i = 0; i < students.size(); ++i) {
        const auto& student = students[i];
        const auto& id = ids[i];

        currentResults.push_back(student);
        currentResultIDs.push_back(id);

        int row = resultsTable->rowCount();
        resultsTable->insertRow(row);

        resultsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(id)));
        resultsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(student.name)));
        resultsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(student.surname)));
        resultsTable->setItem(row, 3, new QTableWidgetItem(QString::number(student.age)));
        resultsTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(student.phone.number)));
    }

    // ОБНОВЛЯЕМ КЭШ
    refreshCache();
}

void MainWindow::searchInResults() {
    const QString searchText = searchInResultsEdit->text().toLower();
    if (searchText.isEmpty() || currentResults.empty()) return;

    std::vector<Student> filteredResults;
    std::vector<std::string> filteredIDs;
    const QString field = searchInResultsField->currentText();

    for (size_t i = 0; i < currentResults.size(); ++i) {
        const auto& student = currentResults[i];
        bool match = false;

        if (field == "Все поля") {
            match = QString::fromStdString(student.name).toLower().contains(searchText) ||
                    QString::fromStdString(student.surname).toLower().contains(searchText) ||
                    QString::fromStdString(student.phone.number).contains(searchText) ||
                    QString::number(student.age).contains(searchText);
        }
        else if (field == "Имя") {
            match = QString::fromStdString(student.name).toLower().contains(searchText);
        }
        else if (field == "Фамилия") {
            match = QString::fromStdString(student.surname).toLower().contains(searchText);
        }
        else if (field == "Телефон") {
            match = QString::fromStdString(student.phone.number).contains(searchText);
        }
        else if (field == "Возраст") { // ДОБАВЛЕНО: поиск по возрасту
            match = QString::number(student.age).contains(searchText);
        }

        if (match) {
            filteredResults.push_back(student);
            filteredIDs.push_back(currentResultIDs[i]);
        }
    }

    displayResultsInTable(filteredResults, filteredIDs);
}

void MainWindow::removeSelectedFromResults() {
    QList<QTableWidgetItem*> selectedItems = resultsTable->selectedItems();
    if (selectedItems.isEmpty()) return;

    // Собираем уникальные строки для удаления
    QSet<int> rowsToRemove;
    for (const QTableWidgetItem* item : selectedItems) {
        rowsToRemove.insert(item->row());
    }

    // Удаляем из базы данных
    for (const int row : rowsToRemove) {
        if (row < currentResultIDs.size()) {
            std::string idToRemove = currentResultIDs[row];
            database.removeStudent(idToRemove);
        }
    }

    // Обновляем интерфейс
    refreshStudentList();
    refreshCache(); // ОБНОВЛЯЕМ КЭШ

    // Убираем удаленных из результатов
    std::vector<Student> newResults;
    std::vector<std::string> newResultIDs;

    for (int i = 0; i < currentResults.size(); ++i) {
        if (!rowsToRemove.contains(i)) {
            newResults.push_back(currentResults[i]);
            newResultIDs.push_back(currentResultIDs[i]);
        }
    }

    currentResults = newResults;
    currentResultIDs = newResultIDs;
    displayResultsInTable(currentResults, currentResultIDs);

    QMessageBox::information(this, "Успех", QString("Удалено %1 студентов").arg(rowsToRemove.size()));
}

void MainWindow::showSearchResults(const std::vector<Student>& students, const std::vector<std::string>& ids, const QString& title, qint64 searchTime) {
    displayResultsInTable(students, ids);

    // Обновляем заголовок с временем поиска
    QString timeText;
    if (searchTime < 1000) {
        timeText = QString("%1 мкс").arg(searchTime);
    } else {
        timeText = QString("%1 мс").arg(static_cast<double>(searchTime) / 1000.0, 0, 'f', 2);
    }

    resultsLabel->setText(QString("Результаты поиска (%1 найдено, время: %2)").arg(students.size()).arg(timeText));

    // ОБНОВЛЯЕМ КЭШ ПОСЛЕ ПОИСКА
    refreshCache();
}

void MainWindow::refreshStudentList() const {
    studentTable->setRowCount(0);
    auto students = database.getAllStudents();

    for (const auto& [id, student] : students) {
        const int row = studentTable->rowCount();
        studentTable->insertRow(row);

        studentTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(id)));
        studentTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(student.name)));
        studentTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(student.surname)));
        studentTable->setItem(row, 3, new QTableWidgetItem(QString::number(student.age)));
        studentTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(student.phone.number)));
    }

    // ОБНОВЛЯЕМ КЭШ
    refreshCache();
}

void MainWindow::refreshCache() const {
    cacheTable->setRowCount(0);
    auto cacheContents = database.getCacheContents();

    for (const auto& [id, student] : cacheContents) {
        const int row = cacheTable->rowCount();
        cacheTable->insertRow(row);

        cacheTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(id)));
        cacheTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(student.name)));
        cacheTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(student.surname)));
        cacheTable->setItem(row, 3, new QTableWidgetItem(QString::number(student.age)));
        cacheTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(student.phone.number)));
    }
}

void MainWindow::generateStudents() {
    if (database.size() > 0) {
        QMessageBox::warning(this, "Ошибка", "Невозможно сгенерировать студентов: база данных не пуста!");
        return;
    }

    bool ok;
    const int count = QInputDialog::getInt(this, "Генерация студентов",
                                   "Количество студентов (до 1 млн):",
                                   1000, 1, 1000000, 100, &ok);
    if (!ok) return;

    QProgressDialog progress("Генерация студентов...", "Отмена", 0, count, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.setFont(QFont("Arial", 12));

    for (int i = 0; i < count; ++i) {
        if (progress.wasCanceled())
            break;

        Student student = Student::generate();
        database.addStudent(std::move(student));

        progress.setValue(i + 1);

        if (i % 1000 == 0) {
            QApplication::processEvents();
        }
    }

    progress.setValue(count);

    refreshStudentList();
    refreshCache(); // ОБНОВЛЯЕМ КЭШ

    QMessageBox::information(this, "Успех",
        QString("Сгенерировано %1 студентов").arg(count));
}

void MainWindow::addStudent() {
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить студента");
    dialog.setModal(true);
    dialog.resize(500, 400);

    auto *form = new QFormLayout(&dialog); // NOLINT

    auto *nameEdit = new QLineEdit(&dialog); // NOLINT
    auto *surnameEdit = new QLineEdit(&dialog); // NOLINT
    auto *phoneEdit = new QLineEdit(&dialog); // NOLINT
    auto *ageSpin = new QSpinBox(&dialog); // NOLINT
    ageSpin->setRange(1, 100);

    QFont inputFont = nameEdit->font();
    inputFont.setPointSize(12);
    nameEdit->setFont(inputFont);
    surnameEdit->setFont(inputFont);
    phoneEdit->setFont(inputFont);
    ageSpin->setFont(inputFont);

    form->addRow("Имя:", nameEdit);
    form->addRow("Фамилия:", surnameEdit);
    form->addRow("Телефон:", phoneEdit);
    form->addRow("Возраст:", ageSpin);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog); // NOLINT
    buttons->setFont(inputFont);
    form->addRow(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        try {
            Student student(nameEdit->text().toStdString(),
                          surnameEdit->text().toStdString(),
                          ageSpin->value(),
                          Phone(phoneEdit->text().toStdString()));

            const std::string id = database.addStudent(std::move(student));
            QMessageBox::information(this, "Успех",
                QString("Студент добавлен с ID: %1").arg(QString::fromStdString(id)));
            refreshStudentList();
            refreshCache(); // ОБНОВЛЯЕМ КЭШ ПОСЛЕ ДОБАВЛЕНИЯ
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Ошибка", e.what());
        }
    }
}

void MainWindow::searchStudent() {
    QDialog dialog(this);
    dialog.setWindowTitle("Поиск студента");
    dialog.setModal(true);
    dialog.resize(600, 500);

    auto *layout = new QVBoxLayout(&dialog);

    auto *searchTabs = new QTabWidget();

    // вкладки поиска
    auto *idTab = new QWidget();
    auto nameTab = new QWidget();
    auto *surnameTab = new QWidget();
    auto phoneTab = new QWidget();
    auto ageRangeTab = new QWidget();

    // Поиск по ID
    auto *idEdit = new QLineEdit();
    auto *searchByIdBtn = new QPushButton("Найти по ID");
    setupSearchTab(idTab, "ID:", idEdit, searchByIdBtn);

    // Поиск по имени
    auto *nameEdit = new QLineEdit();
    auto *searchByNameBtn = new QPushButton("Найти по имени");
    setupSearchTab(nameTab, "Имя:", nameEdit, searchByNameBtn);

    // Поиск по фамилии
    auto *surnameEdit = new QLineEdit();
    auto *searchBySurnameBtn = new QPushButton("Найти по фамилии");
    setupSearchTab(surnameTab, "Фамилия:", surnameEdit, searchBySurnameBtn);

    // Поиск по телефону
    auto *phoneEdit = new QLineEdit();
    auto *searchByPhoneBtn = new QPushButton("Найти по телефону");
    setupSearchTab(phoneTab, "Телефон:", phoneEdit, searchByPhoneBtn);

    // Поиск по диапазону возрастов
    auto *ageRangeWidget = new QWidget();
    auto *ageRangeLayout = new QHBoxLayout(ageRangeWidget);
    auto *minAgeSpin = new QSpinBox();
    auto *maxAgeSpin = new QSpinBox();
    minAgeSpin->setRange(1, 100);
    maxAgeSpin->setRange(1, 100);
    minAgeSpin->setValue(18);
    maxAgeSpin->setValue(25);
    ageRangeLayout->addWidget(minAgeSpin);
    ageRangeLayout->addWidget(new QLabel("до"));
    ageRangeLayout->addWidget(maxAgeSpin);
    auto *searchByAgeRangeBtn = new QPushButton("Найти по диапазону");
    setupSearchTab(ageRangeTab, "Диапазон возрастов:", ageRangeWidget, searchByAgeRangeBtn);

    searchTabs->addTab(idTab, "По ID");
    searchTabs->addTab(nameTab, "По имени");
    searchTabs->addTab(surnameTab, "По фамилии");
    searchTabs->addTab(phoneTab, "По телефону");
    searchTabs->addTab(ageRangeTab, "По возрасту");

    layout->addWidget(searchTabs);

    // Кнопки поиска - БЫСТРЫЙ ПОИСК ЧЕРЕЗ ИНДЕКСЫ
    connect(searchByIdBtn, &QPushButton::clicked, [&, idEdit]() {
        auto start = std::chrono::high_resolution_clock::now();
        auto students = database.findByID(idEdit->text().toStdString());
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::vector<std::string> ids;
        if (!students.empty()) {
            ids.push_back(idEdit->text().toStdString());
        }
        showSearchResults(students, ids, "Результаты поиска по ID", duration.count());
    });

    connect(searchByNameBtn, &QPushButton::clicked, [&, nameEdit]() {
        auto start = std::chrono::high_resolution_clock::now();
        // БЫСТРЫЙ поиск через индекс имени
        std::vector<std::string> ids = database.getNameIndex().Get(nameEdit->text().toStdString());
        std::vector<Student> students;
        for (const auto& id : ids) {
            auto studentResult = database.findByID(id);
            if (!studentResult.empty()) {
                students.push_back(studentResult[0]);
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        showSearchResults(students, ids, "Результаты поиска по имени", duration.count());
    });

    connect(searchBySurnameBtn, &QPushButton::clicked, [&, surnameEdit]() {
        auto start = std::chrono::high_resolution_clock::now();
        // БЫСТРЫЙ поиск через индекс фамилии
        std::vector<std::string> ids = database.getSurnameIndex().Get(surnameEdit->text().toStdString());
        std::vector<Student> students;
        for (const auto& id : ids) {
            auto studentResult = database.findByID(id);
            if (!studentResult.empty()) {
                students.push_back(studentResult[0]);
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        showSearchResults(students, ids, "Результаты поиска по фамилии", duration.count());
    });

    connect(searchByPhoneBtn, &QPushButton::clicked, [&, phoneEdit]() {
        try {
            auto start = std::chrono::high_resolution_clock::now();
            // БЫСТРЫЙ поиск через индекс телефона
            std::vector<std::string> ids = database.getPhoneIndex().Get(Phone(phoneEdit->text().toStdString()));
            std::vector<Student> students;
            for (const auto& id : ids) {
                auto studentResult = database.findByID(id);
                if (!studentResult.empty()) {
                    students.push_back(studentResult[0]);
                }
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            showSearchResults(students, ids, "Результаты поиска по телефону", duration.count());
        } catch ([[maybe_unused]] const std::exception& e) {
            QMessageBox::critical(this, "Ошибка", "Неверный формат телефона");
        }
    });

    connect(searchByAgeRangeBtn, &QPushButton::clicked, [&, minAgeSpin, maxAgeSpin]() {
        auto start = std::chrono::high_resolution_clock::now();
        // БЫСТРЫЙ поиск через индекс возраста
        std::vector<std::string> ids = database.getAgeIndex().GetRange(minAgeSpin->value(), maxAgeSpin->value());
        std::vector<Student> students;
        for (const auto& id : ids) {
            auto studentResult = database.findByID(id);
            if (!studentResult.empty()) {
                students.push_back(studentResult[0]);
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        showSearchResults(students, ids, "Результаты поиска по диапазону возрастов", duration.count());
    });

    dialog.exec();
}

void MainWindow::removeStudent() {
    bool ok;
    QString id = QInputDialog::getText(this, "Удалить студента",
                                     "Введите ID студента:",
                                     QLineEdit::Normal, "", &ok);
    if (ok && !id.isEmpty()) {
        database.removeStudent(id.toStdString());
        refreshStudentList();
        refreshCache(); // ОБНОВЛЯЕМ КЭШ ПОСЛЕ УДАЛЕНИЯ
        QMessageBox::information(this, "Успех", "Студент удален");
    }
}

void MainWindow::setupSearchTab(QWidget* tab, const QString& label, QWidget* input, QPushButton* button) {
    auto *layout = new QVBoxLayout(tab); // NOLINT
    auto *labelWidget = new QLabel(label); // NOLINT
    labelWidget->setFont(QFont("Arial", 12));
    layout->addWidget(labelWidget);
    input->setFont(QFont("Arial", 12));
    layout->addWidget(input);
    button->setFont(QFont("Arial", 12));
    button->setMinimumHeight(35);
    layout->addWidget(button);
    layout->addStretch();
}
