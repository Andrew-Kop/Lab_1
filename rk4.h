#pragma once
#include <vector>
#include <cmath>
#include <QList>

//�����-����� 4 �������.
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


//��������� ������� ��� �������� ��������� �����������.
//x0 - ���. ����. ����, v0 - ���. ����. �������, h0 - ��� ��������������, func - ������� � ��, B - ������� ����������, Hmax - ������������ ����� �����, Egr - �������� ������ �� �������
std::vector<std::pair<double,double>> numericalSolution(double x0, double u0, double h0, double (*func)(double, double), double B, int Hmax, double Egr = 0.001)
{
    std::vector<std::pair<double,double>> finalResult;
    finalResult.push_back({ x0,u0 });

    for(int i = 0; i < Hmax; i++)
    {
        finalResult.push_back(RK4(finalResult.back().first, finalResult.back().second, h0, func)); //��������� ������� (Xn, Vn) � ������ ��������� ����������

        //��������� ����� �� �������(� ������ ��� ������� ������)
        if(finalResult.back().first >= (B - Egr) && finalResult.back().first <= B)
        {
            break;
        }
        else if (finalResult.back().first > B)
        {
            finalResult.pop_back();
            finalResult.push_back(RK4(finalResult.back().first, finalResult.back().second, B - finalResult.back().first - Egr, func)); //���� ����� ��������, �� ���� ���� �� ����� �� ������� �� ��������� Xn = b - Egr
            break;
        }
    }
    return finalResult;
}

//��������� ������� � ��������� ��������� �����������.
//x0 - ���. ����. ����, v0 - ���. ����. �������, h0 - ��� ��������������, func - ������� � ��, B - ������� ����������, Hmax - ������������ ����� �����, E - �����������, Egr - �������� ������ �� �������
std::vector<std::pair<double,double>> numericalSolutionWithControl(double x0, double u0, double h0, double (*func)(double, double), double B, int Hmax, double E, double Egr = 0.001)
{
    std::vector<std::pair<double,double>> finalResult;
    finalResult.push_back({ x0,u0 });


    double H = h0;
    for (int i = 0; i < Hmax; i++)
    {
        //������� 1 ������� ���, � 2 �������� ����
        std::pair<double,double> result_simpleTurn = RK4(finalResult.back().first, finalResult.back().second, H, func);
        std::pair<double,double> result_halfTurn = RK4(finalResult.back().first, finalResult.back().second, (H / 2), func);
        std::pair<double,double> resuslt_2_halfTurn = RK4(result_halfTurn.first, result_halfTurn.second, (H / 2), func);

        double S = (resuslt_2_halfTurn.second - result_simpleTurn.second) / (pow(2, 4) - 1); //��������� S, ������� �������� ����� (���. ����.) / (2^p)

        //�������� ��� ������ ������
        if ((E / pow(2, 5)) <= fabs(S) && fabs(S) <= E)//�������� � �������� �����������, ��������� � ��� ������
        {
            finalResult.push_back(result_simpleTurn);
        }
        else if (fabs(S) < (E / pow(2, 5))) //�������� ������� ������, ��������� � ��������� ���
        {
            finalResult.push_back(result_simpleTurn);
            H = H * 2;
        }
        else //�������� ������� ��������, ��������� ��� � ��� ���� � �������������
        {
            H = H / 2;
            i--;
            continue;
        }

        //��������� ����� �� �������(� ������ ��� ������� ������)
        if (finalResult.back().first >= (B - Egr) && finalResult.back().first <= B)
        {
            break;
        }
        else if (finalResult.back().first > B)
        {
            finalResult.pop_back();
            finalResult.push_back(RK4(finalResult.back().first, finalResult.back().second,B - finalResult.back().first - Egr, func)); //���� ����� ��������, �� ���� ���� �� ����� �� ������� �� ��������� Xn = b - Egr
            break;
        }
    }
    return finalResult;
}
