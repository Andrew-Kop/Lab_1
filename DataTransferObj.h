#pragma once
#ifndef DATATRANSFEROBJ_H
#define DATATRANSFEROBJ_H
#include <QString>
#include <QVector>

class DataTransferObj
/*
 * Вспомогательный класс для хранения и передачи результатов
 * работы. Является родительским классом для математических и
 * выполняет для них роль хранилища данных. Также содержит виртуальные
 * методы для последующего приведения классов-наследников к одному виду (upcast).
 *
 * Public методы и поля:
 * - Поле _type. Означает "тип" объекта для экземпляра класса, например, при _type = -1
 *  объект содержит ошибку (например, вычисления по какой-то причине прервались). Необходим
 *  для обработки ошибок (тип данных: int);
 * - Поле errMsg. Вспомогательное поле, отвечает за хранение сообщения об ошибке (при _type = -1)
 *  (тип данных: QString).
 * - Поле vi. Содержит значения, рассчитаные для единичной сетки (тип данных: QVector<double>).
 * - Поле xi. Содержит координаты точек траектории по оси абсцисс (тип данных: QVector<double>).
 * - Поле resultSteps2. Содержит значения, рассчитаные для двойной сетки (тип данных: QVector<double>).
 * - Поле diff_vi_v2i. Содержит разницу между vi и resultSteps2 (тип данных: QVector<double>).
 * - Поле olp. Содержит значения локальной погрешности (тип данных: QVector<double>).
 * - Поле hi. Содержит значения шага (тип данных: QVector<double>).
 * - Поле c1. Содержит число делений шага в каждой точке (тип данных: QVector<double>).
 * - Поле c2. Содержит число удвоений шага в каждой точке (тип данных: QVector<double>).
 * - Поле ui. Содержит значения истинного решения (для тестовой задачи) (тип данных: QVector<double>).
 * - Поле diff_ui_vi. Содержит разницу между ui и vi (для тестовой задачи) (тип данных: QVector<double>).
 * - Поле firstDer. Содержит значения первой производной (для второй основной задачи) (тип данных: QVector<double>).
 *
 * - Конструктор DataTransferObj.
 * - Виртуальный метод solveWithoutControl.
 * - Виртуальный метод solveWithControl.
 * - Виртуальный деструктор ~DataTransferObj.
 * - Перегрузка оператора присваивания.
*/
{
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
        errMsg(err)
    {}
    /*
     * Конструктор, вызывается при создании экземпляра класса, инициализирует поля.
     * Опционально принимает аргументы:
     * - type - приравнивается к полю _type, по умолчанию = 0 (тип данных: int);
     * - err - приравнивается к полю errMsg, по умолчанию = "" (тип данных: QString).
     * Ничего не возвращает.
    */

    virtual void solveWithoutControl(const QList<double>& initVals, double h0, double B, int Hmax, double Egr) {}
    /*
     * Виртуальный метод, необходим для соответствующей перегрузки в математическом классе-наследнике.
     * Принимает аналогичные аргументы:
     * - initVals - начальные значения для расчётов (тип данных: const QList<double>&).
     * - h0 - начальный шаг (тип данных: double).
     * - B - правая граница вычислений (тип данных: double).
     * - Hmax - максимальное кол-во шагов (тип данных: int).
     * - Egr - точность выхода на границу (тип данных: double).
     * Ничего не возвращает (работает с полями).
    */
    virtual void solveWithControl(const QList<double>& initVals, double h0, double B, int Hmax, double E, double Egr) {}
    /*
     * Виртуальный метод, необходим для соответствующей перегрузки в математическом классе-наследнике.
     * Принимает аналогичные аргументы:
     * - initVals - начальные значения для расчётов (тип данных: const QList<double>&).
     * - h0 - начальный шаг (тип данных: double).
     * - B - правая граница вычислений (тип данных: double).
     * - Hmax - максимальное кол-во шагов (тип данных: int).
     * - E - параметр контроля погрешности (тип данных: double).
     * - Egr - точность выхода на границу (тип данных: double).
     * Ничего не возвращает (работает с полями).
    */

    virtual ~DataTransferObj() {}
    // Виртуальный деструктор, вызывается при уничтожении экземпляра класса.

    DataTransferObj& operator=(const DataTransferObj& obj)
    /*
     * Перегрузка оператора. Необходима для передачи текущему экземпляру
     * класса значений присваемого. Аргумент:
     * - obj - объект, из которого берутся значения (тип данных: const DataTransferObj&).
     * Возвращает ссылку на текущий экземпляр (тип данных: DataTransferObj&).
    */
    {
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
