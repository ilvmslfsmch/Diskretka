#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include "matplot/matplot.h"

int main() {
    std::vector<double> X = {1, 2.5, 4, 5.5, 7, 8.5, 10, 11.5, 13, 14.5, 16, 17.5, 19, 20.5, 
                             22, 23.5, 25, 26.5, 28, 29.5, 31, 32.5, 34, 35.5, 37, 38.5, 
                             40, 41.5, 43, 44.5, 46, 47.5, 49, 50.5, 52, 53.5, 55, 56.5, 58, 59.5, 61};
    
    std::vector<double> Y = {4.644, 3.083, 3.683, 5.038, 5.068, 2.539, 6.493, 5.287, 6.487, 6.708, 
                             5.119, 6.377, 6.466, 5.074, 7.041, 5.617, 4.33, 6.621, 5.538, 6.285, 
                             6.702, 7.346, 6.271, 8.322, 7.293, 8.17, 8.32, 5.479, 9.176, 7.334, 
                             7.695, 8.962, 9.558, 7.295, 7.364, 9.575, 7.044, 8.116, 9.761, 6.363, 7.403};

    int m = X.size();
    
    double sum_x = 0, sum_y = 0, sum_xx = 0, sum_xy = 0;
    for (int i = 0; i < m; i++) {
        sum_x += X[i];
        sum_y += Y[i];
        sum_xx += X[i] * X[i];
        sum_xy += X[i] * Y[i];
    }

    double det = m * sum_xx - sum_x * sum_x;
    
    double C1 = (m * sum_xy - sum_x * sum_y) / det;
    double C0 = (sum_y * sum_xx - sum_x * sum_xy) / det;

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Уравнение аппроксимирующей прямой:" << std::endl;
    std::cout << "y = " << C1 << " * x + " << C0 << std::endl;

    matplot::scatter(X, Y)->marker_size(10).marker_color("blue");
    
    double x_min = X.front();
    double x_max = X.back();
    std::vector<double> x_line = matplot::linspace(x_min, x_max, 100);
    std::vector<double> y_line;
    for (double xi : x_line) {
        y_line.push_back(C1 * xi + C0);
    }
    
    matplot::hold(matplot::on);
    matplot::plot(x_line, y_line, "r-")->line_width(2);
    matplot::hold(matplot::off);

    matplot::title("Линейная аппроксимация методом наименьших квадратов");
    matplot::xlabel("x");
    matplot::ylabel("y");
    matplot::grid(matplot::on);
    matplot::show();

    return 0;
}
