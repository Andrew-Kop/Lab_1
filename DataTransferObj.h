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
    QVector<double> firstDer; // первая производная

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
        diff_ui_vi(),
        firstDer()
    {}

    virtual void solveWithoutControl(const QList<double>& initVals, double h0, double B, int Hmax, double Egr) {}
    virtual void solveWithControl(const QList<double>& initVals, double h0, double B, int Hmax, double E, double Egr) {}

    virtual ~DataTransferObj() {}

    DataTransferObj& operator=(const DataTransferObj& obj) {
        if (this == &obj) return *this;
        this->_type = obj._type;
        this->c1 = obj.c1;
        this->c2 = obj.c2;
        this->diff_ui_vi = obj.diff_ui_vi;
        this->diff_vi_v2i = obj.diff_vi_v2i;
        this->errMsg = obj.errMsg;
        this->hi = obj.hi;
        this->olp = obj.olp;
        this->resultSteps2 = obj.resultSteps2;
        this->ui = obj.ui;
        this->vi = obj.vi;
        this->xi = obj.xi;
        this->firstDer = obj.firstDer;
        return *this;
    }

};

#endif // DATATRANSFEROBJ_H
