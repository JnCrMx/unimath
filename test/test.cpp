#include "polynom.hpp"

#include <iostream>
#include <iomanip>
#include <complex>
#include <ostream>
#include <vector>

std::ostream& operator<<(std::ostream& out, const unimath::C c)
{
	out << c.real() << (c.imag() < 0.0 ? "-" : "+") << std::abs(c.imag()) << "i";
	return out;
}

int main()
{
	using P = unimath::polynom;
	using namespace std::complex_literals;

	int deg;
	std::cout << "Enter degree of polynom: " << std::flush;
	std::cin >> deg;

	std::vector<unimath::C> coeffs(deg+1);
	for(int i=0; i<deg+1; i++)
	{
		std::cout << "Enter coefficient for x^" << (deg-i) << ": " << std::flush;
		unimath::C coeff;
		std::cin >> coeff;
		coeffs[i] = coeff;
	}

	unimath::polynom p(coeffs);

	std::cout << "p(z) = " << p << std::endl;
	std::cout << "deg(z) = " << p.deg() << std::endl;

	auto roots = p.roots();

	std::cout << roots.size() << " root(s):" << std::endl;
	std::cout << std::setprecision(10) << std::fixed;
	for(auto r : roots)
	{
		std::cout << "p(" << r << ") = " << p(r) << std::endl;
	}
}
