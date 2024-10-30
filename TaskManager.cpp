#include "TaskManager.h"
#include "table_rk4.h"
#include "table_rk4_system.h"
#include "rk4_system.h"

void TaskManager::setParams(TaskParameters* settings) {
    params = settings;
    isСonfigured = true;
    toSend = nullptr;
}

TaskManager::TaskManager(TaskParameters* settings) {
    isСonfigured = true;
    params = settings;
    toSend = new DataTransferObj();
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

    if ((dArgs.length() != 3 && params->taskTypeInd == 2) || dArgs.length() < 2) {
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

    execute(dArgs);
    return *toSend;
}

void TaskManager::execute(const QList<double>& args) {
    if (params->taskTypeInd != 2) {
        if (params->error_rate) {
            toSend->solveWithControl(
                args.at(0), args.at(1),
                params->initialStep,
                params->rightborder,
                params->nMax,
                params->epsilon,
                params->boundaryPrecision
                );
        } else {
            toSend->solveWithoutControl(
                args.at(0), args.at(1),
                params->initialStep,
                params->rightborder,
                params->nMax,
                params->boundaryPrecision
                );
        }
    } else {
        auto sys = system_du(params->additionalParam1, params->additionalParam2);
        auto tmp = TableRK4_2task();
        if (params->error_rate) {
            tmp.solveWithControl(
                args.at(0),
                args.at(1),
                args.at(2),
                params->initialStep,
                sys,
                params->rightborder,
                params->nMax,
                params->epsilon,
                params->boundaryPrecision
            );
        } else {
            tmp.solveWithoutControl(
                args.at(0),
                args.at(1),
                args.at(2),
                params->initialStep,
                sys,
                params->rightborder,
                params->nMax,
                params->boundaryPrecision
            );
        }
        toSend = new DataTransferObj(tmp);
    }
}
