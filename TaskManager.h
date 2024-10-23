#pragma once
#ifndef TASKMANAGER_H
#define TASKMANAGER_H
#include "DataTransferObj.h"
#include "TaskParameters.h"
#include <QString>
#include <QStringList>
#include <QList>
#include <vector>

class TaskManager {
private:
    DataTransferObj toSend;
    TaskParameters* params;
    bool isСonfigured;

    void parseSolution(const std::vector<std::vector<double>>& numSol);
    QList<double> parseInitialValues();

public:
    TaskManager();
    TaskManager(TaskParameters* settings);

    void setParams(TaskParameters* settings);
    DataTransferObj& getSolution();
};

#endif // TASKMANAGER_H
