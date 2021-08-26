#include "latex.hpp"

namespace unimath
{
	inline int getlatex()
	{ 
    	static int i = std::ios_base::xalloc();
    	return i;
	}

	std::ostream& latex(std::ostream& out)
	{
		out.iword(getlatex()) = 1; 
		return out;
	}

	bool is_latex(std::ostream &out)
	{
		return out.iword(getlatex()) == 1;
	}

	std::ostream& operator<<(std::ostream& out, const C c)
	{
		if(!is_latex(out))
			return std::operator<<(out, c);

		if(c.imag() == 0)
		{
			out << c.real();
		}
		else if(c.real() == 0)
		{
			if(c.imag() == 1)
				out << "i";
			else if(c.imag() == -1)
				out << "-i";
			else
				out << c.imag() << "i";
		}
		else
		{
			out << "(" << c.real();
			if(c.imag() == 1)
				out << " + i";
			else if(c.imag() == -1)
				out << " - i";
			else
				out << (c.imag()<0?" - ":" + ") << std::abs(c.imag()) << "i";
			out << ")";
		}

		return out;
	}
}