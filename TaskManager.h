#pragma once
#ifndef TASKMANAGER_H
#define TASKMANAGER_H
#include "DataTransferObj.h"
#include "TaskParameters.h"
#include <QString>
#include <QStringList>
#include <QList>

class TaskManager
/*
 * TaskManager обрабатывает входящие пользовательские данные и
 * принимает решение о применении того или иного математического
 * метода, являясь унифицированным интерфейсом для взаимодействия
 * с ними.
 *
 * Private методы и поля:
 * - Поле toSend. Отвечает за хранение результатов работы математического блока,
 *  для их последующей графической интерпретации (тип данных: DataTransferObj*);
 * - Поле params. Отвечает за хранение входящих данных (тип данных: TaskParameters*);
 * - Поле isСonfigured. Вспомогательное поле для оценки состояния экземпляра класса,
 *  помогает при обработке исключений, вызванных нехваткой входящих данных (тип данных: bool);
 *
 * - Метод parseInitialValues;
 * - Метод execute.
 *
 * Public методы и поля:
 * - Конструктор TaskManager и его перегрузка;
 * - Метод setParams;
 * - Метод getSolution;
 * - Деструктор ~TaskManager.
*/
{
private:
    DataTransferObj* toSend;
    TaskParameters* params;
    bool isСonfigured;

    QList<double> parseInitialValues();
    /*
     * Вспомогательный метод для приведения входящих значений к требуемому виду.
     * Возвращает список значений типа double (тип данных: QList<double>).
    */

    void execute(const QList<double>& args);
    /*
     * Метод, инициализирующий расчёты в математическом блоке.
     * - args - должен содержать необходимые параметры для проведения расчётов
     * (тип данных: const QList<double>&).
     * Ничего не возвращает (работает с полем toSend).
    */

public:
    TaskManager();
    TaskManager(TaskParameters* settings);
    /*
     * Конструкторы, вызываются при создании экземпляра класса. Ничего не возвращают,
     * инициализируют поля (toSend = new DataTransferObj(), params = nullptr, ifConfigured = false).
     * Разница в аргументе:
     * - settings - содержит объект с входящими данными (тип данных: TaskParameters*),
     * при его наличии, поле isConfigured автоматически становится true, а params
     * принимает значение аргумента.
    */

    void setParams(TaskParameters* settings);
    /*
     * Метод, взаимодействующий с полем params, присваивает ему новое значение.
     * Также меняет поле isConfigured на true.
     * Аргументы:
     * - settings - объект с новыми пользовательскими данными (тип данных: TaskParameters*).
     * Ничего не возвращает.
    */

    DataTransferObj& getSolution();
    /*
     * Отвечает за выбор необходимого математического метода,
     * обрабатывает входящие параметры (использует parseInitialValues и params),
     * проверяет наличие ошибок (использует isСonfigured),
     * инициализирует расчёты (использует execute) и возвращает
     * результат (ссылку на поле toSend) (тип данных DataTransferObj&).
    */

    ~TaskManager() {delete toSend;}
    /*
     * Деструктор, вызывается при уничтожении экземпляра класса,
     * вызывает деструктор для поля toSend (поскольку он создан с помощью
     * оператора new). Ничего не возвращает.
    */
};

#endif // TASKMANAGER_H
