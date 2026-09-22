#include <iostream>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <string>
#include <vector>

#include "matplot/matplot.h"

void task1(const int n);

void task2();

void task3();

void task4();

long long fib_iter(const int n);

double fib_bine(const int n);

std::vector<double> solve_iter(int n, double x0, double x1, bool eq_a);

double solve_analit(int n, double x0, double x1, bool eq_a);

int main(void) {

	std::cout << "Выберите задание:\n"
		<< "1 - вычислить числа Фибоначчи для n по итерационной формуле и формуле n-ного числа (формула Бине)\n"
		<< "2 - вычислить отношение двух последовательных чисел Фибоначчи и нанести их на график\n"
		<< "3 - вычислить отклонение отношений двух последовательных чисел Фибоначчи от золотого сечения и нанести их на график\n"
		<< "4 - решить разностные уравнения и визуализировать решения" << std::endl;
	int choice;
	std::cin >> choice;

	switch (choice) {
		case 1: {
			int n;
			std::cout << "Введите n - количество чисел Фибоначчи" << std::endl;
			std::cin >> n;
			task1(n);
			break;
			}
		case 2: {
			task2();
			break;
			}
		case 3: {
			task3();
			break;
			}
		case 4: {
			task4();
			break;
			}
		default:
			std::cerr << "Некорректный выбор";
			std::exit(1);
	}

	return 0;
}

void task1(const int n) {
	if (n > 92) {
		std::cerr << "Введите значени меньше чем 93";
		std::exit(1);
	}
	std::cout << std::fixed << std::setprecision(4);

	std::cout << std::setw(5) << "n"
		<< std::setw(25 + 8) << "Итерация"
		<< std::setw(30 + 4) << "Бине"
	       	<< std::setw(15 + 7) << "Разница" << std::endl;
	std::cout << std::string(75, '-') << std::endl;

	if (n <= 12) {
		for (int i = 0; i <= n; i++) {
			std::cout << std::setw(5) << i
				<< std::setw(25) << fib_iter(i)
				<< std::setw(30) << fib_bine(i) 
				<< std::setw(15) << std::abs(fib_iter(i) - fib_bine(i)) << std::endl;
		}
	} else {
		for (int i = 0; i <= 12; i++) {
			std::cout << std::setw(5) << i
				<< std::setw(25) << fib_iter(i)
				<< std::setw(30) << fib_bine(i) 
				<< std::setw(15) << std::abs(fib_iter(i) - fib_bine(i)) << std::endl;
		}
		std::cout << "..." << std::endl;
		std::cout << std::setw(5) << n
			<< std::setw(25) << fib_iter(n)
			<< std::setw(30) << fib_bine(n) 
			<< std::setw(15) << std::abs(fib_iter(n) - fib_bine(n)) << std::endl;
	}
}

void task2() {
	int n;
	std::cout << "Введите n:" << std::endl;
	std::cin >> n;

	if (n < 1 || n > 91) {
		std::cerr << "Слишком мало/много. введите число от 1 до 91.";
		std::exit(1);
	}

	std::cout << std::fixed << std::setprecision(4);
	std::cout << std::setw(5) << "n"
		<< std::setw(25) << "F[n]"
		<< std::setw(30) << "F[n+1]"
		<< std::setw(30) << "R[n] = F[n+1] / F[n]" << std::endl;
	std::cout << std::string(90, '-') << std::endl;

	std::vector<double> n_vec;
	std::vector<double> r_vec;

	for (int i = 1; i <= n; i++) {
		n_vec.push_back(i);
		r_vec.push_back(static_cast<double>(fib_iter(i + 1)) / fib_iter(i));
	}

	if (n <= 12) {
		for (int i = 1; i <= n; i++) {
			long long cur = fib_iter(i);
			long long next = fib_iter(i + 1);
			double R = static_cast<double>(next) / cur;

			std::cout << std::setw(5) << i
				<< std::setw(25) << cur
				<< std::setw(30) << next
				<< std::setw(30) << R << std::endl;
		}
	} else {
		for (int i = 1; i <= 12; i++) {
			long long cur = fib_iter(i);
			long long next = fib_iter(i + 1);
			double R = static_cast<double>(next) / cur;

			std::cout << std::setw(5) << i
				<< std::setw(25) << cur
				<< std::setw(30) << next
				<< std::setw(30) << R << std::endl;
		}
		
		std::cout << "..." << std::endl;

		long long cur = fib_iter(n);
		long long next = fib_iter(n + 1);
		double R = static_cast<double>(next) / cur;

		std::cout << std::setw(5) << n
			<< std::setw(25) << cur
			<< std::setw(30) << next
			<< std::setw(30) << R << std::endl;
	}

	const double phi = (1.0 + std::sqrt(5.0)) / 2.0;

	double x_min = n_vec.front();
	double x_max = n_vec.back();

	if (n < 60) {
		matplot::scatter(n_vec, r_vec)->marker_size(8);
	} else {
		matplot::scatter(n_vec, r_vec)->marker_size(5);
	}

	matplot::hold(matplot::on);
	matplot::plot({x_min, x_max}, {phi, phi}, "r--");
	matplot::hold(matplot::off);

	matplot::xlabel("n");
	matplot::ylabel("R[n] = F[n+1] / F[n]");
	matplot::grid(matplot::on);

	matplot::show();
}

