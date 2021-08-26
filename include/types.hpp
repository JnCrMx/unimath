#pragma once

#include <complex>

namespace unimath
{
	using __whole = long;
	using __uwhole = unsigned long;
	using __float = double;

	using N = __uwhole;
	using Z = __whole;
	using R = __float;
	using C = std::complex<__float>;

	constexpr __float EPSILON = 0.0000001;
}
