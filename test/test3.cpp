#include "latex.hpp"
#include "matrix.hpp"
#include "fraction.hpp"

#include <iostream>

int main()
{
	/*unimath::matrix<unimath::fraction> m({
        {1, 0, 9, 5, 7},
        {0, 2, 1, 3, 1},
        {1, 5, 8, 9, 0},
        {7, 0, 1, 7, 4},
        {9, 4, 3, 1, 6}
	});*/

	/*unimath::matrix<std::complex<unimath::fraction>> m({
		{{-2}, {0, 2}, {0,  0}, { 2}, { 0}},
		{{-4}, {0, 4}, {0, -1}, { 7}, {-2}},
		{{ 1}, {0, 1}, {2,  0}, {-1}, { 2}}
	});*/

	/*std::cout << unimath::latex;
	m.nzsf(&std::cout);
	std::cout << std::endl;*/

	/*unimath::matrix<unimath::fraction> m({
		{-1, 3, 1},
		{1, 0, 0},
		{4, 3, 3},
		{0, 2, 3}
	});*/

	//std::cout << unimath::latex << m << "\\cdot";
	//std::cout << m << std::endl;
	/*std::cout << m.inverse() << std::endl;
	std::cout << m.inverse() * m << std::endl;*/

	/*m.nzsf();
	std::cout << m << std::endl;*/

	unimath::matrix<unimath::fraction> m1({
		{1, -1, 1},
		{0, 1, -1},
		{0, 0, 1}
	});
	unimath::matrix<unimath::fraction> m2({
		{1, 1, -1},
		{0, 2, 1},
		{0, 0, 3}
	});
	unimath::matrix<unimath::fraction> m3({
		{1, 1, 0},
		{0, 1, 1},
		{0, 0, 1}
	});
	std::cout << m1*m2*m3 << std::endl;
}