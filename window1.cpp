#include "window1.h"
#include "ui_window1.h"
#include "TaskManager.h"

window1::window1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::window1) {
    ui->setupUi(this);

    // Установка заголовка окна
    this->setWindowTitle("Задача Коши для ОДУ");

    // Установка флагов окна
    this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

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

    boundaryPrecisionInput = new QLineEdit(this);
    boundaryPrecisionInput->setPlaceholderText("Точность выхода на границу");

    initialStepInput = new QLineEdit(this);
    initialStepInput->setPlaceholderText("Начальный шаг");

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
    inputLayout->addWidget(initialConditions, 0, 0);
    inputLayout->addWidget(nMaxInput, 0, 1);
    inputLayout->addWidget(epsilonInput, 1, 0);
    inputLayout->addWidget(boundaryPrecisionInput, 1, 1);
    inputLayout->addWidget(initialStepInput, 2, 0);
    inputLayout->addWidget(startButton, 2, 1);
    inputLayout->addWidget(stopButton, 2, 2);

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

    setLayout(mainLayout);

    // Подключение сигналов и слотов
    connect(taskComboBox, &QComboBox::currentIndexChanged, this, &window1::onTaskTypeChanged);
    connect(startButton, &QPushButton::clicked, this, &window1::startCalculation);
    connect(stopButton, &QPushButton::clicked, this, &window1::stopCalculation);

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

window1::~window1() {
    delete ui;
}

void window1::onTaskTypeChanged(int index) {
    // Переключаемся между таблицами в зависимости от выбранного типа задачи
    if (index == 0) {
        tableStackedWidget->setCurrentWidget(resultsTableTestTask);  // Показываем таблицу для тестовой задачи
    } else {
        tableStackedWidget->setCurrentWidget(resultsTableMainTask);  // Показываем таблицу для основной задачи
    }
}

void window1::startCalculation() {
    // Создание объекта для хранения параметров задачи
    TaskParameters params;
    params.taskType = taskComboBox->currentText();
    params.initialConditions = initialConditions->text();
    params.nMax = nMaxInput->text().toInt();
    params.epsilon = epsilonInput->text().toDouble();
    params.boundaryPrecision = boundaryPrecisionInput->text().toDouble();
    params.initialStep = initialStepInput->text().toDouble();
    // Добавил в параметры тип задачи в виде индекса
    params.taskTypeInd = taskComboBox->currentIndex();

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

void window1::stopCalculation() {
    // Логика остановки расчётов
    outputTextEdit->append("Расчеты остановлены.");
}


