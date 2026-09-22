#include <iostream>
#include <complex>
#include <cmath>
#include <vector>

//#include <matplot/matplot.h>

void print_complex(const std::complex<double>& result);

int main(void) {

	std::cout << "Значения для примеров вычисления + графики:\n"
		<< "1, -3, 2 - корни 2, 1 (на оси X, справа)\n"
		<< "1, 0, 1 - корни i, -i (на оси Y, сверху/снизу симметрично)\n"
		<< "1, -2, 5 - корни 1 + 2i, 1 - 2i (симметрично на графике, не на осях)\n"
		<< "1, 2, 1 - корни -1, -1 (точки накладываются)"
		<< std::endl;

	std::cout << std::endl;

	double a = 0.0, b = 0.0, c = 0.0;
	std::cout << "Введите a, b, c (ax^2 + bx + c = 0):" << std::endl;
	std::cin >> a >> b >> c;

	if (std::abs(a) < 1e-9) { //Линейное уравнение bx + c = 0
		if (std::abs(b) < 1e-9) {
			if (std::abs(c) < 1e-9) {
				std::cout << "Бесконечно много решений" << std::endl;
			} else {
				std::cout << "Нет решений" << std::endl;
			}
			
		} else {
			double result = -c / b;
			std::cout << "Линейное уравнение, результат: " << result << std::endl;
		}
		return 0;
	}

	std::complex<double> A(a, 0.0);
	std::complex<double> B(b, 0.0);
	std::complex<double> C(c, 0.0);

	std::complex<double> D = B * B - 4.0 * A * C;

	std::complex<double> sqrt_D = std::sqrt(D);

	std::complex<double> x1 = (-B + sqrt_D)/(2.0 * A);
	std::complex<double> x2 = (-B - sqrt_D)/(2.0 * A);

	std::cout << "X1 = ";
	print_complex(x1);
	std::cout << std::endl;

	std::cout << "X2 = ";
	print_complex(x2);
	std::cout << std::endl;
/*
	std::vector<double> x_coords{x1.real(), x2.real()};
	std::vector<double> y_coords{x1.imag(), x2.imag()};

	matplot::scatter(x_coords, y_coords)->marker_size(12);
	matplot::hold(matplot::on);
	matplot::plot(std::vector<double> {-3, 3}, std::vector<double> {0, 0}, "k--"); //ось X
	matplot::plot(std::vector<double> {0, 0}, std::vector<double> {-3, 3}, "k--"); //ось y
	matplot::hold(matplot::off);

	matplot::xlim({-3, 3});
	matplot::ylim({-3, 3});
	matplot::xlabel("Re");
	matplot::ylabel("Im");
	matplot::grid(matplot::on);
	matplot::title("Комплексные корни");

	matplot::show();
*/
	return 0;
}

void print_complex(const std::complex<double>& result) {
	double re = result.real();
	double im = result.imag();

	if (std::abs(im) < 1e-9) {
		std::cout << re;
	} else if (std::abs(re) < 1e-9) {
		std::cout << im << "i";
	} else if (im > 0) {
		std::cout << re << " + " << im << "i";
	} else {
		std::cout << re << " - " << -im << "i";
	}
}
