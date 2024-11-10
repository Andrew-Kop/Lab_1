#pragma once
#include <vector>
#include <cmath>
#include <QList>

//–унге- утта 4 пор€дка.
std::pair<double,double> RK4(double x0, double v0, double h0, double (*func)(double, double))
{
    double k1 = func(x0, v0);
    double k2 = func(x0 + (h0 / 2), v0 + (h0 / 2) * k1);
    double k3 = func(x0 + (h0 / 2), v0 + (h0 / 2) * k2);
    double k4 = func(x0 + h0, v0 + h0 * k3);

    double v1 = v0 + (h0 / 6) * (k1 + 2 * k2 + 2 * k3 + k4);
    double x1 = x0 + h0;

    std::pair<double,double> result = { x1, v1 };
    return result;
}


//„исленное решение без контрол€ локальной погрешности.
//x0 - нач. знач. икса, v0 - нач. знач. решени€, h0 - шаг интегрировани€, func - функци€ в ƒ”, B - гранциа вычислени€, Hmax - максимальное число шагов, Egr - контроль выхода за границу
std::vector<std::pair<double,double>> numericalSolution(double x0, double u0, double h0, double (*func)(double, double), double B, int Hmax, double Egr = 0.001)
{
    std::vector<std::pair<double,double>> finalResult;
    finalResult.push_back({ x0,u0 });

    for(int i = 0; i < Hmax; i++)
    {
        finalResult.push_back(RK4(finalResult.back().first, finalResult.back().second, h0, func)); //добавл€ем элемент (Xn, Vn) в вектор численной траектории

        //ѕровер€ем выход за границу(в видосе так сказано делать)
        if(finalResult.back().first >= (B - Egr) && finalResult.back().first <= B)
        {
            break;
        }
        else if (finalResult.back().first > B)
        {
            finalResult.pop_back();
            finalResult.push_back(RK4(finalResult.back().first, finalResult.back().second, B - finalResult.back().first - Egr, func)); //пока таким способом, то есть если вы вышли за границу то послдений Xn = b - Egr
            break;
        }
    }
    return finalResult;
}

//„исленное решение с контролем локальной погрешности.
//x0 - нач. знач. икса, v0 - нач. знач. решени€, h0 - шаг интегрировани€, func - функци€ в ƒ”, B - гранциа вычислени€, Hmax - максимальное число шагов, E - погрешность, Egr - контроль выхода за границу
std::vector<std::pair<double,double>> numericalSolutionWithControl(double x0, double u0, double h0, double (*func)(double, double), double B, int Hmax, double E, double Egr = 0.001)
{
    std::vector<std::pair<double,double>> finalResult;
    finalResult.push_back({ x0,u0 });


    double H = h0;
    for (int i = 0; i < Hmax; i++)
    {
        //считаем 1 большой шаг, и 2 коротких шага
        std::pair<double,double> result_simpleTurn = RK4(finalResult.back().first, finalResult.back().second, H, func);
        std::pair<double,double> result_halfTurn = RK4(finalResult.back().first, finalResult.back().second, (H / 2), func);
        std::pair<double,double> resuslt_2_halfTurn = RK4(result_halfTurn.first, result_halfTurn.second, (H / 2), func);

        double S = (resuslt_2_halfTurn.second - result_simpleTurn.second) / (pow(2, 4) - 1); //вычисл€ем S, которое примерно равно (лок. погр.) / (2^p)

        //выбираем что делать дальше
        if ((E / pow(2, 5)) <= fabs(S) && fabs(S) <= E)//значение в пределах допустмиого, принимаем и идЄм дальше
        {
            finalResult.push_back(result_simpleTurn);
        }
        else if (fabs(S) < (E / pow(2, 5))) //значение слишком точное, принимаем и удваиваем шаг
        {
            finalResult.push_back(result_simpleTurn);
            H = H * 2;
        }
        else //значение слишком неточное, уменьшаем шаг в два раза и пересчитываем
        {
            H = H / 2;
            i--;
            continue;
        }

        //ѕровер€ем выход за границу(в видосе так сказано делать)
        if (finalResult.back().first >= (B - Egr) && finalResult.back().first <= B)
        {
            break;
        }
        else if (finalResult.back().first > B)
        {
            finalResult.pop_back();
            finalResult.push_back(RK4(finalResult.back().first, finalResult.back().second,B - finalResult.back().first - Egr, func)); //пока таким способом, то есть если вы вышли за границу то послдений Xn = b - Egr
            break;
        }
    }
    return finalResult;
}
