#ifndef WINDOW1_H
#define WINDOW1_H

#include <QDialog>

namespace Ui {
class window1;  // Оставляем имя класса с маленькой буквы
}

class window1 : public QDialog
{
    Q_OBJECT

public:
    explicit window1(QWidget *parent = nullptr);
    ~window1();

private:
    Ui::window1 *ui;  // Оставляем имя класса с маленькой буквы
};

#endif // WINDOW1_H
