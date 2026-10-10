#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

#include <matplot/matplot.h>

int main() {
	int m;
	std::cout << "Введите количество точек m:" << std::endl;
	std::cin >> m;
	if (m < 2) {
		std::cerr << "Необходимо минимум 2 точки";
		return 1;
	}

	std::vector<double> x(m), y(m);
	std::cout << "Введите " << m << " пар точек (x, y):" << std::endl;
	for (int i = 0; i < m; i++) {
		std::cin >> x[i] >> y[i];
	}

	double S_x, S_y, S_xx, S_xy;

	for (int i = 0; i < m; i++) {
		S_x += x[i];
		S_y += y[i];
		S_xx += x[i] * x[i];
		S_xy += x[i] * y[i];
	}

	double denom = m * S_xx - S_x * S_x;

	if (std::abs(denom) < 1e-12) {
		std::cerr << "Деление на ноль, все точки на одной прямой";
		return 1;
	}

	double k = (m * S_xy - S_x * S_y) / denom;
	double b = (S_y * S_xx - S_x * S_xy) / denom;

	std::cout << std::fixed << std::setprecision(4);
	std::cout << "Аппроксимирующая прямая:" << std::endl;
	std::cout << "y = " << k << " * x + " << b << std::endl;

	double x_min = *std::min_element(x.begin(), x.end());
	double x_max = *std::max_element(x.begin(), x.end());

	double margin = (x_max - x_min) * 0.1;
	x_min -= margin;
	x_max += margin;

	std::vector<double> x_line = matplot::linspace(x_min, x_max, 100);
	std::vector<double> y_line;
	y_line.reserve(x_line.size());

	for (double xi : x_line) {
		y_line.push_back(k * xi + b);
	}

	auto p1 = matplot::scatter(x, y);
	p1->marker_size(10).marker_color("blue");
	matplot::hold(matplot::on);
	auto p2 = matplot::plot(x_line, y_line, "r-");
	p2->line_width(2);
	matplot::hold(matplot::off);

	matplot::xlabel("x");
	matplot::ylabel("y");
	matplot::title("Метод наименьших квадратов");
	matplot::grid(matplot::on);

	matplot::show();

	return 0;
}
