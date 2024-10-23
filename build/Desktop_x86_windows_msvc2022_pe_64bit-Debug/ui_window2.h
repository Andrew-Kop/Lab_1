/********************************************************************************
** Form generated from reading UI file 'window2.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOW2_H
#define UI_WINDOW2_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_window2
{
public:
    QFrame *frame;
    QLabel *label;
    QComboBox *comboBox;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QGroupBox *groupBox;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QPushButton *pushButton;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    void setupUi(QDialog *window2)
    {
        if (window2->objectName().isEmpty())
            window2->setObjectName("window2");
        window2->resize(1043, 882);
        frame = new QFrame(window2);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(180, 230, 841, 291));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        label = new QLabel(frame);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 10, 63, 20));
        comboBox = new QComboBox(frame);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(10, 40, 131, 28));
        label_2 = new QLabel(frame);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(230, 10, 63, 20));
        label_3 = new QLabel(frame);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(230, 120, 63, 20));
        label_4 = new QLabel(frame);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(230, 170, 63, 20));
        label_5 = new QLabel(frame);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(230, 60, 63, 20));
        label_6 = new QLabel(frame);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(10, 80, 201, 20));
        groupBox = new QGroupBox(frame);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(460, 10, 371, 271));
        lineEdit = new QLineEdit(frame);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(10, 100, 113, 28));
        lineEdit_2 = new QLineEdit(frame);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(230, 30, 113, 28));
        lineEdit_3 = new QLineEdit(frame);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(230, 80, 113, 28));
        lineEdit_4 = new QLineEdit(frame);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(230, 140, 113, 28));
        lineEdit_5 = new QLineEdit(frame);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(230, 190, 113, 28));
        pushButton = new QPushButton(frame);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(10, 150, 111, 31));
        scrollArea = new QScrollArea(window2);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setGeometry(QRect(180, 530, 411, 321));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 409, 319));
        scrollArea->setWidget(scrollAreaWidgetContents);

        retranslateUi(window2);

        QMetaObject::connectSlotsByName(window2);
    } // setupUi

    void retranslateUi(QDialog *window2)
    {
        window2->setWindowTitle(QCoreApplication::translate("window2", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("window2", "\320\227\320\260\320\264\320\260\321\207\320\260", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("window2", "\320\242\320\265\321\201\321\202\320\276\320\262\320\260\321\217", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("window2", "\320\236\321\201\320\275\320\276\320\262\320\275\320\260\321\217 \342\204\2261", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("window2", "\320\236\321\201\320\275\320\276\320\262\320\275\320\260\321\217 \342\204\2262", nullptr));

        label_2->setText(QCoreApplication::translate("window2", "TextLabel", nullptr));
        label_3->setText(QCoreApplication::translate("window2", "TextLabel", nullptr));
        label_4->setText(QCoreApplication::translate("window2", "TextLabel", nullptr));
        label_5->setText(QCoreApplication::translate("window2", "TextLabel", nullptr));
        label_6->setText(QCoreApplication::translate("window2", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\275\320\260\321\207\320\260\320\273\321\214\320\275\321\213\320\265 \321\203\321\201\320\273\320\276\320\262\320\270\321\217", nullptr));
        groupBox->setTitle(QCoreApplication::translate("window2", "\320\241\320\277\321\200\320\260\320\262\320\272\320\260", nullptr));
        pushButton->setText(QCoreApplication::translate("window2", "\320\222\321\213\321\207\320\270\321\201\320\273\320\270\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class window2: public Ui_window2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOW2_H
