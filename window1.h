#ifndef WINDOW1_H
#define WINDOW1_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include "qcustomplot.h"
#include <QStackedWidget>
#include <QTextEdit>

namespace Ui {
class window1;
}

class window1 : public QDialog {
    Q_OBJECT

public:
    explicit window1(QWidget *parent = nullptr);
    ~window1();

private slots:
    void onTaskTypeChanged(int index);
    void startCalculation();
    void stopCalculation();

private:
    Ui::window1 *ui;

    QComboBox *taskComboBox;
    QLineEdit *initialConditions, *nMaxInput, *epsilonInput, *boundaryPrecisionInput, *initialStepInput;
    QPushButton *startButton, *stopButton;
    QCustomPlot *customPlot;

    // Объявление таблиц и контейнера QStackedWidget
    QTableWidget *resultsTableTestTask;
    QTableWidget *resultsTableMainTask;
    QStackedWidget *tableStackedWidget;

    QTextEdit *outputTextEdit; // Поле для вывода текста
};

#endif // WINDOW1_H
