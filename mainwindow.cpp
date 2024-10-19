#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "window1.h"
#include "window2.h"
#include "window3.h"
#include <QIcon>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Установка иконки
    setWindowIcon(QIcon(":/icons/icons/icon.png"));
    // Инициализация стилей
    setStyle();

    // Подключаем кнопки к соответствующим слотам
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::openWindow1);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::openWindow2);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::openWindow3);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::openWindow1() {
    window1 *myWindow1 = new window1(this);
    myWindow1->show();
}

void MainWindow::openWindow2() {
    window2 *myWindow2 = new window2(this);
    myWindow2->show();
}

void MainWindow::openWindow3() {
    window3 *myWindow3 = new window3(this);
    myWindow3->show();
}

void MainWindow::setStyle() {
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

    QString labelStyle =
        "QLabel {"
        "    font-size: 28px;"
        "    font-weight: bold;"
        "    color: white;"  // Можно изменить на черный, если хотите светлую тему
        "    background-color: transparent;"
        "    text-align: center;"
        "}";

    // Установка цвета фона окна
    this->setStyleSheet("background-color: #1E1E1E;"); // Тёмный фон для окна

    ui->pushButton->setStyleSheet(buttonStyle);
    ui->pushButton_2->setStyleSheet(buttonStyle);
    ui->pushButton_3->setStyleSheet(buttonStyle);

    ui->label->setStyleSheet(labelStyle);
    ui->label->setAlignment(Qt::AlignCenter);  // Центрируем текст в QLabel
}
