#include "fourier.hpp"

#include <iostream>
#include <cmath>
#include <fstream>

std::complex<double> drawing(double t, std::vector<std::complex<double>> points)
{
	double p = (t*points.size())/(2*std::numbers::pi);
	int a = std::floor(p);
	int b = std::ceil(p);
	
	double i = p-a;
	return points[b%points.size()]*i + points[a%points.size()]*(1.0-i);
}

int main(int argc, char* argv[])
{
	if(argc < 4)
	{
		std::cerr << "Usage: fourier <input.csv> <output.txt> <steps> [size]" << std::endl;
		return 1;
	}

	std::ifstream in(argv[1]);
	std::vector<std::complex<double>> points;
	
	std::string line;
	while(std::getline(in, line, '\n'))
	{
		std::istringstream iss(line);

		std::string x, y;
		std::getline(iss, x, ',');
		std::getline(iss, y, ',');

		points.push_back({std::stod(x), std::stod(y)});
	}
	double xmax = std::max_element(points.begin(), points.end(), [](auto a, auto b){return a.real() < b.real();})->real();
	double ymax = std::max_element(points.begin(), points.end(), [](auto a, auto b){return a.imag() < b.imag();})->imag();
	double max = std::max(xmax, ymax);
	if(argc >= 5)
		max /= std::stod(argv[4]);
	std::transform(points.begin(), points.end(), points.begin(), [max](auto a){
		return a/max;
	});

	auto d = std::bind(drawing, std::placeholders::_1, points);

	int steps = std::stoi(argv[3]);
	auto c = unimath::cpfourier(d, 2*std::numbers::pi, steps);

	std::ofstream out(argv[2]);
	c.geogebra(out);

	return 0;
}