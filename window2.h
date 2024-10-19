#ifndef WINDOW2_H
#define WINDOW2_H

#include <QDialog>

namespace Ui {
class window2;
}

class window2 : public QDialog
{
    Q_OBJECT

public:
    explicit window2(QWidget *parent = nullptr);
    ~window2();

private:
    Ui::window2 *ui;
};

#endif // WINDOW2_H
