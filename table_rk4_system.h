#pragma once
#include <vector>
#include <cmath>
#include <list>
#include "rk4_system.h"
#include "DataTransferObj.h"
#include <QList>
using std::vector;
using std::pair;
using std::list;



class TableRK4_2task : public DataTransferObj
{
   
public:
    void solveWithoutControl(const QList<double>& initVals, double h0, double B, int Hmax, double Egr = 0.001) override
    {

        double x0 = initVals[0];
        double u01 = initVals[1];
        double u02 = initVals[2];

        system_du system = system_du(initVals[3], initVals[4]);

        xi.clear();
        vi.clear();
        firstDer.clear();
        diff_vi_v2i.clear();
        resultSteps2.clear();
        olp.clear();
        hi.clear();
        c1.clear();
        c2.clear();

        list<vector<double>> finalResult;
        finalResult.push_back({ x0,u01,u02 });
        for (int i = 0; i < Hmax; i++)
        {
            //считаем 1 большой шаг, и 2 коротких шага
            vector<double> result_simpleTurn = rk4_system(finalResult.back()[0], finalResult.back()[1], finalResult.back()[2], h0, system);
            vector<double> result_halfTurn = rk4_system(finalResult.back()[0], finalResult.back()[1], finalResult.back()[2], h0 / 2, system);
            vector<double> result_2_halfTurn = rk4_system(result_halfTurn[0], result_halfTurn[1], result_halfTurn[2], (h0 / 2), system);

            double S1 = (result_2_halfTurn[1] - result_simpleTurn[1]) / (pow(2, 4) - 1);
            double S2 = (result_2_halfTurn[2] - result_simpleTurn[2]) / (pow(2, 4) - 1);
            double S = std::max(fabs(S1), fabs(S2)); //норма вектора S

            finalResult.push_back(result_simpleTurn); //добавляем элемент (Xn, V1n, V2n) в лист численной траектории

            xi.push_back(finalResult.back()[0]);
            vi.push_back(finalResult.back()[1]);
            firstDer.push_back(finalResult.back()[2]);
            resultSteps2.push_back(result_2_halfTurn[2]);
            diff_vi_v2i.push_back(result_2_halfTurn[2] - result_simpleTurn[2]);
            olp.push_back(S2 * pow(2, 4));
            hi.push_back(h0);
            c1.push_back(0);
            c2.push_back(0);

            //Проверяем выход за границу(в видосе так сказано делать)
            if (finalResult.back()[0] >= (B - Egr) && finalResult.back()[0] <= B)
            {
                break;
            }
            else if (finalResult.back()[0] > B)
            {
                xi.pop_back();
                vi.pop_back();
                firstDer.pop_back();
                resultSteps2.pop_back();
                diff_vi_v2i.pop_back();
                olp.pop_back();
                hi.pop_back();

                double h1 = B - finalResult.back()[0] - Egr;

                vector<double> result_simpleTurn = rk4_system(finalResult.back()[0], finalResult.back()[1], finalResult.back()[2], h1, system);
                vector<double> result_halfTurn = rk4_system(finalResult.back()[0], finalResult.back()[1], finalResult.back()[2], h1 / 2, system);
                vector<double> result_2_halfTurn = rk4_system(result_halfTurn[0], result_halfTurn[1], result_halfTurn[2], (h1 / 2), system);

                double S1 = (result_2_halfTurn[1] - result_simpleTurn[1]) / (pow(2, 4) - 1);
                double S2 = (result_2_halfTurn[2] - result_simpleTurn[2]) / (pow(2, 4) - 1);
                double S = std::max(fabs(S1), fabs(S2)); //норма вектора S

                xi.push_back(finalResult.back()[0]);
                vi.push_back(finalResult.back()[1]);
                firstDer.push_back(finalResult.back()[2]);
                resultSteps2.push_back(result_2_halfTurn[2]);
                diff_vi_v2i.push_back(result_2_halfTurn[2] - result_simpleTurn[2]);
                olp.push_back(S2 * pow(2, 4));
                hi.push_back(h1);
                break;
            }
        }
    }

