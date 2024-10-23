#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void openWindow1();
    void openWindow2();
    void openWindow3();

private:
    Ui::MainWindow *ui;
    void setStyle();
    QCustomPlot *wGraphic;

};

#endif // MAINWINDOW_H
