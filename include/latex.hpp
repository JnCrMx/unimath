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
	
	template<typename T>
	std::ostream& operator<<(std::ostream& out, const std::complex<T> c)
	{
		if(!is_latex(out))
			return std::operator<<(out, c);

		if(c.imag() == T(0))
		{
			out << c.real();
		}
		else if(c.real() == T(0))
		{
			if(c.imag() == T(1))
				out << "i";
			else if(c.imag() == T(-1))
				out << "-i";
			else
				out << c.imag() << "i";
		}
		else
		{
			out << "(" << c.real();
			if(c.imag() == T(1))
				out << " + i";
			else if(c.imag() == T(-1))
				out << " - i";
			else
				out << (has_leading_minus(c.imag())?" - ":" + ") << strip_leading_minus(c.imag()) << "i";
			out << ")";
		}
		return out;
	}
}