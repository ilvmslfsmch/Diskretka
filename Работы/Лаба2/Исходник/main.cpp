#include <matplot/matplot.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>

/**
 * @brief Функция единичного испульса с задержкой
 * @param n - значение n (значение по оси X)
 * @param delay - задержка
 * @return 1.0 при n = delay, иначе 0.0 (Хардкод момент =) мб надо будет менять значения на защите, но реализовывать это я уже устал, ахахах
 */
double impulse (double n, double delay);

/**
 * @brief Функция сигнала с задержкой
 * @param n - значение n (значение по оси x)
 * @param delay - задержка
 * @return 1.0, если настал момент включения после задержки, иначе 0.0
 */
double step (double n, double delay);

/**
 * @bried Функция дискретной экспоненты
 * @param n - значение n (значение по оси x)
 * @pamram a - основание для экспоненты (по умолчанию 0.8)
 * @return значение a^n, иначе 0.0
 */
double dis_exp(double n, double a);

/**
 * @brief Функция дискретной косинусоиды (x[n] = cos (onega * n))
 * @param n - значение n (значение по оси x)
 * @param omega - угловая частота для косинуса (по умолчанию 0.5)
 * @return полученное значение
 */
double dis_cos (double n, double L, double omega);

int main(void) {
	
	double min_x = -5.0; //минимальное значение по оси x (по умолчанию)
	double max_x = 10.0; //максимальное значение по оси x (по умолчанию)
	size_t num_of_points = 16; //Число точек на оси (по умолчанию); шаг выставляется автоматически автоматически
	int choice_lim = 0;
	std::cout << "Как выбрать пределы?\n"
		<< "1 - Вручную\n"
		<< "2 - по умолчанию (min_x = -5, max_x = 10, num_of_points = 16)" << std::endl;
	std::cin >> choice_lim;

	switch (choice_lim) {
		case 1:
			std::cout << "Введите min_x, max_x, num_of_points" << std::endl;
			std::cin >> min_x >> max_x >> num_of_points;
			break;
		default:
			break;
	}

	double delay = 0.0; //Задержка (по умолчанию отсутствует)

	double a = 0.8; //Основание для экспоненты по умолчанию
	double L = 1.0; //Амплитуда для конинусоиды по умолчанию
	double omega = 0.5; //Угловая частота косинусоиды по умолчанию
	
	int choice_func = 0;
	std::cout << "Выберите график\n"
	       << "1 - Единичный испульс\n"
	       << "2 - Единичный скачок\n"
	       << "3 - Дискретная экспонента\n"
	       << "4 - Дискретная косинусоида"
	       << std::endl;
	std::cin >> choice_func;

	std::vector<double> n = matplot::linspace(min_x, max_x, num_of_points); //точки - первая, последняя, количество точек
	std::vector<double> x(n.size(), 0); //пустой вектор для оси y
	
	switch (choice_func) {
		case 1:
			std::cout << "Введите задержку по (0 - без задержки, другое число - длина (время) задержки)" << std::endl;
			std::cin >> delay;
			for (size_t i = 0; i < n.size(); i++) {
				x[i] = impulse(n[i], delay);
			}
			matplot::stem(n, x);
			matplot::title("Единичный импульс, задержка - " + std::to_string(delay));
			break;
		case 2: {
			std::cout << "Введите задержку по умолчанию (0 - без задержки, другое число - длина (время) задержки)" << std::endl;
			std::cin >> delay;
			for (size_t i = 0; i < n.size(); i++) {
				x[i] = step(n[i], delay);
			}
			matplot::stem(n, x);
			matplot::title("Сигнал, задержка - " + std::to_string(delay));
			break;
			}
		case 3:
			std::cout << "Введите значение a основания n^a (по умолчанию а = 0.8):" << std::endl;
			std::cin >> a;
			for(size_t i = 0; i < n.size(); i++) {
				x[i] = dis_exp(n[i], a);
			}
			matplot::scatter(n, x)->marker_size(10);
			matplot::title("Дискретная экспонента");
			break;
		case 4:
			std::cout << "Введите амплитуду L (по умолчанию 1) и значение углового коэффициента omega(по умолчанию 0.5)" << std::endl;
			std::cin >> L >> omega;
			for (size_t i = 0; i < n.size(); i++) {
				x[i] = dis_cos(n[i], L, omega);
			}
			matplot::scatter(n, x)->marker_size(10);
			matplot::title("Дискретная косинусоида");
			break;
		default:
			std::cout << "Нет такого выбора. Учимся читать вместе с данной программой день первый =)" << std::endl;
			return 1;
	}

	matplot::xlim({min_x, max_x});
	matplot::ylim({-1.5, 1.5});
	matplot::xlabel("n");
	matplot::ylabel("x[n]");
	matplot::grid(matplot::on);
	matplot::show();

	return 0;
}

double impulse (double n, double delay) {
	if (std::abs(n - delay) < 1e-9) {
		return 1.0;
	}
	return 0.0;
}

double step(double n, double delay) {
	if (n >= delay) return 1.0;
	return 0.0;
}

double dis_exp (double n, double a) {
	if (n >=0) return std::pow(a, n);
	return 0.0;
}

double dis_cos (double n, double L, double omega) {
	return std::cos(omega * n) * L;
} 