void task3() {
	const double phi = (1.0 + std::sqrt(5)) / 2;
	int n;
	std::cout << "Введите n:" << std::endl;
	std::cin >> n;

	if (n < 1 || n > 91) {
		std::cerr << "Слишком мало/много. введите число от 1 до 91.";
		std::exit(1);
	}

	std::cout << std::fixed << std::setprecision(4);
	std::cout << std::setw(5) << "n"
		<< std::setw(25) << "F[n]"
		<< std::setw(30) << "F[n+1]"
		<< std::setw(25) << "R[n] = F[n+1] / F[n]"
		<< std::setw(15) << "phi(const)"
		<< std::setw(25) << "D[n] = R[n] - phi" << std::endl;
	std::cout << std::string(125, '-') << std::endl;

	std::vector<double> n_vec;
	std::vector<double> d_vec;

	for (int i = 1; i <= n; i++) {
		n_vec.push_back(i);
		double R = static_cast<double>(fib_iter(i + 1)) / fib_iter(i);
		double D = R - phi;
		d_vec.push_back(D);
	}

	if (n <= 12) {
		for (int i = 1; i <= n; i++) {
			long long cur = fib_iter(i);
			long long next = fib_iter(i + 1);
			double R = static_cast<double>(next) / cur;
			double D = R - phi;

			std::cout << std::setw(5) << i
				<< std::setw(25) << cur
				<< std::setw(30) << next
				<< std::setw(25) << R
				<< std::setw(15) << phi
				<< std::setw(25) << D << std::endl;
		}
	} else {
		for (int i = 1; i <= 12; i++) {
			long long cur = fib_iter(i);
			long long next = fib_iter(i + 1);
			double R = static_cast<double>(next) / cur;
			double D = R - phi;

			std::cout << std::setw(5) << i
				<< std::setw(25) << cur
				<< std::setw(30) << next
				<< std::setw(25) << R
				<< std::setw(15) << phi
				<< std::setw(25) << D << std::endl;
		}

		std::cout << "..." << std::endl;

		long long cur = fib_iter(n);
		long long next = fib_iter(n + 1);
		double R = static_cast<double>(next) / cur;
		double D = R - phi;

		std::cout << std::setw(5) << n
			<< std::setw(25) << cur
			<< std::setw(30) << next
			<< std::setw(25) << R
			<< std::setw(15) << phi
			<< std::setw(25) << D << std::endl;
	}

	double x_min = n_vec.front();
	double x_max = n_vec.back();

	if (n < 60) {
		matplot::scatter(n_vec, d_vec)->marker_size(8);
	} else {
		matplot::scatter(n_vec, d_vec)->marker_size(5);
	}

	matplot::hold(matplot::on);
	matplot::plot({x_min, x_max}, {0.0, 0.0}, "r--");
	matplot::hold(matplot::off);

	double max_abs = 0.0;
	for (double v : d_vec) {
		if (std::abs(v) > max_abs) max_abs = std::abs(v);
	}

	max_abs *= 1.2;
	matplot::ylim({-max_abs, max_abs});

	matplot::xlabel("n");
	matplot::ylabel("D[n] = R[n] - phi");
	matplot::grid(matplot::on);

	matplot::show();
}

