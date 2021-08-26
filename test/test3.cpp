#include "latex.hpp"
#include "matrix.hpp"
#include "fraction.hpp"

#include <iostream>

int main()
{
	unimath::matrix<unimath::fraction> m({
        {1, 0, 9, 5, 7},
        {0, 2, 1, 3, 1},
        {1, 5, 8, 9, 0},
        {7, 0, 1, 7, 4},
        {9, 4, 3, 1, 6}
	});

	/*std::cout << unimath::latex;
	m.nzsf(&std::cout);
	std::cout << std::endl;*/

	//std::cout << unimath::latex << m << "\\cdot";
	std::cout << m << std::endl;
	std::cout << m.inverse() << std::endl;
	std::cout << m.inverse() * m << std::endl;
}