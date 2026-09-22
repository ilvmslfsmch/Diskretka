#include <iostream>
#include <complex>
#include <cmath>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

#include <matplot/matplot.h>

const double PI = 3.14159265358979323846;
const double EPS = 1e-9;

const double A = 2.5;
const double B = 3.0;
const int Variant = 19;
const double Limit = 5.0;

void task_1(const double a, const double b);
void task_2(const std::complex<double>& x1, const std::complex<double>& x2);
void task_3(const double a, const double b);
void task_4(const double a, const double b, const int n);

std::complex<double> my_add(const std::complex<double>& z1, const std::complex<double>& z2);
std::complex<double> my_sub(const std::complex<double>& z1, const std::complex<double>& z2);
std::complex<double> my_mul(const std::complex<double>& z1, const std::complex<double>& z2);
std::complex<double> my_div(const std::complex<double>& z1, const std::complex<double>& z2);

bool are_collinear(const std::complex<double>& a, const std::complex<double>& b, const std::complex<double>& c);

double my_arg(const double a, const double b);
void draw_angle_arc(double a, double b, double phi);
std::complex<double> my_root(const std::complex<double>& z, int n, int k);

void print_complex(const std::complex<double>& z);
void setup_axes(double lim);

int main (void) {
	int choice = 0;
	std::cout << "Начальные значения: a = " << A << ", b = " << B << ", вариант номер " << Variant << "." << std::endl;
	std::cout << "Выберите задание:\n"
		<< "1 - Комплексное число z задано парой чисел: z = (a, b). Найти Re z. Im z, |z|, arg(z) и показать их на графике. Также вывести число z в тригонометрической и экспоненциальной формах.\n"
		<< "2 - Даны два комплексных числа z1 и z2. Вывести формулы, вычислить z1 + z2, z1 - z2, z1 * z2, z1 / z2, если z1 = (a, b), z2 = (a + 0.1 * номер варианта, b - 0.1 * номер варианта)\n"
		<< "3 - Нарисовать на комплексной плоскости 4 числа: z, z с чертой (комплексно-сопряжённое число), 1/z и 0.\n"
		<< "4 - Вычислить z^(1/n) (корень n-й степени из z), где n = номер варианта + 5, и показать все значения на комплексной плоскости." << std::endl;
	std::cin >> choice;
	switch (choice) {
		case 1:
			task_1(A, B);
			break;
		case 2: {
			std::complex<double> z1(A, B);
			std::complex<double> z2(A + 0.1 * Variant, B - 0.1 * Variant);
			task_2(z1, z2);
			break;
			}
		case 3: 
			task_3(A, B);
			break;
		case 4: {
			int n = Variant + 5;
			task_4(A, B, n);
			break;
			}
		default:
			break;
	}

	return 0;
}

void task_1 (const double a, const double b) {
	std::complex<double> z(a, b);
	double real = z.real();
	double imag = z.imag();
	double mod_z = std::sqrt(std::pow(real, 2) + std::pow(imag, 2));
	double arg_z = my_arg(real, imag);

	std::cout << "Re z = " << real << "\n"
		<< "Im z = " << imag << "\n"
		<< "|z| = " << mod_z << "\n"
		<< "arg(z) = " << arg_z << std::endl;

	std::cout << "Тригометрическая форма записи для числа z: (z = |z|(cos(arg(z)) + i sin(arg(z))))\n"
		<< "z = " << mod_z << " * (" << std::cos(arg_z) << " + i * " << std::sin(arg_z) << ")" << std::endl;

	std::cout << "Экспоненциальная форма записи: (z = |z| * e^(i arg(z)))\n"
		<< "z = " << mod_z << " * e^(i * " << arg_z << ")" << std::endl;


	matplot::hold(matplot::on);
	matplot::plot(std::vector<double> {-Limit, Limit}, std::vector<double> {0.0, 0.0}, "k--")->line_width(1.2);
	matplot::plot(std::vector<double> {0.0, 0.0}, std::vector<double> {-Limit, Limit}, "k--")->line_width(1.2);
	matplot::plot({real, real}, {0.0, imag}, "b--")->line_width(0.8);
	matplot::plot({0.0, real}, {imag, imag}, "b--")->line_width(0.8);
	matplot::plot({0.0, real}, {0.0, imag}, "k-")->line_width(2.0);
	std::vector<double> xs = {real};
	std::vector<double> ys = {imag};
	matplot::scatter(xs, ys)->marker_size(12);
	matplot::text(real + 0.15, imag + 0.15, "z");
	draw_angle_arc(real, imag, arg_z);
	std::stringstream ss_re, ss_im, ss_mod;
	ss_re << std::fixed << std::setprecision(2) << real;
	ss_im << std::fixed << std::setprecision(2) << imag;
	ss_mod << std::fixed << std::setprecision(2) << mod_z;
	matplot::text(real - 0.6, -0.5, "Re = " + ss_re.str());
	matplot::text(-2.3, imag + 0.15, "Im = " + ss_im.str());
	matplot::text(real / 2.0 + 0.3, imag / 2.0 + 0.1, "|z| = " + ss_mod.str());
	matplot::hold(matplot::off);
	setup_axes(Limit);
	matplot::show();
}

