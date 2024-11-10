#ifndef TASKPARAMETERS_H
#define TASKPARAMETERS_H

#include <QString>

class TaskParameters
/*

 * TaskParameters предназначен для хранения и передачи параметров,
 * необходимых для проведения расчётов в TaskManager. Обеспечивает
 * унифицированный интерфейс для доступа к различным данным, требуемым
 * для настройки и выполнения математических задач.
 *
 * Public поля:
 * - Поле taskType. Определяет тип задачи (возможные значения: тестовая,
 *   основная №1, основная №2) (тип данных: QString);
 * - Поле initialConditions. Хранит начальные условия для задачи
 *   (тип данных: QString);
 * - Поле nMax. Определяет максимальное количество шагов для алгоритма
 *   (тип данных: int);
 * - Поле epsilon. Устанавливает допустимую локальную погрешность для
 *   алгоритма (тип данных: double);
 * - Поле boundaryPrecision. Определяет точность при достижении границы
 *   области решения (тип данных: double);
 * - Поле initialStep. Начальный шаг для расчётов (тип данных: double);
 * - Поле taskTypeInd. Индекс задачи, указывающий на тип задачи
 *   (0 - тестовая, 1 - основная №1, 2 - основная №2) (тип данных: int);
 * - Поле error_rate. Индикатор, учитывается ли локальная погрешность
 *   в расчётах (тип данных: bool);
 * - Поле rightborder. Правая граница области расчётов (тип данных: double);
 * - Поля additionalParam1 и additionalParam2. Дополнительные параметры,
 *   предоставляющие дополнительную информацию для расчётов
 *   (тип данных: double).
 *
 * Public методы:
 * - Метод toString. Возвращает строковое представление полей класса,
 *   упрощая отладку и логгирование параметров задачи (тип данных: QString).

 */
{
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
