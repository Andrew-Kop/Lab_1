#include "window1.h"
#include "ui_window1.h"

window1::window1(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::window1)  // Оставляем имя класса с маленькой буквы
{
    ui->setupUi(this);
}

window1::~window1()
{
    delete ui;
}
