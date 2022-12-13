#include "fraction.hpp"

#include <iostream>

int main()
{
	const unimath::fraction target = 2;

	unimath::fraction a = 0;
	unimath::fraction b = 2;

	for(;;)
	{
		unimath::fraction mid = (a+b)/2;
		unimath::fraction midsq = mid*mid;

		std::cout << a  << " " << b << " | " << mid << " = " << midsq << std::endl;

		if(midsq < target)
			a = mid;
		else
		 	b = mid;
	}
}