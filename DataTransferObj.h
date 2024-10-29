#pragma once
#ifndef DATATRANSFEROBJ_H
#define DATATRANSFEROBJ_H
#include <QString>
#include <QVector>

class DataTransferObj {
public:
    int _type; // -1 - ошибка, стоит посмотреть сообщение о ней
    QString errMsg;
    QVector<double> vi; //vi
    QVector<double> xi; //xi
    QVector<double> resultSteps2; // v2i
    QVector<double> diff_vi_v2i; //vi - v2i
    QVector<double> olp; // ОЛП
    QVector<double> hi; //hi
    QVector<double> c1; //c1
    QVector<double> c2; //c2
    QVector<double> ui; //ui
    QVector<double> diff_ui_vi; // |ui - vi|

    DataTransferObj(int type = 0, QString err = ""):
        _type(type),
        errMsg(err),
        vi(),
        xi(),
        resultSteps2(),
        diff_vi_v2i(),
        olp(),
        hi(),
        c1(),
        c2(),
        ui(),
        diff_ui_vi()
    {}

    virtual void solveWithoutControl(double x0, double u0, double h0, double B, int Hmax, double Egr) {}
    virtual void solveWithControl(double x0, double u0, double h0, double B, int Hmax, double E, double Egr) {}

    virtual ~DataTransferObj() {}

};

#endif // DATATRANSFEROBJ_H
