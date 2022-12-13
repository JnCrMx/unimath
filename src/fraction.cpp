#include "fraction.hpp"
#include "latex.hpp"

#include <compare>
#include <numeric>

namespace unimath
{
	fraction::fraction(int p, int q) : m_p(p), m_q(q)
	{
		clean();
	}

	fraction::fraction(long p, long q) : m_p(p), m_q(q)
	{
		clean();
	}

	fraction::fraction(float f)
	{
		long q;
		for(q = 1; ((long)f)!=f; q*=10, f*=10);

		m_p = (long)f;
		m_q = q;
		clean();
	}

	void fraction::clean()
	{
		if(m_p == 0)
		{
			m_q = 1;
			return;
		}

		long gcd = std::gcd(m_p, m_q);
		m_p /= gcd;
		m_q /= gcd;

		if(m_q < 0)
		{
			m_q = -m_q;
			m_p = -m_p;
		}
	}

	bool fraction::operator==(const fraction other) const
	{
		return m_p == other.m_p && m_q == other.m_q;
	}
	std::strong_ordering fraction::operator<=>(const fraction other) const
	{
		auto p1 = m_p * other.m_q;
		auto p2 = other.m_p * m_q;
		return p1 <=> p2;
	}

	fraction fraction::operator+(const fraction other) const
	{
		return {m_p * other.m_q + other.m_p * m_q, m_q * other.m_q};
	}
	fraction fraction::operator-(const fraction other) const
	{
		return {m_p * other.m_q - other.m_p * m_q, m_q * other.m_q};
	}
	fraction fraction::operator*(const fraction other) const
	{
		return {m_p * other.m_p, m_q * other.m_q};
	}
	fraction fraction::operator/(const fraction other) const
	{
		return {m_p * other.m_q, m_q * other.m_p};
	}

	fraction& fraction::operator+=(const fraction other)
	{
		m_p = m_p * other.m_q + other.m_p * m_q;
		m_q = m_q * other.m_q;
		clean();
		return *this;
	}
	fraction& fraction::operator-=(const fraction other)
	{
		m_p = m_p * other.m_q + other.m_p * m_q;
		m_q = m_q * other.m_q;
		clean();
		return *this;
	}
	fraction& fraction::operator*=(const fraction other)
	{
		m_p = m_p * other.m_p;
		m_q = m_q * other.m_q;
		clean();
		return *this;
	}
	fraction& fraction::operator/=(const fraction other)
	{
		m_p = m_p * other.m_q;
		m_q = m_q * other.m_p;
		clean();
		return *this;
	}

	fraction fraction::operator-() const
	{
		return {-m_p, m_q};
	}

	std::ostream& operator<<(std::ostream& out, const fraction f)
	{
		if(is_latex(out))
		{
			if(is_one(f.m_q))
				out << f.m_p;
			else
			{
				if(has_leading_minus(f.m_p))
					out << "-";
				out << "\\frac{" << strip_leading_minus(f.m_p) << "}{" << f.m_q << "}";
			}
		}
		else
		{
			if(f.m_q == 1)
				out << f.m_p;
			else
				out << "(" << f.m_p << "/" << f.m_q << ")";
		}
		return out;
	}
}