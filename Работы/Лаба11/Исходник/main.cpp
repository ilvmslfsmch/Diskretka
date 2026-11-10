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

std::vector<double> get_norm_chebishev_coeffs(int n);

void task1();

void task2();

void task4();

int main() {
    int choice;
    std::cout << "Выберите задание:\n"
              << "1 - Вычислить и показать первые 8 полиномов Чебышева\n"
              << "2 - Вычислить полином Чебышева 60 степени\n"
	      << "4 - Поиск нулей/экстремумов для полинома 7-й степени" << std::endl;
    std::cin >> choice;

    switch (choice) {
        case 1: task1(); break;
        case 2: task2(); break;
	case 4: task4(); break;
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

std::vector<double> get_norm_chebishev_coeffs(int n) {
	if (n == 0) return {1.0};
	if (n == 1) return {0.0, 1.0};
	std::vector<double> T_prev = {1.0};
	std::vector<double> T_curr = {0.0, 1.0};
	for (int k = 2; k <= n; k++) {
		std::vector<double> T_next(T_curr.size() + 1, 0.0);
		for (size_t i = 0; i < T_curr.size(); i++) {
			T_next[i+1] += T_curr[i];
		}
		double factor = (k == 2) ? 0.5 : 0.25;
		for (size_t i = 0; i < T_prev.size(); i++) {
			T_next[i] -= factor * T_prev[i];
		}
		T_prev = T_curr;
		T_curr = T_next;
	}
	return T_curr;
}

void task2() {
	int n = 60;
	std::cout << "Задание 2. Полином Чебышева " << n << "-й степени" << std::endl;
	std::vector<double> x_vals = matplot::linspace(-1.0, 1.0, 2000);
	auto norm_coeffs = get_norm_chebishev_coeffs(n);
	double norm_factor = std::pow(2.0, n - 1);
	std::vector<double> y_horner(x_vals.size());
	std::vector<double> y_trig(x_vals.size());
	for (size_t i = 0; i < x_vals.size(); i++) {
		double x = x_vals[i];
		double norm_val = horner(norm_coeffs, x);
		y_horner[i] = norm_val * norm_factor;
		y_trig[i] = std::cos(n * std::acos(x));
	}
	matplot::cla();
	matplot::hold(matplot::on);
	auto p1 = matplot::plot(x_vals, y_horner, "b-");
	p1->line_width(1.5);

	std::vector<double> x_sparse, y_trig_sparse;
	for (size_t i = 0; i < x_vals.size(); i++) {
		x_sparse.push_back(x_vals[i]);
		y_trig_sparse.push_back(y_trig[i]);
	}
	auto p2 = matplot::scatter(x_sparse, y_trig_sparse);
	p2->marker_size(1).marker_color("red");

	matplot::title("Полином чебышева T_6_0[x] (нормирование)");
	matplot::xlabel("x");
	matplot::ylabel("T_60(x)");
	matplot::xlim({-1.05, 1.05});
	matplot::ylim({-1.5, 1.5});
	matplot::grid(matplot::on);
	matplot::legend({"Горнер(норм.)", "Тригонометрия"});

	matplot::save("cheb_T60.png");
	std::cout << "Сохранено: cheb_T60.png" << std::endl;
	matplot::hold(matplot::off);
}

void task4() {
    int n = 7;
    const double PI = std::acos(-1.0);
    std::cout << "ЗАДАНИЕ 4" << std::endl;
    std::cout << "Поиск нулей и экстремумов T_" << n << "(x)\n" << std::endl;

    std::vector<double> an_roots;
    std::vector<double> an_extrema;

    for (int k = 1; k <= n; ++k) {
        an_roots.push_back(std::cos(PI * (2 * k - 1) / (2.0 * n)));
    }

    for (int k = 0; k <= n; ++k) {
        an_extrema.push_back(std::cos(PI * k / n));
    }

    double step = 0.0001;
    std::vector<double> x_vals, y_vals;

    for (double x = -1.0; x <= 1.0 + step/2.0; x += step) {
        x_vals.push_back(x);
        y_vals.push_back(std::cos(n * std::acos(x)));
    }

    std::vector<double> num_roots;
    std::vector<double> num_extrema;

    for (size_t i = 1; i < x_vals.size() - 1; ++i) {
        if (y_vals[i-1] * y_vals[i] <= 0) {
            double root = x_vals[i-1] - y_vals[i-1] * (x_vals[i] - x_vals[i-1]) / (y_vals[i] - y_vals[i-1]);
            num_roots.push_back(root);
        }

        if ((y_vals[i] > y_vals[i-1] && y_vals[i] > y_vals[i+1]) ||
            (y_vals[i] < y_vals[i-1] && y_vals[i] < y_vals[i+1])) {
            num_extrema.push_back(x_vals[i]);
        }
    }

    num_extrema.push_back(1.0);
    num_extrema.push_back(-1.0);

    std::sort(num_roots.rbegin(), num_roots.rend());
    std::sort(num_extrema.rbegin(), num_extrema.rend());

    std::cout << std::fixed << std::setprecision(5);

    std::cout << "КОРНИ (НУЛИ)" << std::endl;
    std::cout << std::setw(5)  << "k"
              << std::setw(20) << "Numeric"
              << std::setw(20) << "Analitic"
              << std::setw(20) << "Diff" << std::endl;
    std::cout << std::string(65, '-') << std::endl;

    for (size_t i = 0; i < an_roots.size(); ++i) {
        double diff = std::abs(num_roots[i] - an_roots[i]);
        std::cout << std::setw(5)  << i + 1
                  << std::setw(20) << num_roots[i]
                  << std::setw(20) << an_roots[i]
                  << std::setw(20) << diff << std::endl;
    }

    std::cout << "\nЭКСТРЕМУМЫ (МАКС/МИН)" << std::endl;
    std::cout << std::setw(5)  << "k"
              << std::setw(20) << "Numeric"
              << std::setw(20) << "Analitic"
              << std::setw(20) << "Diff" << std::endl;
    std::cout << std::string(65, '-') << std::endl;

    for (size_t i = 0; i < an_extrema.size(); ++i) {
        double diff = std::abs(num_extrema[i] - an_extrema[i]);
        std::cout << std::setw(5)  << i
                  << std::setw(20) << num_extrema[i]
                  << std::setw(20) << an_extrema[i]
                  << std::setw(20) << diff << std::endl;
    }
}
