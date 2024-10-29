#include "TaskManager.h"
#include "table_rk4.h"

void TaskManager::setParams(TaskParameters* settings) {
    params = settings;
    isСonfigured = true;
    toSend = nullptr;
}

TaskManager::TaskManager(TaskParameters* settings) {
    isСonfigured = true;
    params = settings;
    toSend = nullptr;
}

TaskManager::TaskManager() {
    isСonfigured = false;
    params = nullptr;
    toSend = nullptr;
}

QList<double> TaskManager::parseInitialValues() {
    QStringList args = params->initialConditions.split(' ', Qt::SkipEmptyParts);
    QList<double> dArgs;
    for (const QString& arg : args) {
        bool ok;
        double number = arg.toDouble(&ok);
        if (ok) {
            dArgs.append(number);
        }
    }
    return dArgs;
}

DataTransferObj& TaskManager::getSolution() {
    if (!isСonfigured) {
        toSend->_type = -1;
        toSend->errMsg = "The initial values ​​are not initialized!";
        return *toSend;
    }

    QList<double> dArgs = parseInitialValues();

    if (dArgs.length() != 2) {
        toSend->_type = -1;
        toSend->errMsg = "The initial values ​​are not initialized!";
        return *toSend;
    }

    bool isValid = true;
    switch(params->taskTypeInd) {
    case 0:
        toSend = new TableRK4test();
        break;
    case 1:
        toSend = new TableRK4_1task();
        break;
    case 2:

        break;

    default:
        isValid = false;
        break;
    }

    if (!isValid) {
        toSend->_type = -1;
        toSend->errMsg = "Unknown task type!";
        return *toSend;
    }

    if (params->error_rate) {
        toSend->solveWithControl(
            dArgs.at(0), dArgs.at(1),
            params->initialStep,
            params->rightborder,
            params->nMax,
            params->epsilon,
            params->boundaryPrecision
            );
    } else {
        toSend->solveWithoutControl(
            dArgs.at(0), dArgs.at(1),
            params->initialStep,
            params->rightborder,
            params->nMax,
            params->boundaryPrecision
            );
    }

    return *toSend;
}
