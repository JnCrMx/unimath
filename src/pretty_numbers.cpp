#include "pretty_numbers.hpp"

#include <numbers>
#include <cmath>

namespace unimath
{
	bool is_close_int(double d)
	{
		return std::abs(std::round(d)-d) < 1e-3;
	}

	std::string pretty(double d)
	{
		if(std::abs(d) < 1e-3)
			return "0";

		if(is_close_int(d))
			return std::to_string(std::round(d));
		
		if(is_close_int(std::numbers::pi*d))
			return std::to_string(std::round(std::numbers::pi*d))+"/pi";

		return std::to_string(d);
	}
}