void task4() {
	int eq;
	std::cout << "Выберите уравнение:\n"
		<< "1 - x[n+2] = x[n+1] - x[n]\n"
		<< "2 - x[n+2] = 2*x[n+1] - x[n]" << std::endl;
	std::cin >> eq;

	if (eq != 1 && eq != 2) {
		std::cerr << "Некорректный выбор.";
		std::exit(1);
	}

	bool eq_a = (eq == 1);

	int n;
	double x0, x1;
	std::cout << "Введите N - количество членов последовательности:" << std::endl;
	std::cin >> n;
	std::cout << "Введите x0, x1" << std::endl;
	std::cin >> x0 >> x1;

	if (n < 2 || n > 100) {
		std::cerr << "N должно быть в промежутке от 2 до 100";
		std::exit(1);
	}

	std::vector<double> x_iter = solve_iter(n, x0, x1, eq_a);
	std::vector<double> n_vec;
	std::vector<double> iter_vec;
	std::vector<double> analit_vec;

	for (int i = 0; i <= n; i++) {
		n_vec.push_back(i);
		iter_vec.push_back(x_iter[i]);
		analit_vec.push_back(solve_analit(i, x0, x1, eq_a));
	}
	
	std::cout << std::fixed << std::setprecision(6);

	std::cout << std::setw(5) << "n"
		<< std::setw(20) << "Iter"
		<< std::setw(20) << "Analit"
		<< std::setw(20) << "|diff|" << std::endl;
	std::cout << std::string(65, '-') << std::endl;

	for (int i = 0; i <= n; i++) {
		double diff = std::abs(iter_vec[i] - analit_vec[i]);
		std::cout << std::setw(5) << i
			<< std::setw(20) << iter_vec[i]
			<< std::setw(20) << analit_vec[i]
			<< std::setw(20) << diff << std::endl;
	}

	matplot::hold(matplot::on);
	auto p1 = matplot::scatter(n_vec, iter_vec);
	p1->marker_size(8).marker_color("blue");

	auto p2 = matplot::scatter(n_vec, analit_vec);
	p2->marker_size(10).marker_color("red");

	matplot::hold(matplot::off);

	matplot::xlabel("n");
	matplot::ylabel("X[n]");

	if (eq_a) {
		matplot::title("x[n+2] = x[n+1] - x[n]");
	} else {
		matplot::title("x[n+2] = 2*x[n+1] - x[n]");
	}

	matplot::grid(matplot::on);
	matplot::show();
}

std::vector<double> solve_iter(int n, double x0, double x1, bool eq_a) {

	std::vector<double> x(n + 1);
	x[0] = x0;
	x[1] = x1;

	for (int i = 0; i <= n - 2; i++) {
		if (eq_a) {
			x[i + 2] = x[i + 1] - x[i];
		} else {
			x[i + 2] = 2*x[i + 1] - x[i];
		}
	}

	return x;
}

double solve_analit(int n, double x0, double x1, bool eq_a) {
	if (eq_a) {
		const double pi = std::acos(-1.0);
		const double C1 = x0;
		const double C2 = (2 * x1 - x0) / std::sqrt(3.0);
		return C1 * std::cos(pi * n / 3.0) + C2 * std::sin(pi * n / 3.0);
	} else {
		return x0 + (x1 - x0) * n;
	}
}

long long fib_iter(const int n) {
	long long f_prev = 0;
	long long f_curr = 1;

	if (n == 0) {
		return 0;
	}

	if (n == 1) {
		return 1;
	}

	for (int a = 2; a <= n; a++) {
		long long f_next = f_prev + f_curr;
		f_prev = f_curr;
		f_curr = f_next;
	}

	return f_curr;
}

double fib_bine(const int n) {
	const double phi = (1 + std::sqrt(5)) / 2;
	const double psi = (1 - std::sqrt(5)) / 2;

	return (std::pow(phi, n) - std::pow(psi, n)) / std::sqrt(5);
}
