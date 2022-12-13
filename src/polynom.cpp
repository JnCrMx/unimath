#include "polynom.hpp"
#include "types.hpp"
#include "latex.hpp"

#include <limits>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <functional>

namespace unimath
{
	polynom::polynom(std::vector<C> coeffs, bool reverse) : 
		m_coefficients(reverse?std::vector<C>(coeffs.rbegin(), coeffs.rend()):coeffs)
	{
		// remove 0 coefficients for highest x^k
		// e.g. 0x^3 + 0x^2 + x - 5 = x - 5
		while(!m_coefficients.empty() && m_coefficients.back() == 0.0)
		{
			m_coefficients.erase(m_coefficients.end()-1);
		}
	}

	polynom polynom::singular(C coefficient, int degree)
	{
		std::vector<C> c(degree+1);
		c[degree] = coefficient;
		return polynom(c, false);
	}

	polynom polynom::root(C root, int exp)
	{
		polynom p({1, -root});
		polynom r = p;
		for(int i=1; i<exp; i++)
			r = r * p;
		return r;
	}

	polynom polynom::roots(std::vector<C> roots)
	{
		if(roots.empty())
			return polynom({});

		polynom p({1, -roots[0]});
		for(int i=1; i<roots.size(); i++)
			p = p*polynom({1, -roots[i]});
		return p;
	}

	int polynom::deg() const
	{
		return m_coefficients.size() - 1;
	}

	C polynom::operator()(C z) const
	{
		C c = 0;
		for(int i=0; i<m_coefficients.size(); i++)
		{
			c += m_coefficients[i] * std::pow(z, i);
		}
		return c;
	}

	polynom polynom::derivative() const
	{
		if(deg() <= 0)
			return polynom({});

		std::vector<C> coeffs(m_coefficients.size()-1);

		for(int i=1; i<m_coefficients.size(); i++)
		{
			coeffs[i-1] = m_coefficients[i]*C::value_type(i);
		}

		return polynom(coeffs, false);
	}

	polynom polynom::operator*(const polynom other) const
	{
		std::vector<C> coeffs(deg() + other.deg() + 1);

		for(int i=0; i<m_coefficients.size(); i++)
		{
			for(int j=0; j<other.m_coefficients.size(); j++)
			{
				coeffs[i+j] += m_coefficients[i] * other.m_coefficients[j];
			}
		}
		return polynom(coeffs, false);
	}

	polynom polynom::operator+(const polynom other) const
	{
		std::vector<C> coeffs(std::max(m_coefficients.size(), other.m_coefficients.size()));
		for(int i=0; i<m_coefficients.size(); i++)
			coeffs[i] = m_coefficients[i];	
		for(int i=0; i<other.m_coefficients.size(); i++)
			coeffs[i] += other.m_coefficients[i];
		return polynom(coeffs, false);
	}

	polynom polynom::operator-(const polynom other) const
	{
		std::vector<C> coeffs(std::max(m_coefficients.size(), other.m_coefficients.size()));
		for(int i=0; i<m_coefficients.size(); i++)
			coeffs[i] = m_coefficients[i];
		for(int i=0; i<other.m_coefficients.size(); i++)
			coeffs[i] -= other.m_coefficients[i];
		return polynom(coeffs, false);
	}

	polynom polynom::operator-() const
	{
		std::vector<C> coeffs(m_coefficients.size());
		std::transform(m_coefficients.begin(), m_coefficients.end(), coeffs.begin(), [](C c){return -c;});
		return polynom(coeffs, false);
	}

	polynom divHelper(const polynom p, const polynom q, std::vector<C>& coeffs)
	{
		if(p.deg() < q.deg())
			return p;

		C pc = p.m_coefficients[p.m_coefficients.size()-1];
		C qc = q.m_coefficients[q.m_coefficients.size()-1];

		C c = pc/qc;
		coeffs.push_back(c);

		polynom a = q * polynom::singular(c, p.deg() - q.deg());
		polynom b = p - a;

		if(b.deg() == -1)
		{
			for(int i=0; i<(p.deg()-q.deg()); i++)
				coeffs.push_back(0);
		}
		else
		{
			for(int i=b.deg(); i<p.deg()-1; i++)
				coeffs.push_back(0);
		}

		return divHelper(b, q, coeffs);
	}

	std::tuple<polynom, polynom> polynom::operator/(const polynom other) const
	{
		std::vector<C> coeffs;
		polynom remainder = divHelper(*this, other, coeffs);
		return {polynom(coeffs, true), remainder};
	}

	polynom polynom::operator*(const C c) const
	{
		return *this * polynom({c});
	}

	C newtonHelper(polynom p, polynom d, C init, int n)
	{
		C z = init;

		for(int i=0; i<1000; i++)
		{
			z = z - p(z)/d(z);
		}
		return z;
	}

