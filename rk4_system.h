#pragma once
#include <cmath>
#include <list>
#include <vector>
#include "functions.h"
using std::list;
using std::pair;
using std::vector;



//Рунге Кутта 4 порядка для системы
vector<double> rk4_system(double x0, double u01, double u02, double h0, system_du system)
{
	pair<double, double>k1 = system(x0, u01, u02);
	pair<double, double>k2 = system(x0 + h0 / 2, u01 + h0 / 2 * k1.first, u02 + h0 / 2 * k1.second);
	pair<double, double>k3 = system(x0 + h0 / 2, u01 + h0 / 2 * k2.first, u02 + h0 / 2 * k2.second);
	pair<double, double>k4 = system(x0 + h0, u01 + h0 * k3.first, u02 + h0 * k3.second);
	pair<double, double> result = { u01 + h0 / 6 * (k1.first + 2 * k2.first + 2 * k3.first + k4.first), u02 + h0 / 6 * (k1.second + 2 * k2.second + 2 * k3.second + k4.second) };
	double x1 = x0 + h0;
	return { x1, result.first, result.second };
}

//Численное решение без контроля локальной погрешности.
//x0 - нач. знач. икса, u01 - первое нач. знач. решения, u02 - второе нач. знач. решения, h0 - шаг интегрирования, system - система 2 порядка ДУ, B - гранциа вычисления, Hmax - максимальное число шагов, Egr - контроль выхода за границу
vector<vector<double>> numericalSolutionSystem(double x0, double u01, double u02, double h0, system_du system, double B, int Hmax,  double Egr = 0.001)
{
    vector<vector<double>> finalResult;
	finalResult.push_back({ x0,u01,u02 });
	for (int i = 0; i < Hmax; i++)
	{
		finalResult.push_back(rk4_system(finalResult.back()[0], finalResult.back()[1], finalResult.back()[2], h0, system)); //добавляем элемент (Xn, Vn) в вектор численной траектории

		//Проверяем выход за границу(в видосе так сказано делать)
		if (finalResult.back()[0] >= (B - Egr) && finalResult.back()[0] <= B)
		{
			break;
		}
		else if (finalResult.back()[0] > B)
		{
			finalResult.pop_back();
			finalResult.push_back(rk4_system(finalResult.back()[0], finalResult.back()[1], finalResult.back()[2], B - finalResult.back()[0] - Egr, system)); //пока таким способом, то есть если вы вышли за границу то послдений Xn = b - Egr
			break;
		}
	}
	return finalResult;
}

//Численное решение с контролем локальной погрешности.
//x0 - нач. знач. икса, u01 - первое нач. знач. решения, u02 - второе нач. знач. решения, h0 - шаг интегрирования, system - система 2 порядка ДУ, B - гранциа вычисления, Hmax - максимальное число шагов, Egr - контроль выхода за границу
vector<vector<double>> numericalSolutionSystemWithControl(double x0, double u01, double u02, double h0, system_du system, double B, int Hmax, double E, double Egr = 0.001)
{
    vector<vector<double>> finalResult;
    finalResult.push_back({ x0,u01,u02 });


    double H = h0;
    for (int i = 0; i < Hmax; i++)
    {
        //считаем 1 большой шаг, и 2 коротких шага
        vector<double> result_simpleTurn = rk4_system(finalResult.back()[0], finalResult.back()[1], finalResult.back()[2], H, system);
        vector<double> result_halfTurn = rk4_system(finalResult.back()[0], finalResult.back()[1], finalResult.back()[2], H / 2, system);
        vector<double> resuslt_2_halfTurn = rk4_system(result_halfTurn[0], result_halfTurn[1], result_halfTurn[2], (H / 2), system);

        double S1 = (resuslt_2_halfTurn[1] - result_simpleTurn[1]) / (pow(2, 4) - 1); 
        double S2 = (resuslt_2_halfTurn[2] - result_simpleTurn[2]) / (pow(2, 4) - 1); 
		double S = std::max(fabs(S1), fabs(S2)); //норма вектора S
        //выбираем что делать дальше
        if ((E / pow(2, 5)) <= S && S <= E)//значение в пределах допустмиого, принимаем и идём дальше
        {
            finalResult.push_back(result_simpleTurn);
        }
        else if (S < (E / pow(2, 5))) //значение слишком точное, принимаем и удваиваем шаг
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

        //Проверяем выход за границу(в видосе так сказано делать)
		if (finalResult.back()[0] >= (B - Egr) && finalResult.back()[0] <= B)
		{
			break;
		}
		else if (finalResult.back()[0] > B)
		{
			finalResult.pop_back();
			finalResult.push_back(rk4_system(finalResult.back()[0], finalResult.back()[1], finalResult.back()[2], B - finalResult.back()[0] - Egr, system)); //пока таким способом, то есть если вы вышли за границу то послдений Xn = b - Egr
			break;
		}
    }
    return finalResult;
}
