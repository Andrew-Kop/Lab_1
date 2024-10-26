#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QIcon>
#include <QDir>
#include "TaskParameters.h"
#include "TaskManager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Установка заголовка окна
    this->setWindowTitle("Лабораторная работа №1");

    //Установил иконку
    setWindowIcon(QIcon(":/icons/icons/icon.png"));

    // Установка флагов окна
    this->setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);


    // Инициализация выпадающего списка задач
    taskComboBox = new QComboBox(this);
    taskComboBox->addItem("Тестовая задача");
    taskComboBox->addItem("Основная задача №1");
    taskComboBox->addItem("Основная задача №2");

    // Инициализация полей ввода параметров
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

    // Новые поля ввода для основной задачи №2
    additionalParam1 = new QLineEdit(this);
    additionalParam1->setPlaceholderText("Дополнительный параметр 1 = a");

    additionalParam2 = new QLineEdit(this);
    additionalParam2->setPlaceholderText("Дополнительный параметр 2 = b");


    // Инициализация кнопок управления
    startButton = new QPushButton("Начать расчёт", this);
    stopButton = new QPushButton("Остановить", this);

    // Инициализация графика
    customPlot = new QCustomPlot(this);
    customPlot->setMinimumHeight(300);
    customPlot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    resultsTableTestTask = new QTableWidget(this);
    resultsTableTestTask->setColumnCount(11);
    resultsTableTestTask->setRowCount(20);
    resultsTableTestTask->setHorizontalHeaderLabels(QStringList() << "i" << "xi" << "vi" << "v2i" << "vi - v2i"
                                                                  << "ОЛП" << "hi" << "C1" << "C2" << "ui" << "|ui - vi|");

    // Устанавливаем режим растягивания для заголовков
    resultsTableTestTask->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    resultsTableMainTask = new QTableWidget(this);
    resultsTableMainTask->setColumnCount(9);
    resultsTableMainTask->setRowCount(20);
    resultsTableMainTask->setHorizontalHeaderLabels(QStringList() << "№" << "xi" << "vi" << "v2i" << "vi - v2i"
                                                                  << "ОЛП" << "hi" << "C1" << "C2");

    // Устанавливаем режим растягивания для заголовков
    resultsTableMainTask->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Используем QStackedWidget для переключения таблиц
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
    inputLayout->addWidget(startButton, 4, 0);                    // Кнопка "Начать расчёт"
    inputLayout->addWidget(stopButton, 4, 1);                     // Кнопка "Остановить"
    inputLayout->addWidget(additionalParam1, 5, 0);
    inputLayout->addWidget(additionalParam2, 5, 1);
    additionalParam1->setVisible(false); // Скрыть по умолчанию
    additionalParam2->setVisible(false); // Скрыть по умолчанию


    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(customPlot);
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


    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Подключение сигналов и слотов
    connect(taskComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::onTaskTypeChanged);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startCalculation);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::stopCalculation);

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
    stopButton->setStyleSheet(buttonStyle);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onTaskTypeChanged(int index) {
    // Переключаемся между таблицами в зависимости от выбранного типа задачи
    if (index == 0) {
        tableStackedWidget->setCurrentWidget(resultsTableTestTask);  // Показываем таблицу для тестовой задачи
        additionalParam1->setVisible(false); // Скрываем дополнительные параметры
        additionalParam2->setVisible(false);
    } else if (index == 1) {
        tableStackedWidget->setCurrentWidget(resultsTableMainTask);  // Показываем таблицу для основной задачи 1
        additionalParam1->setVisible(false); // Скрываем дополнительные параметры
        additionalParam2->setVisible(false);
    } else if (index == 2) {
        tableStackedWidget->setCurrentWidget(resultsTableMainTask);  // Показываем таблицу для основной задачи 2
        additionalParam1->setVisible(true); // Показываем дополнительные параметры
        additionalParam2->setVisible(true);
    }
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
    params.rightborder = rightborder->text().toInt();
    params.error_rate = check_of_error_rate->isChecked();

    if (params.taskTypeInd == 2) { // Убедитесь, что это основная задача 2
        params.additionalParam1 = additionalParam1->text().toDouble(); // Если у вас есть соответствующие поля в структуре
        params.additionalParam2 = additionalParam2->text().toDouble(); // Если у вас есть соответствующие поля в структуре
    }
    // Пример вывода параметров для отладки
    outputTextEdit->clear();
    outputTextEdit->append("Запуск расчетов...");
    outputTextEdit->append(params.toString());  // Используем объект для вывода

    // Здесь можно вызвать функции для вычислений, передав объект params
    TaskManager manager(&params);
    DataTransferObj data = manager.getSolution();

    //ошибка
    if(data._type == -1) return;

    customPlot->addGraph();
    customPlot->graph(0)->setData(data.xData, data.numericalSolutionData);

    customPlot->addGraph();
    customPlot->graph(1)->setData(data.xData, data.trueSolutionData);

    customPlot->replot();
}

void MainWindow::stopCalculation() {
    // Логика остановки расчётов
    outputTextEdit->append("Расчеты остановлены.");
}
