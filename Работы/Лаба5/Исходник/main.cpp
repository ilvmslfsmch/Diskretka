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
	std::cout << "Заглушка";
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