void task_2 (const std::complex<double>& z1, const std::complex<double>& z2) {
	std::cout << std::fixed << std::setprecision(2);

	std::cout << "z1 = "; print_complex(z1); std::cout << std::endl;
	std::cout << "z2 = "; print_complex(z2); std::cout << std::endl;

	std::cout << "Формулы (z1 = a + bi, z2 = c + di):\n"
		<< "z1 + z2 = (a + c) + (b + d)i\n"
		<< "z1 - z2 = (a - c) + (b - d)i\n"
		<< "z1 * z2 = (ac - bd) + (ad + bc)i\n"
		<< "z1 / z2 = ((ac + bd) + (bc - ad)i) / (c^2 + d^2)\n" << std::endl;
	std::complex<double> sum = my_add(z1, z2);
	std::complex<double> dif = my_sub(z1, z2);
	std::complex<double> prod = my_mul(z1, z2);
	std::complex<double> quot = my_div(z1, z2);

	std::cout << "Полученные результаты (функции прописаны вручную):\n";
	std::cout << "z1 + z2 = "; print_complex(sum); std::cout << "\n";
	std::cout << "z1 - z2 = "; print_complex(dif); std::cout << "\n";
	std::cout << "z1 * z2 = "; print_complex(prod); std::cout << "\n";
	std::cout << "z1 / z2 = "; print_complex(quot); std::cout << "\n" << std::endl;

	std::complex<double> sum_stand = z1 + z2;
	std::complex<double> dif_stand = z1 - z2;
	std::complex<double> prod_stand = z1 * z2;
	std::complex<double> quot_stand = z1 / z2;

	std::cout << "Ожидаемые значения (рассчёты с помощью встроенных функций):\n"
		<< "z1 + z2 = "; print_complex(sum_stand); std::cout << "\n";
	std::cout << "z1 - z2 = "; print_complex(dif_stand); std::cout << "\n";
	std::cout << "z1 * z2 = "; print_complex(prod_stand); std::cout << "\n";
	std::cout << "z1 / z2 = "; print_complex(quot_stand); std::cout << "\n" << std::endl;
}

