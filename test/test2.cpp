#include "polynom.hpp"
#include "latex.hpp"

#include <iomanip>
#include <ios>
#include <iostream>

int main()
{
	using namespace std::complex_literals;

	//unimath::polynom p({1, -4, -2, 17});
	//unimath::polynom q({1, -6, 9});

	//unimath::polynom p({1, -1, 0});
	//unimath::polynom q({1, -2, 10});

	//unimath::polynom p({3, 1, 0});
	//unimath::polynom q({1, 1, -1, -1});

	//unimath::polynom p({7, 44, 55});
	//unimath::polynom q({1, 2, -15, -36});

	//unimath::polynom p({8, 0});
	//unimath::polynom q({1, 0, -4});

	//unimath::polynom p({11, 18});
	//unimath::polynom q({1, 1, -6});

	//unimath::polynom p({1, -4, -2, 17});
	//unimath::polynom q({1, -6, 9});

	unimath::polynom p({1});
	unimath::polynom q({1, -6, 12, -8});

	auto [s, parts] = unimath::complex_pfd(p, q, 1e-4);

	std::cout << unimath::latex;
	std::cout << "p(x) &= " << p << "\\\\" << std::endl;
	std::cout << "q(x) &= " << q << "\\\\" << std::endl;
	std::cout << std::setprecision(5);

	unimath::C z = 1.0 + 1.0i;
	unimath::C val = s(z);

	std::cout << "\\frac{p(x)}{q(x)} = " << "\\frac{" << p << "}{" << q << "} &= ";
	if(s.deg() >= 0)
		std::cout << s << " + ";

	for(int i=0; i<parts.size(); i++)
	{
		auto part = parts[i];
		std::cout << (i==0?"":" + ") << part;
		val += part(z);
	}
	std::cout << std::endl;
	
	unimath::C realVal = p(z)/q(z);
	std::cout << realVal << " = " << val << " => " << std::boolalpha << (std::abs(realVal-val) < unimath::EPSILON) << std::endl;

	return 0;
}
