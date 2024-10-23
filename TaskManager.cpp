#include "TaskManager.h"
#include "rk4.h"

void TaskManager::setParams(TaskParameters* settings) {
    params = settings;
    isСonfigured = true;
}

TaskManager::TaskManager(TaskParameters* settings) {
    isСonfigured = true;
    params = settings;
    toSend = DataTransferObj();
}

TaskManager::TaskManager() {
    isСonfigured = false;
    params = nullptr;
    toSend = DataTransferObj();
}

void TaskManager::parseSolution(const std::vector<std::vector<double>>& numSol) {
    for (unsigned long long i = 0; i < numSol.size(); i++) {
        toSend.xData.append(numSol.at(i).at(0));
        toSend.numericalSolutionData.append(numSol.at(i).at(1));
    }
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
        toSend._type = -1;
        toSend.errMsg = "The initial values ​​are not initialized!";
        return toSend;
    }

    QList<double> dArgs = parseInitialValues();

    if (dArgs.length() != 3) {
        toSend._type = -1;
        toSend.errMsg = "The initial values ​​are not initialized!";
        return toSend;
    }

    switch(params->taskTypeInd) {
    case 0:
        parseSolution(numericalSolutionWithControl(
            dArgs.at(0), dArgs.at(1), params->initialStep, testFuncDu, dArgs.at(2),
            params->nMax, params->epsilon, params->boundaryPrecision));
        toSend.trueSolutionData = getTrueSolution(toSend.xData, testUTrue);
        return toSend;
        break;

    case 1:
        break;

    case 2:
        break;

    default:
        break;
    }

    toSend._type = -1;
    toSend.errMsg = "Unknown task type!";
    return toSend;
}
