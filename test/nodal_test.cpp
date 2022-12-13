#include "nodal_analysis.hpp"

#include <array>
#include <complex>
#include <optional>
#include <iostream>

int main()
{
	/*
         +--------------R6------------------+
		 |                                  |
         +------+---R3---+---R4---+---------+
		 |      |        |        |         |
		I01     R1       R5       R2       I02
		 |      |        |        |         |
		 +------+--------+--------+---------+
		            |
				   GND
	*/

	std::array<std::optional<std::complex<double>>, 3> sources = {
		2,
		{},
		1.5
	};

	return 0;
}