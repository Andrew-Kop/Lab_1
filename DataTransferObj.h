#pragma once
#ifndef DATATRANSFEROBJ_H
#define DATATRANSFEROBJ_H
#include <QString>
#include <QVector>

class DataTransferObj {
public:
    int _type; // -1 - ошибка, стоит посмотреть сообщение о ней
    QString errMsg;
    QVector<double> numericalSolutionData;
    QVector<double> trueSolutionData;
    QVector<double> xData;

    DataTransferObj(int type = 0, QString err = ""): _type(type), errMsg(err), numericalSolutionData(), trueSolutionData(), xData() {}
};

#endif // DATATRANSFEROBJ_H
