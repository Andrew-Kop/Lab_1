#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QIcon>
#include <QDir>
#include "TaskParameters.h"
#include "TaskManager.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Установка заголовка окна
    this->setWindowTitle("Лабораторная работа №1");

    //Установка иконки
    setWindowIcon(QIcon(":/icons/icons/icon.png"));

    //Установка флагов окна
    this->setWindowFlags(Qt::Window | Qt:: WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);

    //Инициализация выпадающего списка
    taskComboBox = new QComboBox(this);
    taskComboBox->addItem("Тестовая задача");
    taskComboBox->addItem("Основная задача №1");
    taskComboBox->addItem("Основная задача №2");

    //Инициализация полей ввода параметров
    initialConditions = new QLineEdit(this);
    initialConditions->setPlaceholderText("Начальные условия");

    nMaxInput = new QLineEdit(this);
    nMaxInput->setPlaceholderText("Максимальное количество шагов (Nmax)");

    epsilonInput = new QLineEdit(this);
    epsilonInput->setPlaceholderText("Контроль локальной погрешности (ε)");

    rightborder = new QLineEdit(this);
    rightborder->setPlaceholderText("Правая граница");

    boundaryPrecisionInput = new QLineEdit(this);
    boundaryPrecisionInput->setPlaceholderText("Точность выхода на границу");

    initialStepInput = new QLineEdit(this);
    initialStepInput->setPlaceholderText("Начальный шаг");

    check_of_error_rate = new QCheckBox("С учётом локальной погрешности?", this);
    check_of_error_rate->setStyleSheet("QCheckBox { font-size: 14px }");

    //Поля для основной задачи №2

    additionalParam1 = new QLineEdit(this);
    additionalParam1->setPlaceholderText("Дополнительный параметр 1 = а");

    additionalParam2 = new QLineEdit(this);
    additionalParam2->setPlaceholderText("Дополнительный параметр 2 = b");

    //Инициализация кнопок управления
    startButton = new QPushButton("Начать расчёт", this);
    //stopButton = new QPushButton("Остановить", this);

    //Инициализация графиков
    customPlot = new QCustomPlot(this);
    customPlot->setMinimumHeight(250);
    customPlot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    phasePortraitPlot = new QCustomPlot(this);
    phasePortraitPlot->setMinimumHeight(250);
    phasePortraitPlot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    //Добавление графиков в компоновщик
    QWidget *graphsWidget = new QWidget(this);
    QHBoxLayout *graphsLayout = new QHBoxLayout(graphsWidget); //устанавливаем компоновщик на widget

    //Добавление графиков в компоновщик
    graphsLayout->addWidget(customPlot);
    graphsLayout->addWidget(phasePortraitPlot);
    phasePortraitPlot->setVisible(false);

    //Инициализация таблиц
    resultsTableTestTask = new QTableWidget(this);
    resultsTableTestTask->setColumnCount(11);
    resultsTableTestTask->setRowCount(20);
    resultsTableTestTask->setHorizontalHeaderLabels(QStringList()<< "i" << "xi" << "vi" << "v2i" << "vi - v2i"
                                                                  << "ОЛП" << "hi" << "C1" << "C2" << "ui" << "|ui - vi|");
    resultsTableMainTask = new QTableWidget(this);
    resultsTableMainTask->setColumnCount(9);
    resultsTableMainTask->setRowCount(20);
    resultsTableMainTask->setHorizontalHeaderLabels(QStringList() << "№" << "xi" << "vi" << "v2i" << "vi - v2i"
                                                                  << "ОЛП" << "hi" << "C1" << "C2");

    // Устанавливаем режим растягивания для заголовков
    resultsTableMainTask->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    resultsTableTestTask->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //Используем QStackedWidget для переключения таблиц
    tableStackedWidget = new QStackedWidget(this);
    tableStackedWidget->addWidget(resultsTableTestTask);  // Первая таблица
    tableStackedWidget->addWidget(resultsTableMainTask);  // Вторая таблица

    // Layout для окна
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(taskComboBox);

    // GridLayout для ввода параметров
    QGridLayout *inputLayout = new QGridLayout();
    inputLayout->addWidget(initialConditions, 0, 0, 1, 2);        // Начальные условия, растягиваем на 2 колонки
    inputLayout->addWidget(nMaxInput, 1, 0);                      // Максимальное количество шагов
    inputLayout->addWidget(epsilonInput, 1, 1);                   // Локальная погрешность
    inputLayout->addWidget(boundaryPrecisionInput, 2, 0);         // Точность выхода на границу
    inputLayout->addWidget(initialStepInput, 2, 1);               // Начальный шаг
    inputLayout->addWidget(check_of_error_rate, 3, 0);      // Чекбокс, растягиваем на 2 колонки
    inputLayout->addWidget(rightborder, 3, 1);                    // Поле правой границы
    inputLayout->addWidget(startButton, 4, 0, 1 , 2);                    // Кнопка "Начать расчёт"
    //inputLayout->addWidget(stopButton, 4, 1);                     // Кнопка "Остановить"
    inputLayout->addWidget(additionalParam1, 5, 0);
    inputLayout->addWidget(additionalParam2, 5, 1);
    additionalParam1->setVisible(false); // Скрыть по умолчанию
    additionalParam2->setVisible(false); // Скрыть по умолчанию

    //Добавляем всё в mainLayout
    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(graphsWidget);
    mainLayout->addWidget(tableStackedWidget);  // Добавляем stack виджет для таблиц

    // Добавляем справочную информацию
    QLabel *helpLabel = new QLabel("Справка:", this);
    helpLabel->setStyleSheet("font-weight: bold; font-size: 16px; color: #4CAF50;");  // Зеленый цвет
    outputTextEdit = new QTextEdit(this);
    outputTextEdit->setReadOnly(true);  // Только для чтения
    outputTextEdit->setMinimumHeight(100);  // Минимальная высота
    outputTextEdit->setStyleSheet(
        "QTextEdit {"
        "    font-size: 16px;"
        "    color: black;"
        "    background-color: #F1F8E9;"  // Светлый фон для текстового поля
        "    border: 1px solid #A5D6A7;"  // Обводка текста
        "    border-radius: 8px;"  // Закругленные углы
        "    padding: 5px;"
        "}"
        );

    mainLayout->addWidget(helpLabel);  // Добавляем заголовок справки
    mainLayout->addWidget(outputTextEdit);  // Добавляем текстовое поле

    //Устанавливаем центральный Layout на окно
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Подключение сигналов и слотов
    connect(taskComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::onTaskTypeChanged);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startCalculation);
    //connect(stopButton, &QPushButton::clicked, this, &MainWindow::stopCalculation);

    // Установка стилей для кнопок
    QString buttonStyle =
        "QPushButton {"
        "    background-color: #4CAF50;"  // Зеленый фон
        "    color: white;"
        "    border-radius: 12px;"
        "    padding: 10px 20px;"
        "    border: none;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "} "
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "} "
        "QPushButton:pressed {"
        "    background-color: #388E3C;"
        "    padding: 9px 19px;"
        "} "
        "QPushButton:disabled {"
        "    background-color: #A5D6A7;"
        "    color: #B0BEC5;"
        "    border: none;"
        "}";

    startButton->setStyleSheet(buttonStyle);
    //stopButton->setStyleSheet(buttonStyle);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onTaskTypeChanged(int index) {
    // Переключаемся между таблицами в зависимости от выбранного типа задачи
    if (index == 0) {
        tableStackedWidget->setCurrentWidget(resultsTableTestTask);
        phasePortraitPlot->setVisible(false);  // Скрываем фазовый портрет
        additionalParam1->setVisible(false);
        additionalParam2->setVisible(false);
    } else if (index == 1) {
        tableStackedWidget->setCurrentWidget(resultsTableMainTask);
        phasePortraitPlot->setVisible(false);  // Скрываем фазовый портрет
        additionalParam1->setVisible(false);
        additionalParam2->setVisible(false);
    } else if (index == 2) {
        tableStackedWidget->setCurrentWidget(resultsTableMainTask);
        phasePortraitPlot->setVisible(true);   // Показываем фазовый портрет
        additionalParam1->setVisible(true);
        additionalParam2->setVisible(true);
    }
}