	std::vector<C> polynom::roots(__float epsilon) const
	{
		int d = deg();
		if(d == -1)
			return {C(std::numeric_limits<__float>::quiet_NaN(), std::numeric_limits<__float>::quiet_NaN())};
		if(d == 0)
			return {};
		if(d == 1)
			return {-m_coefficients[0] / m_coefficients[1]};
		if(d == 2)
		{
			auto a = m_coefficients[2];
			auto b = m_coefficients[1];
			auto c = m_coefficients[0];
			return {
				(-b + std::sqrt(std::pow(b, 2) - 4.0*a*c)) / (2.0*a),
				(-b - std::sqrt(std::pow(b, 2) - 4.0*a*c)) / (2.0*a),
			};
		}
		else
		{
			polynom d = derivative();

			C z = 0;

			std::default_random_engine generator;
			std::normal_distribution<double> distribution(-10000, 10000);
			auto rng = std::bind(distribution, generator);

			while(std::abs((*this)(z)) > epsilon)
			{
				C init(rng(), rng());
				z = newtonHelper(*this, d, init, 1000);
			}

			auto [p, r] = (*this)/polynom::root(z);

			auto v = p.roots();
			v.push_back(z);
			return v;
		}
	}

	std::ostream& operator<<(std::ostream& out, const polynom& p)
	{
		if(is_latex(out))
		{
			if(p.deg() == -1)
			{
				out << "0";
				return out;
			}

			for(int i=p.m_coefficients.size()-1; i>=0; i--)
			{
				auto c = p.m_coefficients[i];
				if(c == 0.0)
					continue;
				if(i < p.m_coefficients.size()-1)
				{
					if(c.real() < 0 && c.imag() == 0)
					{
						if(c.real() == -1 && i > 0)
							out << " - ";
						else	
							out << " - " << -c.real() << " ";
					}
					else
					{
						out << " + ";
						if(p.m_coefficients[i] != 1.0 || i==0)
							out << p.m_coefficients[i] << " ";
					}
				}
				else
				{
					if(p.m_coefficients[i] != 1.0 || i==0)
						out << p.m_coefficients[i] << " ";
				}
				
				if(i == 1)
					out << "z";
				if(i > 1)
					out << "z^{" << i << "}";
			}
		}
		else
		{
			out << "[";
			if(p.deg() == -1)
			{
				out << "0]";
				return out;
			}
			for(int i=p.m_coefficients.size()-1; i>=0; i--)
			{
				out << p.m_coefficients[i];
				if(i == 1)
					out << "z + ";
				if(i > 1)
					out << "z^" << i << " + ";
			}
			out << "]";
		}
		return out;
	}

	std::tuple<polynom, std::vector<partial_fraction>> complex_pfd(polynom p, polynom q, __float epsilon)
	{
		auto [s, r] = p/q;

		auto sorter = [](C c1, C c2){
			if (c1.real() == c2.real())
				return c1.imag() < c2.imag();
			return c1.real() < c2.real();
		};

		std::vector<C> roots = q.roots();
		std::sort(roots.begin(), roots.end(), sorter);

		C last = {C(std::numeric_limits<__float>::quiet_NaN(), std::numeric_limits<__float>::quiet_NaN())};
		std::vector<partial_fraction> parts(roots.size());
		std::map<C, int, decltype(sorter)> max_multiplicities(sorter);

		for(int i=0; i<roots.size(); i++)
		{
			C root = roots[i];
			parts[i] = {.coefficient = 0, .root = root, .multiplicity = 1};
			if(std::abs(last-root) < epsilon)
			{
				parts[i].multiplicity = parts[i-1].multiplicity + 1;
				parts[i].root = last;
			}
			max_multiplicities[parts[i].root] = parts[i].multiplicity;

			last = parts[i].root;
		}

		std::vector<int> failed;
		for(int i=0; i<parts.size(); i++)
		{
			auto& part = parts[i];
			C root = part.root;

			if(part.multiplicity == max_multiplicities[root])
			{
				// Zuhaltemethode
				C top = r(root);
				auto [qm, rm] = q/polynom::root(root, part.multiplicity);
				C bottom = qm(root);
				part.coefficient = top/bottom;
			}
			else
			{
				failed.push_back(i);
			}
		}

		if(failed.size() == 1)
		{
			// ein fehlender Koeffizient kann eindeutig ohne LGS berechnet werden
			C z = 0;
			C left = r(z)/q(z);
			C right = 0;
			for(auto pp : parts) right += pp(z); // der Koeffizient ist auf 0 initialisiert und muss somit nicht übersprungen werden

			partial_fraction& fail = parts[failed[0]];
			fail.coefficient = (left - right)*(z-fail.root);
		}

		return {s, parts};
	}

	C partial_fraction::operator()(C z) const
	{
		return coefficient / std::pow(z - root, multiplicity);
	}

	C round(C c, __float epsilon = EPSILON)
	{
		C c2 = c;
		if(std::abs(c2.real()) < epsilon)
			c2 = {0, c2.imag()};
		if(std::abs(c2.imag()) < epsilon)
			c2 = {c2.real(), 0};
		return c2;
	}

	std::ostream& operator<<(std::ostream& out, const unimath::partial_fraction& part)
	{
		if(is_latex(out))
		{
			auto coeff = round(part.coefficient);
			auto root = round(part.root);

			out << "\\frac{" << coeff << "}{";
			if(part.multiplicity > 1) out << "(";
			if(root.real() < 0 && root.imag() == 0)
				out << "z + " << -root.real();
			else
				out << "z - " << root;
			if(part.multiplicity > 1) out << ")^{" << part.multiplicity << "}";
			out << "}";
		}
		else
		{
			out << part.coefficient << "/(z-" << part.root << ")^" << part.multiplicity;
		}
		return out;
	}
}