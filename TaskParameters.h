#ifndef TASKPARAMETERS_H
#define TASKPARAMETERS_H

#include <QString>

class TaskParameters {
public:
    QString taskType;           // Тип задачи (тестовая, основная №1, основная №2)
    QString initialConditions;  // Начальные условия
    int nMax;                   // Максимальное количество шагов
    double epsilon;             // Контроль локальной погрешности
    double boundaryPrecision;   // Точность выхода на границу
    double initialStep;         // Начальный шаг
    int taskTypeInd; // Индекс задачи (0 - тестовая, 1 - основная №1, 2 - основная #2)
    bool error_rate; // C учётом локальной погрешности и без
    double rightborder; // Правая граница
    double additionalParam1, additionalParam2; //Дополнительные параметры

    // Функция для вывода данных (например, для отладки)
    QString toString() const {
        return QString("Task: %1\nInitial Conditions: %2\nNmax: %3\nEpsilon: %4\nBoundary Precision: %5\nInitial Step: %6\nTask index: %7")
        .arg(taskType)
            .arg(initialConditions)
            .arg(nMax)
            .arg(epsilon)
            .arg(boundaryPrecision)
            .arg(initialStep)
            .arg(taskTypeInd);
    }
};

#endif // TASKPARAMETERS_H
