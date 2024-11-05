#include "TaskManager.h"
#include "table_rk4.h"
#include "table_rk4_system.h"
#include "rk4_system.h"
#include "functions.h"

void TaskManager::setParams(TaskParameters* settings) {
    params = settings;
    isСonfigured = true;
}

TaskManager::TaskManager(TaskParameters* settings) {
    isСonfigured = true;
    params = settings;
    toSend = new DataTransferObj();
}

TaskManager::TaskManager() {
    isСonfigured = false;
    params = nullptr;
    toSend = new DataTransferObj();
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

    if ((dArgs.length() != 3 && params->taskTypeInd == 2) || dArgs.length() < 2) {
        toSend->_type = -1;
        toSend->errMsg = "The initial values ​​are not initialized!";
        return *toSend;
    }

    if (params->taskTypeInd == 2) {
        dArgs.append(params->additionalParam1);
        dArgs.append(params->additionalParam2);
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
        toSend = new TableRK4_2task();
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

    execute(dArgs);
    return *toSend;
}

void TaskManager::execute(const QList<double>& args) {
    if (params->error_rate) {
        toSend->solveWithControl(
            args,
            params->initialStep,
            params->rightborder,
            params->nMax,
            params->epsilon,
            params->boundaryPrecision
        );
        } else {
            toSend->solveWithoutControl(
            args,
            params->initialStep,
            params->rightborder,
            params->nMax,
            params->boundaryPrecision
        );
     }
}
