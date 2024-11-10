#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QTableWidget>
#include "qcustomplot.h"
#include <QStackedWidget>
#include <QCheckBox>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

/*
 * MainWindow управляет пользовательским интерфейсом, отвечает за обработку
 * взаимодействий пользователя и визуализацию результатов.
 * Содержит элементы интерфейса для ввода данных и запуска вычислений,
 * а также методы для управления этими элементами и проверки корректности ввода.
 *
 * Public методы и поля:
 * - Конструктор MainWindow и его деструктор ~MainWindow;
 *
 * - Слот onTaskTypeChanged - отвечает за реакцию на выбор типа задачи
 *   (тип данных: int);
 * - Слот startCalculation - инициализирует расчёты;
 * - Метод validateInput - проверяет корректность пользовательского ввода.
 *
 * Private методы и поля:
 * - Метод setStyle - настраивает стили элементов интерфейса;
 *
 * - Поле ui - предоставляет доступ к графическим элементам интерфейса
 *   (тип данных: Ui::MainWindow*);
 * - Поле taskComboBox - выпадающий список для выбора типа задачи
 *   (тип данных: QComboBox*);
 * - Поля для ввода значений - initialConditions, nMaxInput, epsilonInput,
 *   boundaryPrecisionInput, initialStepInput, rightborder, additionalParam1,
 *   additionalParam2 (тип данных: QLineEdit*);
 * - Поле startButton - кнопка для запуска расчётов (тип данных: QPushButton*);
 * - Поля customPlot и phasePortraitPlot - для визуализации графиков
 *   (тип данных: QCustomPlot*);
 * - Поле check_of_error_rate - флажок для выбора учёта локальной погрешности
 *   (тип данных: QCheckBox*);
 * - Поля resultsTableTestTask и resultsTableMainTask - таблицы для отображения
 *   результатов расчётов для разных типов задач (тип данных: QTableWidget*);
 * - Поле tableStackedWidget - контейнер QStackedWidget для переключения
 *   между таблицами (тип данных: QStackedWidget*);
 * - Поле outputTextEdit - текстовое поле для вывода результатов и сообщений
 *   (тип данных: QTextEdit*).
 */

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTaskTypeChanged(int index);
    /*
     * Слот, который срабатывает при изменении типа задачи.
     * - index - индекс выбранного типа задачи в выпадающем списке.
     * (тип данных: int).
     */

    void startCalculation();
    /*
     * Слот, запускающий расчёты на основе текущих значений ввода.
     * Инициирует выполнение задачи и отображение результатов.
     */

    bool validateInput(int index);
    /*
     * Метод для проверки корректности пользовательского ввода.
     * - index - индекс типа задачи для специфичной проверки.
     * Возвращает true, если ввод корректен, иначе - false.
     */

private:
     Ui::MainWindow *ui;

    void setStyle();
     /*
     * Вспомогательный метод для настройки стилей элементов интерфейса.
     * Применяет нужные стили для улучшения визуализации.
     */

     QComboBox *taskComboBox;
     QLineEdit *initialConditions, *nMaxInput, *epsilonInput, *boundaryPrecisionInput, *initialStepInput, *rightborder, *additionalParam1, *additionalParam2;
    QPushButton *startButton; // Кнопка для запуска расчётов
    QCustomPlot *customPlot;  // Поле для отображения основного графика
    QCustomPlot *phasePortraitPlot; // Поле для отображения фазового портрета
    QCheckBox *check_of_error_rate; // Флажок для выбора учёта погрешности

    QTableWidget *resultsTableTestTask; // Таблица для результатов тестовой задачи
    QTableWidget *resultsTableMainTask; // Таблица для результатов основной задачи
    QStackedWidget *tableStackedWidget; // QStackedWidget для переключения между таблицами

    QTextEdit *outputTextEdit; // Поле для вывода текста и сообщений
};

#endif // MAINWINDOW_H