void task_3(const double a, const double b) {
	std::complex<double> z(a, b);
	std::complex<double> z_conj = std::conj(z);
	std::complex<double> z_inv = my_div(std::complex<double>(1.0, 0.0), z);
	std::complex<double> zero(0.0, 0.0);

	std::cout << std::fixed << std::setprecision(4);
	std::cout << "Исходные числа:" << std::endl;
	std::cout << "z = "; print_complex(z); std::cout << "\n";
	std::cout << "conj(z) = "; print_complex(z_conj); std::cout << "\n";
	std::cout << "1 / z = "; print_complex(z_inv); std::cout << "\n";
	std::cout << "0\n" << std::endl;

	std::cout << "Проверка коллинеарности"; //знаю что аналитически доказывать надо, сделал сугубо для себя
	std::cout << "0, z, conj(z) - " << (are_collinear(zero, z, z_conj) ? "Коллинеарны" : "Неколлинеарны") << "\n";
	std::cout << "0, z, 1/z - " << (are_collinear(zero, z, z_inv) ? "Коллинеарны" : "Неколлинеарны") << "\n";
	std::cout << "0, conj(z), 1/z - " << (are_collinear(zero, z_conj, z_inv) ? "Коллинеарны" : "Неколлинеарны") << "\n";
	std::cout << "z, conj(z), 1/z - " << (are_collinear(z, z_conj, z_inv) ? "Коллинеарны" : "Неколлинеарны") << "\n";

	matplot::hold(matplot::on);

	matplot::plot(std::vector<double> {-Limit, Limit}, std::vector<double> {0.0, 0.0}, "k--")->line_width(1.2);
	matplot::plot(std::vector<double> {0.0, 0.0}, std::vector<double> {-Limit, Limit}, "k--")->line_width(1.2);

	if (are_collinear(zero, z_conj, z_inv)) { //Линия через заведомо известные 3 коллинеарные точки
		double k = Limit / std::abs(z_conj);
		matplot::plot(std::vector<double> {-k * z_conj.real(), k * z_conj.real()}, std::vector<double> {-k * z_conj.imag(), k * z_conj.imag()}, "r--")->line_width(0.8);
	}

	matplot::scatter(std::vector<double>{z.real()}, std::vector<double>{z.imag()})->marker_size(10);
	matplot::scatter(std::vector<double>{z_conj.real()}, std::vector<double>{z_conj.imag()})->marker_size(10);
	matplot::scatter(std::vector<double>{z_inv.real()}, std::vector<double>{z_inv.imag()})->marker_size(10);
	matplot::scatter(std::vector<double>{zero.real()}, std::vector<double>{zero.imag()})->marker_size(10);

	matplot::text(z.real() + 0.15, z.imag() + 0.15, "z");
	matplot::text(z_conj.real() + 0.15, z_conj.imag() - 0.3, "conj(z)");
	matplot::text(z_inv.real() + 0.15,z_inv.imag() - 0.3, "1/z");
	matplot::text(-0.5, -0.4, "0");

	matplot::plot(std::vector<double>{z.real(), z.real()}, std::vector<double>{0.0, z.imag()}, "b--")->line_width(0.8); //проекции для z
	matplot::plot(std::vector<double>{0.0, z.real()}, std::vector<double>{z.imag(), z.imag()}, "b--")->line_width(0.8);

	matplot::plot(std::vector<double>{z_conj.real(), z_conj.real()}, std::vector<double>{0.0, z_conj.imag()}, "g--")->line_width(0.8); //проекция для conj(z)
	matplot::plot(std::vector<double>{0.0, z_conj.real()}, std::vector<double>{z_conj.imag(), z_conj.imag()}, "g--")->line_width(0.8);
	std::stringstream ss_zre, ss_zim, ss_cre, ss_cim;
	ss_zre << std::fixed << std::setprecision(2) << z.real();
	ss_zim << std::fixed << std::setprecision(2) << z.imag();
	ss_cre << std::fixed << std::setprecision(2) << z_conj.real();
	ss_cim << std::fixed << std::setprecision(2) << z_conj.imag();

	matplot::text(z.real() + 0.1, -0.4, "Re conj(z) = " + ss_zre.str());
	matplot::text(-2.2, z.imag() + 0.1, "Im z = " + ss_zim.str());

	matplot::text(z_conj.real() + 0.1, 0.2, "Re Z = " + ss_cre.str());
	matplot::text(-2.5, z_conj.imag() + 0.1, "Im conj(z) = " + ss_cim.str());
	matplot::hold(matplot::off);
	setup_axes(Limit);
	matplot::show();
}

void task_4 (const double a, const double b, const int n) {
	std::complex<double> z(a, b);

	std::cout << std::fixed << std::setprecision(4);
	std::cout << "z = "; print_complex(z); std::cout << "\n";
	std::cout << "n =" << n << "\n" << std::endl;

	std::vector<std::complex<double>> roots(n);
	for (int k = 0; k < n; k++) {
		roots[k] = my_root(z, n, k);
	}

	double mod_z = std::sqrt (a*a + b*b);
	double r = std::pow(mod_z, 1.0 / n);
	double phi = my_arg(a, b);

	std::cout << "|z| = " << mod_z << std::endl;
	std::cout << "r = |z|^(1/n) = " << r << std::endl;
	std::cout << "Шаг угла = " << 360.0 / n << std::endl;

	std::cout << "Все " << n << "корней:" << std::endl;
	for (int k = 0; k < n; k++) {
		double angle_deg = (phi + 2.0 * PI * k) / n * 180.0 / PI;
		std::cout << "k = " << std::setw(2) << k << ": " << std::endl;
		print_complex(roots[k]);
		std::cout << " (угол " << std::setprecision(2) << angle_deg << ").\n";
	}

	matplot::hold(matplot::on);
	matplot::plot(std::vector<double>{-Limit, Limit}, std::vector<double>{0.0, 0.0}, "k--")->line_width(1.0);
	matplot::plot(std::vector<double>{0.0, 0.0}, std::vector<double>{-Limit, Limit}, "k--")->line_width(1.0);

	const int circle_steps = 200;
	std::vector<double> circ_x, circ_y;
	for (int i = 0; i <= circle_steps; i++) {
		double t = 2.0 * PI * i / circle_steps;
		circ_x.push_back(r * std::cos(t));
		circ_y.push_back(r * std::sin(t));
	}
	matplot::plot(circ_x, circ_y, "b--")->line_width(0.8);

	std::vector<double> rx, ry;
	for (int k = 0; k < n; k++) {
		rx.push_back(roots[k].real());
		ry.push_back(roots[k].imag());
	}
	matplot::scatter(rx, ry)->marker_size(10);

	for(int k = 0; k < n; k++) { //радиус-линии, чисто для крЫсоты
		matplot::plot(std::vector<double>{0.0, roots[k].real()}, std::vector<double>{0.0, roots[k].imag()}, "r-")->line_width(0.8);
	}

	matplot::text(roots[0].real() + 0.1, roots[0].imag() + 1, "k = 0");
	matplot::hold(matplot::off);
	setup_axes(1.5);
	matplot::show();

}

