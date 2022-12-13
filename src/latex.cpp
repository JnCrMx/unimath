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
}