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

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onTaskTypeChanged(int index);
    void startCalculation();
    //void stopCalculation();
    bool validateInput(int index);

private:
    Ui::MainWindow *ui;
    void setStyle();
    QComboBox *taskComboBox;
    QLineEdit *initialConditions, *nMaxInput, *epsilonInput, *boundaryPrecisionInput, *initialStepInput, *rightborder, *additionalParam1, *additionalParam2;
    QPushButton *startButton;// *stopButton;
    QCustomPlot *customPlot;
    QCustomPlot *phasePortraitPlot;
    QCheckBox *check_of_error_rate;


    // Объявление таблиц и контейнера QStackedWidget
    QTableWidget *resultsTableTestTask;
    QTableWidget *resultsTableMainTask;
    QStackedWidget *tableStackedWidget;

    QTextEdit *outputTextEdit; // Поле для вывода текста
};

#endif // MAINWINDOW_H
