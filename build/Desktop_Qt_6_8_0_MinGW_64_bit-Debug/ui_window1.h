/********************************************************************************
** Form generated from reading UI file 'window1.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOW1_H
#define UI_WINDOW1_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_window1
{
public:

    void setupUi(QDialog *window1)
    {
        if (window1->objectName().isEmpty())
            window1->setObjectName("window1");
        window1->resize(859, 642);

        retranslateUi(window1);

        QMetaObject::connectSlotsByName(window1);
    } // setupUi

    void retranslateUi(QDialog *window1)
    {
        window1->setWindowTitle(QCoreApplication::translate("window1", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class window1: public Ui_window1 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOW1_H