std::complex<double> my_add(const std::complex<double>& z1, const std::complex<double>& z2) {
	double a = z1.real(), b = z1.imag();
	double c = z2.real(), d = z2.imag();
	return std::complex<double>(a + c, b + d);
}

std::complex<double> my_sub(const std::complex<double>& z1, const std::complex<double>& z2) {
	double a = z1.real(), b = z1.imag();
	double c = z2.real(), d = z2.imag();
	return std::complex<double>(a - c, b - d);
}

std::complex<double> my_mul(const std::complex<double>& z1, const std::complex<double>& z2) {
	double a = z1.real(), b = z1.imag();
	double c = z2.real(), d = z2.imag();
	return std::complex<double>(a*c - b*d, a*d + b*c);
}

std::complex<double> my_div(const std::complex<double>& z1, const std::complex<double>& z2) {
	double a = z1.real(), b = z1.imag();
	double c = z2.real(), d = z2.imag();

	double denom = c*c + d*d;
	if (std::abs(denom) < EPS) {
		std::cerr << "Ошибка. Сделаю открытие - на ноль мы не делим" << std::endl;
		return std::complex<double>(0.0, 0.0);
	}
	return std::complex<double>((a*c + b*d) / denom, (b*c - a*d) / denom);
}

double my_arg(const double a, const double b) {
	if (std::abs(a) < EPS && std::abs(b) < EPS) {
		return 0.0; //аргумент не определён
	}
	if (std::abs(a) < EPS) {
		return (b > 0) ? PI / 2 : -PI / 2;
	}
	if (a > 0) {
		return std::atan(b / a); //ответ в радианах
	}

	if (b > 0 || std::abs(b) < EPS) {
		return std::atan(b / a) + PI; //фнукция atan возвращает ответ только для первой и четвёртой четверти, потому мы прибавляем/убавляем PI в зависимости от того, какая четверть нам нужна
	} else {
		return std::atan(b / a) - PI;
	}
	
}

void draw_angle_arc(double a, double b, double phi) {
	double r_arc = std::sqrt(a*a + b*b) * 0.35; //делаем радиус дуги как треть от длины вектора, для крЫсоты =)
	const int steps = 50;
	std::vector<double> arc_x, arc_y;
	for (int i = 0; i <= steps; i++) {
		double t = phi * i / steps;
		arc_x.push_back(r_arc * std::cos(t));
		arc_y.push_back(r_arc * std::sin(t));
	}
	matplot::plot(arc_x, arc_y, "r-")->line_width(1.5);

	//делаем красивую подпись угла +- на директрисе
	double label_angle = phi / 2.0;
	double label_r = r_arc + 0.35;

	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << (phi * 180.0 / PI); // угол в градусы переводим
	matplot::text(label_r * std::cos(label_angle), label_r * std::sin(label_angle), "arg = " + ss.str() + "\xc2\xb0"); //\xc2\xc8 - трабл кодировки, переводим в UTF-8
}

void print_complex(const std::complex<double>& z) {
	double re = z.real();
	double im = z.imag();

	if (std::abs(im) < EPS) {
		std::cout << re << std::endl;
	} else if (std::abs(re) < EPS) {
		std::cout << im << "i" << std::endl;
	} else if (im > 0) {
		std::cout << re << " + " << im << "i" << std::endl;
	} else {
		std::cout << re << " - " << -im << "i" << std::endl;
	}
}

void setup_axes(double lim) {
	matplot::xlim({-lim, lim});
	matplot::ylim({-lim, lim});
	matplot::xlabel("Re");
	matplot::ylabel("Im");
	matplot::grid(matplot::on);
}

bool are_collinear(const std::complex<double>& a, const std::complex<double>& b, const std::complex<double>& c) {
	double det = (b.real() - a.real()) * (c.imag() - a.imag()) - (c.real() - a.real()) * (b.imag() - a.imag());
	return std::abs(det) < EPS;
}

std::complex<double> my_root(const std::complex<double>& z, int n, int k) {
	double mod_z = std::sqrt(z.real() * z.real() + z.imag() * z.imag());
	double r = std::pow(mod_z, 1.0 / n);
	double phi = my_arg(z.real(), z.imag());

	double angle = (phi + 2.0 * PI * k) / n;

	return std::complex<double>(r * std::cos(angle), r * std::sin(angle));
}
