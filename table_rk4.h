#pragma once
#include <vector>
#include <cmath>
#include <list>
#include "rk4.h"
#include "DataTransferObj.h"
using std::vector;
using std::pair;
using std::list;

double uTestRed(double x1, double x0, double u0)
{
    double c = u0 / std::exp((-7.0 / 2) * x0);
    return c*std::exp((-7.0 / 2) * x1);
}

double func1Du(double x0, double v0)
{
    return pow(v0, 2) / (1 + 3 * x0 + pow(x0, 2)) + v0 - pow(v0, 3) * sin(10 * x0);
}

class TableRK4test : public DataTransferObj
{
private:
    pair<double,double> RK4(double x0, double v0, double h0, double (*func)(double, double))
    {
        double k1 = func(x0, v0);
        double k2 = func(x0 + h0 / 2, v0 + (h0 * k1) / 2);
        double k3 = func(x0 + h0 / 2, v0 + (h0 * k2) / 2);
        double k4 = func(x0 + h0, v0 + h0 * k3);

        double v1 = v0 + h0 * (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        double x1 = x0 + h0;

        pair<double,double> result = { x1, v1 };
        return result;
    }
    
public:

    void solveWithoutControl(const QList<double>& initVals, double h0, double B, int Hmax, double Egr = 0.001) override
	{

        double x0 = initVals[0];
        double u0 = initVals[1];

        xi.clear();
        vi.clear();
        resultSteps2.clear();
        diff_vi_v2i.clear();
        olp.clear();
        hi.clear();
        c1.clear();
        c2.clear();
        ui.clear();
        diff_ui_vi.clear();

        xi.push_back(x0);
        vi.push_back(u0);
        for (int i = 0; i < Hmax; i++)
        {
            //считаем 1 большой шаг, и 2 коротких шага
            pair<double,double> result_simpleTurn = RK4(xi.back(), vi.back(), h0, testFuncDu);
            pair<double,double> result_halfTurn = RK4(xi.back(), vi.back(), (h0 / 2), testFuncDu);
            pair<double,double> result_2_halfTurn = RK4(result_halfTurn.first, result_halfTurn.second, (h0 / 2), testFuncDu);

            xi.push_back(result_simpleTurn.first); //добавляем элемент (Xn, Vn) в вектор численной траектории
            vi.push_back(result_simpleTurn.second);
            resultSteps2.push_back(result_2_halfTurn.second);
            diff_vi_v2i.push_back(result_2_halfTurn.second - result_simpleTurn.second);

            double S = (result_2_halfTurn.second - result_simpleTurn.second) / (pow(2, 4) - 1);
            olp.push_back(S * pow(2, 4));
            hi.push_back(h0);
            c1.push_back(0);
            c2.push_back(0);
            ui.push_back(uTestRed(result_simpleTurn.first, x0, u0));
            diff_ui_vi.push_back(fabs(uTestRed(result_simpleTurn.first, x0, u0) - result_simpleTurn.second));


            //Проверяем выход за границу(в видосе так сказано делать)
            if (xi.back() >= (B - Egr) && xi.back() <= B)
            {
                break;
            }
            else if (xi.back() > B)
            {
                xi.pop_back();
                vi.pop_back();
                resultSteps2.pop_back();
                diff_vi_v2i.pop_back();
                olp.pop_back();
                hi.pop_back();
                
                ui.pop_back();
                diff_ui_vi.pop_back();

                double h1 = B - xi.back() - Egr;

                pair<double, double> result_simpleTurn = RK4(xi.back(), vi.back(), h1, testFuncDu);
                pair<double, double> result_halfTurn = RK4(xi.back(), vi.back(), (h1 / 2), testFuncDu);
                pair<double, double> result_2_halfTurn = RK4(result_halfTurn.first, result_halfTurn.second, (h1 / 2), testFuncDu);

                double S = (result_2_halfTurn.second - result_simpleTurn.second) / (pow(2, 4) - 1);

                xi.push_back(result_simpleTurn.first); //пока таким способом, то есть если вы вышли за границу то послдений Xn = b - Egr
                vi.push_back(result_simpleTurn.second);
                resultSteps2.push_back(result_2_halfTurn.second);
                diff_vi_v2i.push_back(result_2_halfTurn.second - result_simpleTurn.second);
                olp.push_back(S * pow(2, 4));
                hi.push_back(h0);
                
                ui.push_back(uTestRed(result_simpleTurn.first, x0, u0));
                diff_ui_vi.push_back(fabs(uTestRed(result_simpleTurn.first, x0, u0) - result_simpleTurn.second));

                break;
            }
        }
        xi.pop_front();
        vi.pop_front();
	}

    void solveWithControl(const QList<double>& initVals, double h0, double B, int Hmax, double E, double Egr = 0.001) override
    {

        double x0 = initVals[0];
        double u0 = initVals[1];

        xi.clear();
        vi.clear();
        resultSteps2.clear();
        diff_vi_v2i.clear();
        olp.clear();
        hi.clear();
        c1.clear();
        c2.clear();
        ui.clear();
        diff_ui_vi.clear();

        int c1count = 0;
        int c2count = 0;
        double H = h0;

        xi.push_back(x0);
        vi.push_back(u0);

        for (int i = 0; i < Hmax; i++)
        {
            //считаем 1 большой шаг, и 2 коротких шага
            pair<double, double> result_simpleTurn = RK4(xi.back(), vi.back(), H, testFuncDu);
            pair<double, double> result_halfTurn = RK4(xi.back(), vi.back(), (H / 2), testFuncDu);
            pair<double, double> result_2_halfTurn = RK4(result_halfTurn.first, result_halfTurn.second, (H / 2), testFuncDu);

            double S = (result_2_halfTurn.second - result_simpleTurn.second) / (pow(2, 4) - 1);

            //выбираем что делать дальше
            double h0 = H;
            if (fabs(S) < (E / pow(2, 5))) //значение слишком точное, принимаем и удваиваем шаг
            {
                H = H * 2;
                c2count++;
            }
            else if (fabs(S) >= E) //значение слишком неточное, уменьшаем шаг в два раза и пересчитываем
            {
                H = H / 2;
                c1count++;
                i--;
                continue;
            }

            xi.push_back(result_simpleTurn.first); //добавляем элемент (Xn, Vn) в вектор численной траектории
            vi.push_back(result_simpleTurn.second);
            resultSteps2.push_back(result_2_halfTurn.second);
            diff_vi_v2i.push_back(result_2_halfTurn.second - result_simpleTurn.second);
            olp.push_back(S * pow(2, 4));
            hi.push_back(h0);
            c1.push_back(c1count);
            c2.push_back(c2count);
            ui.push_back(uTestRed(result_simpleTurn.first, x0, u0));
            diff_ui_vi.push_back(fabs(uTestRed(result_simpleTurn.first, x0, u0) - result_simpleTurn.second));


            //Проверяем выход за границу(в видосе так сказано делать)
            if (xi.back() >= (B - Egr) && xi.back() <= B)
            {
                break;
            }
            else if (xi.back() > B)
            {
                xi.pop_back();
                vi.pop_back();
                resultSteps2.pop_back();
                diff_vi_v2i.pop_back();
                olp.pop_back();
                hi.pop_back();
                
                ui.pop_back();
                diff_ui_vi.pop_back();

                double h1 = B - xi.back() - Egr;

                pair<double, double> result_simpleTurn = RK4(xi.back(), vi.back(), h1, testFuncDu);
                pair<double, double> result_halfTurn = RK4(xi.back(), vi.back(), (h1 / 2), testFuncDu);
                pair<double, double> result_2_halfTurn = RK4(result_halfTurn.first, result_halfTurn.second, (h1 / 2), testFuncDu);

                double S = (result_2_halfTurn.second - result_simpleTurn.second) / (pow(2, 4) - 1);

                //пока таким способом, то есть если вы вышли за границу то послдений Xn = b - Egr
                xi.push_back(result_simpleTurn.first);
                vi.push_back(result_simpleTurn.second);
                resultSteps2.push_back(result_2_halfTurn.second);
                diff_vi_v2i.push_back(result_2_halfTurn.second - result_simpleTurn.second);
                olp.push_back(S * pow(2, 4));
                hi.push_back(h0);
                
                ui.push_back(uTestRed(result_simpleTurn.first, x0, u0));
                diff_ui_vi.push_back(fabs(uTestRed(result_simpleTurn.first, x0, u0) - result_simpleTurn.second));

                break;
            }
        }
        xi.pop_front();
        vi.pop_front();
    }

};

class TableRK4_1task : public DataTransferObj
{
private:
    pair<double, double> RK4(double x0, double v0, double h0, double (*func)(double, double))
    {
        double k1 = func(x0, v0);
        double k2 = func(x0 + h0 / 2, v0 + (h0 * k1) / 2);
        double k3 = func(x0 + h0 / 2, v0 + (h0 * k2) / 2);
        double k4 = func(x0 + h0, v0 + h0 * k3);

        double v1 = v0 + h0 * (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        double x1 = x0 + h0;

        pair<double, double> result = { x1, v1 };
        return result;
    }

public:

    void solveWithoutControl(const QList<double>& initVals, double h0, double B, int Hmax, double Egr = 0.001) override
    {

        double x0 = initVals[0];
        double u0 = initVals[1];

        xi.clear();
        vi.clear();
        resultSteps2.clear();
        diff_vi_v2i.clear();
        olp.clear();
        hi.clear();
        c1.clear();
        c2.clear();

        xi.push_back(x0);
        vi.push_back(u0);
        for (int i = 0; i < Hmax; i++)
        {
            //считаем 1 большой шаг, и 2 коротких шага
            pair<double, double> result_simpleTurn = RK4(xi.back(), vi.back(), h0, testFuncDu);
            pair<double, double> result_halfTurn = RK4(xi.back(), vi.back(), (h0 / 2), testFuncDu);
            pair<double, double> result_2_halfTurn = RK4(result_halfTurn.first, result_halfTurn.second, (h0 / 2), testFuncDu);

            xi.push_back(result_simpleTurn.first); //добавляем элемент (Xn, Vn) в вектор численной траектории
            vi.push_back(result_simpleTurn.second);
            resultSteps2.push_back(result_2_halfTurn.second);
            diff_vi_v2i.push_back(result_2_halfTurn.second - result_simpleTurn.second);

            double S = (result_2_halfTurn.second - result_simpleTurn.second) / (pow(2, 4) - 1);
            olp.push_back(S * pow(2, 4));
            hi.push_back(h0);
            c1.push_back(0);
            c2.push_back(0);
            


            //Проверяем выход за границу(в видосе так сказано делать)
            if (xi.back() >= (B - Egr) && xi.back() <= B)
            {
                break;
            }
            else if (xi.back() > B)
            {
                xi.pop_back();
                vi.pop_back();
                resultSteps2.pop_back();
                diff_vi_v2i.pop_back();
                olp.pop_back();
                hi.pop_back();
                
                double h1 = B - xi.back() - Egr;

                pair<double, double> result_simpleTurn = RK4(xi.back(), vi.back(), h1, testFuncDu);
                pair<double, double> result_halfTurn = RK4(xi.back(), vi.back(), (h1 / 2), testFuncDu);
                pair<double, double> result_2_halfTurn = RK4(result_halfTurn.first, result_halfTurn.second, (h1 / 2), testFuncDu);

                double S = (result_2_halfTurn.second - result_simpleTurn.second) / (pow(2, 4) - 1);

                xi.push_back(result_simpleTurn.first); //пока таким способом, то есть если вы вышли за границу то послдений Xn = b - Egr
                vi.push_back(result_simpleTurn.second);
                resultSteps2.push_back(result_2_halfTurn.second);
                diff_vi_v2i.push_back(result_2_halfTurn.second - result_simpleTurn.second);
                olp.push_back(S * pow(2, 4));
                hi.push_back(h1);
                
                

                break;
            }
        }
        xi.pop_front();
        vi.pop_front();
    }

    void solveWithControl(const QList<double>& initVals, double h0, double B, int Hmax, double E, double Egr = 0.001) override
    {

        double x0 = initVals[0];
        double u0 = initVals[1];

        xi.clear();
        vi.clear();
        resultSteps2.clear();
        diff_vi_v2i.clear();
        olp.clear();
        hi.clear();
        c1.clear();
        c2.clear();
        

        int c1count = 0;
        int c2count = 0;
        double H = h0;
        xi.push_back(x0);
        vi.push_back(u0);

        for (int i = 0; i < Hmax; i++)
        {
            //считаем 1 большой шаг, и 2 коротких шага
            pair<double, double> result_simpleTurn = RK4(xi.back(), vi.back(), H, testFuncDu);
            pair<double, double> result_halfTurn = RK4(xi.back(), vi.back(), (H / 2), testFuncDu);
            pair<double, double> result_2_halfTurn = RK4(result_halfTurn.first, result_halfTurn.second, (H / 2), testFuncDu);

            double S = (result_2_halfTurn.second - result_simpleTurn.second) / (pow(2, 4) - 1);

            //выбираем что делать дальше
            double h0 = H;
            if (fabs(S) < (E / pow(2, 5))) //значение слишком точное, принимаем и удваиваем шаг
            {
                H = H * 2;
                c2count++;
            }
            else if(fabs(S) >= E) //значение слишком неточное, уменьшаем шаг в два раза и пересчитываем
            {
                H = H / 2;
                c1count++;
                i--;
                continue;
            }

            xi.push_back(result_simpleTurn.first);
            vi.push_back(result_simpleTurn.second);
            resultSteps2.push_back(result_2_halfTurn.second);
            diff_vi_v2i.push_back(result_2_halfTurn.second - result_simpleTurn.second);
            olp.push_back(S * pow(2, 4));
            hi.push_back(h0);
            c1.push_back(c1count);
            c2.push_back(c2count);
            


            //Проверяем выход за границу(в видосе так сказано делать)
            if (xi.back() >= (B - Egr) && xi.back() <= B)
            {
                break;
            }
            else if (xi.back() > B)
            {
                xi.pop_back();
                vi.pop_back();
                resultSteps2.pop_back();
                diff_vi_v2i.pop_back();
                olp.pop_back();
                hi.pop_back();
               
               

                double h1 = B - xi.back() - Egr;

                pair<double, double> result_simpleTurn = RK4(xi.back(), vi.back(), h1, testFuncDu);
                pair<double, double> result_halfTurn = RK4(xi.back(), vi.back(), (h1 / 2), testFuncDu);
                pair<double, double> result_2_halfTurn = RK4(result_halfTurn.first, result_halfTurn.second, (h1 / 2), testFuncDu);

                double S = (result_2_halfTurn.second - result_simpleTurn.second) / (pow(2, 4) - 1);

                xi.push_back(result_simpleTurn.first);//пока таким способом, то есть если вы вышли за границу то послдений Xn = b - Egr
                vi.push_back(result_simpleTurn.second);
                resultSteps2.push_back(result_2_halfTurn.second);
                diff_vi_v2i.push_back(result_2_halfTurn.second - result_simpleTurn.second);
                olp.push_back(S * pow(2, 4));
                hi.push_back(h1);
                
                

                break;
            }
        }
        xi.pop_front();
        vi.pop_front();
    }

};
