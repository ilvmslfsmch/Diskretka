#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip> 
#include <sstream>

#include "matplot/matplot.h"

std::vector<double> get_chebishev_coeffs(int n);
double horner(const std::vector<double>& coeffs, double x);
std::string poly_to_string(const std::vector<double>& coeffs);
void task1();
void task2();

int main() {
    int choice;
    std::cout << "Выберите задание:\n"
              << "1 - Вычислить и показать первые 8 полиномов Чебышева\n"
              << "2 - Вычислить полином Чебышева 60 степени" << std::endl;
    std::cin >> choice;

    switch (choice) {
        case 1: task1(); break;
        case 2: task2(); break;
        default: std::cerr << "Некорректный выбор!" << std::endl; return 1;
    }
    return 0;
}

std::string poly_to_string(const std::vector<double>& coeffs) {
    std::ostringstream oss;
    bool first = true;
    for (int i = static_cast<int>(coeffs.size()) - 1; i >= 0; --i) {
        double c = coeffs[i];
        if (std::abs(c) < 1e-9) continue;

        if (!first) {
            oss << (c > 0 ? " + " : " - ");
            c = std::abs(c);
        } else if (c < 0) {
            oss << "-";
            c = std::abs(c);
        }

        if (std::abs(c - 1.0) > 1e-9 || i == 0) {
            oss << c;
        }
        if (i > 0) {
            oss << "x";
            if (i > 1) oss << "^" << i;
        }
        first = false;
    }
    return oss.str().empty() ? "0" : oss.str();
}

std::vector<double> get_chebishev_coeffs(int n) {
    if (n == 0) return {1.0};
    if (n == 1) return {0.0, 1.0};
    std::vector<double> T_prev = {1.0};
    std::vector<double> T_curr = {0.0, 1.0};
    for (int k = 2; k <= n; k++) {
        std::vector<double> T_next(T_curr.size() + 1, 0.0);
        for (size_t i = 0; i < T_curr.size(); i++) {
            T_next[i+1] += 2.0 * T_curr[i];
        }
        for (size_t i = 0; i < T_prev.size(); i++) {
            T_next[i] -= T_prev[i];
        }
        T_prev = T_curr;
        T_curr = T_next;
    }
    return T_curr;
}

double horner(const std::vector<double>& coeffs, double x) {
    if (coeffs.empty()) return 0.0;
    double result = coeffs.back();
    for (int i = static_cast<int>(coeffs.size()) - 2; i >= 0; i--) {
        result = result * x + coeffs[i];
    }
    return result;
}

void task1() {
    std::cout << "--- ЗАДАНИЕ 1 ---" << std::endl;
    
    std::cout << "\n1. Полученные полиномы Чебышева:\n";
    for (int n = 0; n <= 7; ++n) {
        auto coeffs = get_chebishev_coeffs(n);
        std::cout << "T_" << n << "(x) = " << poly_to_string(coeffs) << std::endl;
    }

    std::cout << "\n2. Проверка значений при x = 0.5:\n";
    std::cout << std::fixed << std::setprecision(4);
    std::cout << std::setw(5)  << "n" 
              << std::setw(20) << "Horner" 
              << std::setw(20) << "Trigonimetry" << std::endl;
    std::cout << std::string(45, '-') << std::endl;

    for (int n = 0; n <= 7; ++n) {
        auto coeffs = get_chebishev_coeffs(n);
        double h_val = horner(coeffs, 0.5);
        double t_val = std::cos(n * std::acos(0.5));
        std::cout << std::setw(5)  << n 
                  << std::setw(20) << h_val 
                  << std::setw(20) << t_val << std::endl;
    }

    std::cout << "\n3. Сохранение графиков..." << std::endl;
    std::vector<double> x_vals = matplot::linspace(-1.0, 1.0, 500);

    for (int n = 0; n <= 7; ++n) {
        matplot::cla();
	matplot::hold(matplot::on);
        
        auto coeffs = get_chebishev_coeffs(n);
        std::vector<double> y_horner(x_vals.size());
        std::vector<double> y_trig(x_vals.size());

        for (size_t i = 0; i < x_vals.size(); ++i) {
            double x = x_vals[i];
            y_horner[i] = horner(coeffs, x);
            y_trig[i] = std::cos(n * std::acos(x));
        }

        auto p_line = matplot::plot(x_vals, y_horner, "-");
        p_line->line_width(2.0);

        std::vector<double> x_sparse, y_trig_sparse;
        for (size_t i = 0; i < x_vals.size(); i += 20) {
            x_sparse.push_back(x_vals[i]);
            y_trig_sparse.push_back(y_trig[i]);
        }
        auto p_dots = matplot::scatter(x_sparse, y_trig_sparse);
        p_dots->marker_size(7).marker_style("o").marker_color("black");

        matplot::title("Полином Чебышева T" + std::to_string(n));
        matplot::xlabel("x");
        matplot::ylabel("T_" + std::to_string(n) + "(x)");
        matplot::xlim({-1.1, 1.1});
        matplot::ylim({-1.2, 1.2});
        matplot::grid(matplot::on);

	//matplot::legend({"Горнер (линия)", "Тригонометрия (точки)"});
        std::string filename = "cheb_T" + std::to_string(n) + ".png";
        matplot::save(filename);
        std::cout << "Сохранено: " << filename << std::endl;
        
        matplot::hold(matplot::off);
    }
    std::cout << "Готово!" << std::endl;
}

void task2() {
    std::cout << "Пока не сделано =(" << std::endl;
}
