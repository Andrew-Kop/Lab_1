#pragma once
#ifndef TASKMANAGER_H
#define TASKMANAGER_H
#include "DataTransferObj.h"
#include "TaskParameters.h"
#include <QString>
#include <QStringList>
#include <QList>

class TaskManager {
private:
    DataTransferObj* toSend;
    TaskParameters* params;
    bool isСonfigured;

    QList<double> parseInitialValues();

public:
    TaskManager();
    TaskManager(TaskParameters* settings);

    void setParams(TaskParameters* settings);
    DataTransferObj& getSolution();

    ~TaskManager() {delete toSend;}
};

#endif // TASKMANAGER_H
