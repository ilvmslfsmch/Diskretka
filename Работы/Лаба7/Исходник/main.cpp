#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

#include "matplot/matplot.h"

double det3(double a11, double a12, double a13,
            double a21, double a22, double a23,
            double a31, double a32, double a33)
{
    return a11 * a22 * a33 +
	    a12 * a23 * a31 +
	    a13 * a21 * a32 -
	    a31 * a22 * a13 -
	    a32 * a23 * a11 -
	    a33 * a21 * a12;
}

struct Parabola {
    double a, b, c;
    double x_start, x_end;
    
    double eval(double x) const {
        return a * x * x + b * x + c;
    }
};

Parabola solve_parabola(double x0, double y0,
                        double x1, double y1,
                        double x2, double y2)
{
    double x0s = x0 * x0;
    double x1s = x1 * x1;
    double x2s = x2 * x2;
    
    double D = det3(x0s, x0, 1,
                    x1s, x1, 1,
                    x2s, x2, 1);
    
    double Da = det3(y0, x0, 1,
                     y1, x1, 1,
                     y2, x2, 1);
    
    double Db = det3(x0s, y0, 1,
                     x1s, y1, 1,
                     x2s, y2, 1);
    
    double Dc = det3(x0s, x0, y0,
                     x1s, x1, y1,
                     x2s, x2, y2);
    
    Parabola p;
    p.a = Da / D;
    p.b = Db / D;
    p.c = Dc / D;
    p.x_start = x0;
    p.x_end = x2;
    return p;
}

int main() {
    std::vector<double> X = {0, 1, 2, 3.5, 4.5, 5.5, 6.5};
    std::vector<double> Y = {-0.5, -1, 0, 0.5, 2, 1.5, 0.5};
    
    int n = X.size();
    
    std::vector<double> k_lin(n - 1), b_lin(n - 1);
    for (int i = 0; i < n - 1; i++) {
        k_lin[i] = (Y[i+1] - Y[i]) / (X[i+1] - X[i]);
        b_lin[i] = Y[i] - k_lin[i] * X[i];
    }
    
    std::vector<Parabola> parabolas;
    for (int i = 0; i + 2 < n; i += 2) {
        parabolas.push_back(
            solve_parabola(X[i], Y[i], X[i+1], Y[i+1], X[i+2], Y[i+2])
        );
    }
    
    std::cout << std::fixed << std::setprecision(4);
    
    std::cout << "Кусочно-линейная интерполяция\n";
    for (int i = 0; i < n - 1; i++) {
        std::cout << "[" << X[i] << ", " << X[i+1] << "]: "
                  << "y = " << k_lin[i] << "*x + " << b_lin[i] << "\n";
    }
    
    std::cout << "\nКусочно-квадратичная интерполяция\n";
    for (size_t i = 0; i < parabolas.size(); i++) {
        const Parabola& p = parabolas[i];
        std::cout << "[" << p.x_start << ", " << p.x_end << "]: "
                  << "y = " << p.a << "*x^2 + " << p.b << "*x + " << p.c << "\n";
    }
    
    double x_min = X.front();
    double x_max = X.back();
    int N = 500;
    
    std::vector<double> x_line = matplot::linspace(x_min, x_max, N);
    std::vector<double> y_linear(N);
    std::vector<double> y_quadratic(N);
    
    for (int j = 0; j < N; j++) {
        double x = x_line[j];
        
        int i_lin = 0;
        for (int i = 0; i < n - 1; i++) {
            if (x >= X[i] && x <= X[i+1]) {
                i_lin = i;
                break;
            }
        }
        y_linear[j] = k_lin[i_lin] * x + b_lin[i_lin];
        
        int i_quad = 0;
        for (size_t i = 0; i < parabolas.size(); i++) {
            if (x >= parabolas[i].x_start && x <= parabolas[i].x_end) {
                i_quad = i;
                break;
            }
        }
        y_quadratic[j] = parabolas[i_quad].eval(x);
    }
    
    auto pts = matplot::scatter(X, Y);
    pts->marker_size(12).marker_color("blue");
    
    matplot::hold(matplot::on);
    
    auto lin = matplot::plot(x_line, y_linear, "r--");
    lin->line_width(2);
    
    auto quad = matplot::plot(x_line, y_quadratic, "g-");
    quad->line_width(2);
    
    matplot::hold(matplot::off);
    
    matplot::xlabel("x");
    matplot::ylabel("y");
    matplot::title("Интерполяция: точки, кусочно-линейная, кусочно-квадратичная");
    matplot::legend({"Точки", "Кусочно-линейная", "Кусочно-квадратичная"});
    matplot::grid(matplot::on);
    
    matplot::show();
    
    return 0;
}
