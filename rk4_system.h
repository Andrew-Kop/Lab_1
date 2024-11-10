#pragma once
#include <cmath>
#include <list>
#include <vector>
#include "functions.h"
using std::list;
using std::pair;
using std::vector;



//����� ����� 4 ������� ��� �������
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

//��������� ������� ��� �������� ��������� �����������.
//x0 - ���. ����. ����, u01 - ������ ���. ����. �������, u02 - ������ ���. ����. �������, h0 - ��� ��������������, system - ������� 2 ������� ��, B - ������� ����������, Hmax - ������������ ����� �����, Egr - �������� ������ �� �������
vector<vector<double>> numericalSolutionSystem(double x0, double u01, double u02, double h0, system_du system, double B, int Hmax,  double Egr = 0.001)
{
    vector<vector<double>> finalResult;
	finalResult.push_back({ x0,u01,u02 });
	for (int i = 0; i < Hmax; i++)
	{
		finalResult.push_back(rk4_system(finalResult.back()[0], finalResult.back()[1], finalResult.back()[2], h0, system)); //��������� ������� (Xn, Vn) � ������ ��������� ����������

		//��������� ����� �� �������(� ������ ��� ������� ������)
		if (finalResult.back()[0] >= (B - Egr) && finalResult.back()[0] <= B)
		{
			break;
		}
		else if (finalResult.back()[0] > B)
		{
			finalResult.pop_back();
			finalResult.push_back(rk4_system(finalResult.back()[0], finalResult.back()[1], finalResult.back()[2], B - finalResult.back()[0] - Egr, system)); //���� ����� ��������, �� ���� ���� �� ����� �� ������� �� ��������� Xn = b - Egr
			break;
		}
	}
	return finalResult;
}

//��������� ������� � ��������� ��������� �����������.
//x0 - ���. ����. ����, u01 - ������ ���. ����. �������, u02 - ������ ���. ����. �������, h0 - ��� ��������������, system - ������� 2 ������� ��, B - ������� ����������, Hmax - ������������ ����� �����, Egr - �������� ������ �� �������
vector<vector<double>> numericalSolutionSystemWithControl(double x0, double u01, double u02, double h0, system_du system, double B, int Hmax, double E, double Egr = 0.001)
{
    vector<vector<double>> finalResult;
    finalResult.push_back({ x0,u01,u02 });


    double H = h0;
    for (int i = 0; i < Hmax; i++)
    {
        //������� 1 ������� ���, � 2 �������� ����
        vector<double> result_simpleTurn = rk4_system(finalResult.back()[0], finalResult.back()[1], finalResult.back()[2], H, system);
        vector<double> result_halfTurn = rk4_system(finalResult.back()[0], finalResult.back()[1], finalResult.back()[2], H / 2, system);
        vector<double> resuslt_2_halfTurn = rk4_system(result_halfTurn[0], result_halfTurn[1], result_halfTurn[2], (H / 2), system);

        double S1 = (resuslt_2_halfTurn[1] - result_simpleTurn[1]) / (pow(2, 4) - 1); 
        double S2 = (resuslt_2_halfTurn[2] - result_simpleTurn[2]) / (pow(2, 4) - 1); 
		double S = std::max(fabs(S1), fabs(S2)); //����� ������� S
        //�������� ��� ������ ������
        if ((E / pow(2, 5)) <= S && S <= E)//�������� � �������� �����������, ��������� � ��� ������
        {
            finalResult.push_back(result_simpleTurn);
        }
        else if (S < (E / pow(2, 5))) //�������� ������� ������, ��������� � ��������� ���
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
			finalResult.push_back(rk4_system(finalResult.back()[0], finalResult.back()[1], finalResult.back()[2], B - finalResult.back()[0] - Egr, system)); //���� ����� ��������, �� ���� ���� �� ����� �� ������� �� ��������� Xn = b - Egr
			break;
		}
    }
    return finalResult;
}