bool MainWindow::validateInput(int index) {
    bool ok;

    // Validate Nmax as positive integer
    int nMax = nMaxInput->text().toInt(&ok);
    if (!ok || nMax <= 0) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a positive integer for Nmax.");
        return false;
    }

    // Validate epsilon as positive double
    double epsilon = epsilonInput->text().toDouble(&ok);
    if (!ok || epsilon <= 0.0) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a positive number for Epsilon.");
        return false;
    }

    // Validate boundary precision as positive double
    double boundaryPrecision = boundaryPrecisionInput->text().toDouble(&ok);
    if (!ok || boundaryPrecision <= 0.0) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a positive number for Boundary Precision.");
        return false;
    }

    // Validate initial step as positive double
    double initialStep = initialStepInput->text().toDouble(&ok);
    if (!ok || initialStep <= 0.0) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a positive number for Initial Step.");
        return false;
    }

    // Validate right border as positive integer
    int rightBorder = rightborder->text().toInt(&ok);
    if (!ok || rightBorder <= 0) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a positive integer for Right Border.");
        return false;
    }
    if (index ==2){
        // Additional parameters as doubles
        double param1 = additionalParam1->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Invalid Input", "Please enter a valid number for Additional Parameter 1.");
            return false;
        }

        double param2 = additionalParam2->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Invalid Input", "Please enter a valid number for Additional Parameter 2.");
            return false;
        }
    }

    return true;
}



