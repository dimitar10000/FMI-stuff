#define _USE_MATH_DEFINES

#include <iostream>
#include <string>
#include <vector>
#include <math.h>

const double EPS = 0.0001;

double func(double x) {
	return pow(sin(3 * M_PI * x), 2) + (x - 1) * (x - 1);
}

double first_deriv(double x) {
	return 6*M_PI*cos(3*M_PI*x) * sin(3*M_PI*x) + 2*(x - 1);
}

double sec_deriv(double x) {
	return -18 * M_PI * M_PI * pow(sin(3 * M_PI * x), 2) + 18 * M_PI * M_PI * pow(cos(3 * M_PI * x), 2) + 2;
}

std::vector<std::pair<double, double>> newtons_method(double start_point,int max_iter) {
	double x = start_point;

	std::vector<std::pair<double, double>> result;
	result.push_back(std::make_pair(x, func(x)));

	for (int i = 0; i < max_iter; i++) {
		double df = first_deriv(x);

		std::cout << "first deriv: " << df << std::endl;
		
		if (abs(df) < EPS) {
			std::cout << "found solution\n";
			std::cout << "number of iterations: " << i << std::endl;
			return result;
		}

		double ddf = sec_deriv(x);

		std::cout << "second deriv: " << ddf << std::endl;

		if (ddf == 0) {
			std::cout << "2nd div is 0, abort\n";
			return result;
		}

		x = x - df / ddf;
		result.push_back(std::make_pair(x, func(x)));
	}

	std::cout << "exceeded max iterations\n";
	return result;
}

void print(std::vector<std::pair<double, double>> vec) {
	for (int i = 0; i < vec.size(); i++) {
		std::cout << vec[i].first << ' ' << vec[i].second << std::endl;
	}
}

std::vector<std::pair<double, double>> armijos_method(double start_point,double b, int max_iter) {

	double x = start_point;
	double t = 0.5;

	std::vector<std::pair<double, double>> result;
	result.push_back(std::make_pair(x, func(x)));

	for (int i = 0; i < max_iter; i++) {
		std::cout << "x = " << x << std::endl;
		double df = first_deriv(x);
		double step_size = 1;

		std::cout << "first deriv: " << df << std::endl;

		if (abs(df) < EPS) {
			std::cout << "gradient zero, return\n";
			std::cout << "number of iterations: " << i << std::endl;
			return result;
		}

		double curr_value = func(x);

		while (func(x - step_size * df) > curr_value - step_size * b * df * df) {
			step_size = step_size * t;
		}
		std::cout << "step size is " << step_size << std::endl;
		// weve found the optimal step size for this iteration

		x = x - step_size * df;

		result.push_back(std::make_pair(x, func(x)));
	}

	std::cout << "exceeded max iterations\n";
	return result;
}

int main() {
	std::vector<std::pair<double, double>> vec;
	std::vector<std::pair<double, double>> vec2;

	vec = newtons_method(1.05, 100);
	std::cout << std::endl;
	print(vec);
	std::cout << std::endl;

	vec2 = armijos_method(1.05, 0.0001, 100);
	std::cout << std::endl;
	print(vec2);
	
	return 0;
}