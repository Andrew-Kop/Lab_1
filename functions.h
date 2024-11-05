#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//Функция в дифференциальном уравнении.
double testFuncDu(double x, double u)
{
    return (-1.0) * (7.0 / 2.0) * u;
}

//Решение задачи коши для тестового ДУ
class generalSolutionU
{
private:
    double x0,u0;
public:
    generalSolutionU(double x0, double u0): x0(x0),u0(u0)
    {
    }
    double operator()(double x)
    {
        double c = u0 / std::exp((-7.0 / 2.0) * x0);
        return c*std::exp((-7.0 / 2.0) * x);
    }
};

double func1Du(double x0, double v0)
{
    return pow(v0, 2) / (1.0 + 3.0 * x0 + pow(x0, 2)) + v0 - pow(v0, 3) * sin(10.0 * x0);
}

//Система Ду с заданными пармаетрами а и b
class system_du
{
private:
    double a, b;
public:
    system_du(double a, double b): a(a), b(b)
    {
    }
    std::pair<double, double> operator()(double x, double u1, double u2) //u1' = u2; u2' = -a*sqrt(u2^2 + 1) - b.
    {
        std::pair<double, double> result;
        result.first = u2;
        result.second = -a * sqrt(pow(u2, 2) + 1.0) - b;
        return result;
    }
};

#endif // FUNCTIONS_H