void MainWindow::startCalculation() {

    // Создание объекта для хранения параметров задачи
    TaskParameters params;
    params.taskType = taskComboBox->currentText();
    params.initialConditions = initialConditions->text();
    params.nMax = nMaxInput->text().toInt();
    params.epsilon = epsilonInput->text().toDouble();
    params.boundaryPrecision = boundaryPrecisionInput->text().toDouble();
    params.initialStep = initialStepInput->text().toDouble();
    params.taskTypeInd = taskComboBox->currentIndex();
    params.rightborder = rightborder->text().toDouble();
    params.error_rate = check_of_error_rate->isChecked();

    if (params.taskTypeInd == 2) { // Убедитесь, что это основная задача 2
        params.additionalParam1 = additionalParam1->text().toDouble(); // Если у вас есть соответствующие поля в структуре
        params.additionalParam2 = additionalParam2->text().toDouble(); // Если у вас есть соответствующие поля в структуре
    }
    if (!validateInput(params.taskTypeInd)) {
        return;
    }
    // Пример вывода параметров для отладки
    outputTextEdit->clear();
    outputTextEdit->append("Запуск расчетов...");

    // Здесь можно вызвать функции для вычислений, передав объект params
    TaskManager manager(&params);
    DataTransferObj data = manager.getSolution();

    std::cout << "\nFinal vi values:\n";
    for (size_t j = 0; j < data.vi.size(); ++j) {
        std::cout << "vi[" << j << "]: " << data.vi[j] << "\n";
    }

    std::cout << "\nFinal firstDer values:\n";
    for (size_t j = 0; j < data.firstDer.size(); ++j) {
        std::cout << "firstDer[" << j << "]: " << data.firstDer[j] << "\n";
    }


    //ошибка
    if(data._type == -1) {
        QMessageBox::warning(this, "Error", data.errMsg);
        return;
    }

    // Вывод справки
    auto itMax = std::max_element(data.hi.begin(), data.hi.end());
    auto itMin = std::min_element(data.hi.begin(), data.hi.end());
    QString info = QString("Шагов: %1\nЧисло делений: %2\nЧисло удвоений: %3"
                           "\nМаксимальный шаг: %4, при x = %5\nМинимальный шаг: %6, при x = %7"
                           "\nМаксимальное значение ОЛП: %8\nРазница между последней точкой и границей: %9")
        .arg(data.xi.length())
        .arg(data.c1.back())
        .arg(data.c2.back())
        .arg(*itMax)
        .arg(data.xi.at(std::distance(data.hi.begin(), itMax)))
        .arg(*itMin)
        .arg(data.xi.at(std::distance(data.hi.begin(), itMin)))
        .arg(*std::max_element(data.olp.begin(), data.olp.end()))
        .arg(params.rightborder - data.xi.back());

    if (params.taskTypeInd == 0) {
        auto it = std::max_element(data.diff_ui_vi.begin(), data.diff_ui_vi.end());
        info.append(QString("\nМаксимальная ошибка: %10, при x = %11")
            .arg(*it)
            .arg(data.xi.at(std::distance(data.diff_ui_vi.begin(), it))));
    }
    info.append("\n\n");

    outputTextEdit->append(info);
    outputTextEdit->append("Информация для отладки:");
    outputTextEdit->append(params.toString());  // Используем объект для вывода

    // Построение графиков
    customPlot->clearGraphs();
    phasePortraitPlot->clearGraphs();

    // Настройка графика для численного решения
    customPlot->addGraph();
    customPlot->graph(0)->setData(data.xi, data.vi);
    customPlot->graph(0)->setPen(QPen(Qt::blue));
    customPlot->graph(0)->setName("Численное решение");

    // Настройка графика для истинного решения
    if (params.taskTypeInd == 0) {
        customPlot->addGraph();
        customPlot->graph(1)->setData(data.xi, data.ui);
        customPlot->graph(1)->setPen(QPen(Qt::red));
        customPlot->graph(1)->setName("Истинное решение");
    }

    if (params.taskTypeInd == 2) {
        customPlot->addGraph();
        customPlot->graph(1)->setData(data.xi, data.firstDer);
        customPlot->graph(1)->setPen(QPen(Qt::red));
        customPlot->graph(1)->setName("Первая производная");

        phasePortraitPlot->addGraph();
        phasePortraitPlot->graph(0)->setData(data.vi, data.firstDer);
        phasePortraitPlot->graph(0)->setPen(QPen(Qt::blue));
        phasePortraitPlot->graph(0)->setName("(v, v')");

    }

    // Определение минимальных и максимальных значений для осей
    double xMin = *std::min_element(data.xi.begin(), data.xi.end());
    double xMax = *std::max_element(data.xi.begin(), data.xi.end());
    double yMin;
    double yMax;

    if (params.taskTypeInd == 0) {
        yMin = std::min(*std::min_element(data.vi.begin(), data.vi.end()),
                               *std::min_element(data.ui.begin(), data.ui.end()));
        yMax = std::max(*std::max_element(data.vi.begin(), data.vi.end()),
                               *std::max_element(data.ui.begin(), data.ui.end()));
    } else {
        yMin = *std::min_element(data.vi.begin(), data.vi.end());
        yMax = *std::max_element(data.vi.begin(), data.vi.end());
    }

    // Установка диапазона осей с небольшим отступом
    double offset = 0.1; // Отступ, который вы можете настроить
    customPlot->xAxis->setRange(xMin - offset, xMax + offset);
    customPlot->yAxis->setRange(yMin - offset, yMax + offset);

    if(params.taskTypeInd == 2)
    {
    double xMin_1 = *std::min_element(data.vi.begin(), data.vi.end());
    double xMax_1 = *std::max_element(data.vi.begin(), data.vi.end());
    double yMin_1 = *std::min_element(data.firstDer.begin(), data.firstDer.end());
    double yMax_1 = *std::max_element(data.firstDer.begin(), data.firstDer.end());

    // Установка диапазона осей с небольшим отступом
    phasePortraitPlot->xAxis->setRange(xMin_1 - offset, xMax_1 + offset);
    phasePortraitPlot->yAxis->setRange(yMin_1 - offset, yMax_1 + offset);
    }

    // Отображение легенды
    customPlot->legend->setVisible(true);

    // Обновление графика для отображения изменений
    customPlot->replot();
    if (params.taskTypeInd == 2) {
        phasePortraitPlot->legend->setVisible(true);
        phasePortraitPlot->replot();
    }

    // Заполнение таблицы
    QStringList currentRowData;
    switch (params.taskTypeInd) {
    case 0:
        resultsTableTestTask->clearContents();
        resultsTableTestTask->setRowCount(data.xi.length());

        for (int row = 0; row < data.xi.length(); ++row) {
            // Заполняем ячейки
            currentRowData = {
                QString::number(row), // i
                QString::number(data.xi.at(row)), // xi
                QString::number(data.vi.at(row)), // vi
                QString::number(data.resultSteps2.at(row)), // v2i
                QString::number(data.diff_vi_v2i.at(row)), // vi - v2i
                QString::number(data.olp.at(row)), // olp
                QString::number(data.hi.at(row)), // hi
                QString::number(data.c1.at(row)), // c1
                QString::number(data.c2.at(row)), // c2
                QString::number(data.ui.at(row)), // ui
                QString::number(data.diff_ui_vi.at(row)), // |ui-vi|
                };

            for (int col = 0; col < currentRowData.size(); col++) {
                resultsTableTestTask->setItem(row, col, new QTableWidgetItem(currentRowData.at(col)));
            }
        }

        break;

    // надо будет поменять под таблицы для второй основной
    case 1:
        resultsTableMainTask->clearContents();
        resultsTableMainTask->setRowCount(data.xi.length());

        for (int row = 0; row < data.xi.length(); ++row) {
            // Заполняем ячейки
            currentRowData = {
                QString::number(row), // i
                QString::number(data.xi.at(row)), // xi
                QString::number(data.vi.at(row)), // vi
                QString::number(data.resultSteps2.at(row)), // v2i
                QString::number(data.diff_vi_v2i.at(row)), // vi - v2i
                QString::number(data.olp.at(row)), // olp
                QString::number(data.hi.at(row)), // hi
                QString::number(data.c1.at(row)), // c1
                QString::number(data.c2.at(row)), // c2
            };

            for (int col = 0; col < currentRowData.size(); col++) {
                resultsTableMainTask->setItem(row, col, new QTableWidgetItem(currentRowData.at(col)));
            }
        }

        break;
    case 2:
        resultsTableMainTask->clearContents();
        resultsTableMainTask->setRowCount(data.xi.length());

        for (int row = 0; row < data.xi.length(); ++row) {
            // Заполняем ячейки
            currentRowData = {
                QString::number(row), // i
                QString::number(data.xi.at(row)), // xi
                QString::number(data.firstDer.at(row)), // vi
                QString::number(data.resultSteps2.at(row)), // v2i
                QString::number(data.diff_vi_v2i.at(row)), // vi - v2i
                QString::number(data.olp.at(row)), // olp
                QString::number(data.hi.at(row)), // hi
                QString::number(data.c1.at(row)), // c1
                QString::number(data.c2.at(row)), // c2
            };

            for (int col = 0; col < currentRowData.size(); col++) {
                resultsTableMainTask->setItem(row, col, new QTableWidgetItem(currentRowData.at(col)));
            }
        }

        break;
    }

}

