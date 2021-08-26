#pragma once

#include "types.hpp"

#include <complex>
#include <ostream>

namespace unimath
{
	class NullBuffer : public std::streambuf
	{
		public:
			int overflow(int c) { return c; }
	};


	std::ostream& latex(std::ostream& out);
	inline int getlatex();

	bool is_latex(std::ostream& out);

	std::ostream& operator<<(std::ostream &, const C);

	template<typename T>
	bool has_leading_minus(T t) {return t < 0;}

	template<typename T>
	bool has_leading_minus(std::complex<T> t) {return std::real(t) < 0 || (std::real(t)==0 && std::imag(t)<0);}

	template<typename T>
	bool is_zero(T t) {return t == T(0);}

	template<typename T>
	bool is_one(T t) {return t == T(1);}

	template<typename T>
	T strip_leading_minus(T t)
	{
		if(has_leading_minus(t))
			return -t;
		return t;
	}
}