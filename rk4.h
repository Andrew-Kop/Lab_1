#pragma once
#include <vector>
#include <cmath>
#include <QList>

//�����-����� 4 �������.
std::vector<double> RK4(double x0, double v0, double h0, double (*func)(double, double))
{
    double k1 = func(x0, v0);
    double k2 = func(x0 + h0 / 2, v0 + (h0 * k1) / 2);
    double k3 = func(x0 + h0 / 2, v0 + (h0 * k2) / 2);
    double k4 = func(x0 + h0, v0 + h0 * k3);

    double v1 = v0 + h0 * (k1 + 2 * k2 + 2 * k3 + k4) / 6;
    double x1 = x0 + h0;

    std::vector<double> result = { x1, v1 };
    return result;
}

//������� � ���������������� ���������.
double testFuncDu(double x, double u)
{
    return (-1) * (7 / 2) * u;
}

//���������� ������ �������� ��������� �������
QList<double> getTrueSolution(const QList<double>& xArgs, double (*trueFunc)(double)) {
    QList<double> res;

    for (long long i = 0; i < xArgs.size(); i++)
    {
        res.append(trueFunc(xArgs.at(i)));
    }

    return res;
}

//�������� �������
double testUTrue(double x)
{
    return std::exp((-7.0 / 2) * x);
}

//��������� ������� ��� �������� ��������� �����������.
//x0 - ���. ����. ����, v0 - ���. ����. �������, h0 - ��� ��������������, func - ������� � ��, B - ������� ����������, Hmax - ������������ ����� �����, Egr - �������� ������ �� �������
std::vector<std::vector<double>> numericalSolution(double x0, double u0, double h0, double (*func)(double, double), double B, int Hmax, double Egr = 0.001)
{
    std::vector<std::vector<double>> finalResult;
    finalResult.push_back({ x0,u0 });

    for(int i = 0; i < Hmax; i++)
    {
        finalResult.push_back(RK4(finalResult.back()[0], finalResult.back()[1], h0, func)); //��������� ������� (Xn, Vn) � ������ ��������� ����������

        //��������� ����� �� �������(� ������ ��� ������� ������)
        if(finalResult.back()[0] >= (B - Egr) && finalResult.back()[0] <= B)
        {
            break;
        }
        else if (finalResult.back()[0] > B)
        {
            finalResult.pop_back();
            finalResult.push_back(RK4(finalResult.back()[0], finalResult.back()[1], B - finalResult.back()[0] - Egr, func)); //���� ����� ��������, �� ���� ���� �� ����� �� ������� �� ��������� Xn = b - Egr
            break;
        }
    }
    return finalResult;
}

//��������� ������� � ��������� ��������� �����������.
//x0 - ���. ����. ����, v0 - ���. ����. �������, h0 - ��� ��������������, func - ������� � ��, B - ������� ����������, Hmax - ������������ ����� �����, E - �����������, Egr - �������� ������ �� �������
std::vector<std::vector<double>> numericalSolutionWithControl(double x0, double u0, double h0, double (*func)(double, double), double B, int Hmax, double E, double Egr = 0.001)
{
    std::vector<std::vector<double>> finalResult;
    finalResult.push_back({ x0,u0 });


    double H = h0;
    for (int i = 0; i < Hmax; i++)
    {
        //������� 1 ������� ���, � 2 �������� ����
        std::vector<double> result_simpleTurn = RK4(finalResult.back()[0], finalResult.back()[1], H, func);
        std::vector<double> result_halfTurn = RK4(finalResult.back()[0], finalResult.back()[1], (H / 2), func);
        std::vector<double> resuslt_2_halfTurn = RK4(result_halfTurn[0], result_halfTurn[1], (H / 2), func);

        double S = (resuslt_2_halfTurn[1] - result_simpleTurn[1]) / (pow(2, 4) - 1); //��������� S, ������� �������� ����� (���. ����.) / (2^p)

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
        if (finalResult.back()[0] >= (B - Egr) && finalResult.back()[0] <= B)
        {
            break;
        }
        else if (finalResult.back()[0] > B)
        {
            finalResult.pop_back();
            finalResult.push_back(RK4(finalResult.back()[0], finalResult.back()[1],B - finalResult.back()[0] - Egr, func)); //���� ����� ��������, �� ���� ���� �� ����� �� ������� �� ��������� Xn = b - Egr
            break;
        }
    }
    return finalResult;
}