    void solveWithControl(const QList<double>& initVals, double h0, double B, int Hmax, double E, double Egr = 0.001) override
    {

        double x0 = initVals[0];
        double u01 = initVals[1];
        double u02 = initVals[2];

        system_du system = system_du(initVals[3], initVals[4]);

        vi.clear();
        xi.clear();
        firstDer.clear();
        diff_vi_v2i.clear();
        resultSteps2.clear();
        olp.clear();
        hi.clear();
        c1.clear();
        c2.clear();


        int c1count = 0;
        int c2count = 0;
        double H = h0;
        list<vector<double>> finalResult;
        finalResult.push_back({ x0,u01,u02 });

        for (int i = 0; i < Hmax; i++)
        {
            //считаем 1 большой шаг, и 2 коротких шага
            vector<double> result_simpleTurn = rk4_system(finalResult.back()[0], finalResult.back()[1], finalResult.back()[2], H, system);
            vector<double> result_halfTurn = rk4_system(finalResult.back()[0], finalResult.back()[1], finalResult.back()[2], H / 2, system);
            vector<double> result_2_halfTurn = rk4_system(result_halfTurn[0], result_halfTurn[1], result_halfTurn[2], (H / 2), system);

            double S1 = (result_2_halfTurn[1] - result_simpleTurn[1]) / (pow(2, 4) - 1);
            double S2 = (result_2_halfTurn[2] - result_simpleTurn[2]) / (pow(2, 4) - 1);
            double S = std::max(fabs(S1), fabs(S2));

            double hOld = H;
            //выбираем что делать дальше
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

            finalResult.push_back(result_simpleTurn); //добавляем элемент (Xn, V1n, V2n) в лист численной траектории

            xi.push_back(finalResult.back()[0]);
            vi.push_back(finalResult.back()[1]);
            firstDer.push_back(finalResult.back()[2]);
            resultSteps2.push_back(result_2_halfTurn[2]);
            diff_vi_v2i.push_back(result_2_halfTurn[2] - result_simpleTurn[2]);
            olp.push_back(S2 * pow(2, 4));
            hi.push_back(hOld);
            c1.push_back(c1count);
            c2.push_back(c2count);



            //Проверяем выход за границу(в видосе так сказано делать)
            if (finalResult.back()[0] >= (B - Egr) && finalResult.back()[0] <= B)
            {
                break;
            }
            else if (finalResult.back()[0] > B)
            {

                finalResult.pop_back();

                xi.pop_back();
                vi.pop_back();
                firstDer.pop_back();
                resultSteps2.pop_back();
                diff_vi_v2i.pop_back();
                olp.pop_back();
                hi.pop_back();

                double h1 = B - finalResult.back()[0] - Egr;

                vector<double> result_simpleTurn = rk4_system(finalResult.back()[0], finalResult.back()[1], finalResult.back()[2], h1, system);
                vector<double> result_halfTurn = rk4_system(finalResult.back()[0], finalResult.back()[1], finalResult.back()[2], h1 / 2, system);
                vector<double> result_2_halfTurn = rk4_system(result_halfTurn[0], result_halfTurn[1], result_halfTurn[2], (h1 / 2), system);

                double S1 = (result_2_halfTurn[1] - result_simpleTurn[1]) / (pow(2, 4) - 1);
                double S2 = (result_2_halfTurn[2] - result_simpleTurn[2]) / (pow(2, 4) - 1);
                double S = std::max(fabs(S1), fabs(S2)); //норма вектора S

                finalResult.push_back(result_simpleTurn);

                xi.push_back(finalResult.back()[0]);
                vi.push_back(finalResult.back()[1]);
                firstDer.push_back(finalResult.back()[2]);
                resultSteps2.push_back(result_2_halfTurn[2]);
                diff_vi_v2i.push_back(result_2_halfTurn[2] - result_simpleTurn[2]);
                olp.push_back(S2 * pow(2, 4));
                hi.push_back(h1);
                break;
            }
        }
    }

};
