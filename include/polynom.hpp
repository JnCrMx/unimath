#pragma once

#include "types.hpp"

#include <fstream>
#include <ostream>
#include <vector>
#include <tuple>

namespace unimath
{
	class polynom
	{
		public:

			/**
			 * Create a polynom from a list of its coefficients.
			 * By default the argument list will be reserved, so
			 * that the first coefficient is the one for the highest
			 * exponent of x and the last coefficient is the one for x^0.
			 * Zero coefficients will be removed if they appear on the
			 * higher side of the polynom.
			 */
			polynom(std::vector<C> coefficients, bool reverse = true);
			/**
			 * Create a polynom in the form "c*x^k + 0*x^(k-1) + 0*x^(k-2) + ... + 0*x^0"
			 * with coefficient as c and degree as k.
			 */
			static polynom singular(C coefficient, int degree);

			static polynom root(C root, int exp = 1);
			static polynom roots(std::vector<C> roots);

			/**
			 * Computes the sum of this polynom and another polynom,
			 * essentially by adding the indiviual coefficients.
			 */
			polynom operator+(const polynom other) const;
			/**
			 * Computes the difference of this polynom and another polynom,
			 * essentially by subtracing the indiviual coefficients.
			 */
			polynom operator-(const polynom other) const;
			/**
			 * Multiplies this polynom with another polynome
			 * by multiplying each coefficient with every other.
			 */
			polynom operator*(const polynom other) const;
			/**
			 * Performs polynomial division.
			 * p(z)/q(z) with p(z) being this polynom and q(z) the other polynom.
			 * Returns a polynom s(z) and a rest r(z), such that
			 * p(z) = s(z)*q(z) + r(z)
			 */
			std::tuple<polynom, polynom> operator/(const polynom other) const;
			/**
			 * Computes the negative of this polynom,
			 * essentially by taking the negative of each coefficient.
			 */
			polynom operator-() const;

			/**
			 * Scales this polynom.
			 * Each indiviual coefficient is multiplied by the scalar.
			 */
			polynom operator*(const C c) const;

			/**
			 * Computes the polynom for a given value of z.
			 */
			C operator()(C) const;

			/**
			 * Returns the derivative of the polynom.
			 * For the null polynom this is also the null polynom.
			 * For a constant polynom p(z) = c this is the null polymon.
			 */
			polynom derivative() const;

			/**
			 * Returns the degree of the polynom, i.e. the highest exponent.
			 * For the null polynom p(c) = 0, this is -1 and no -infinity.
			 */
			int deg() const;
			/**
			 * Finds all (real and complex) roots of this polynom.
			 * They are returned in no particular order, with
			 * roots of multiplicity m being returned m times
			 * (but also in no particular order and therefore not
			 * necessarily adjacent).
			 */
			std::vector<C> roots(__float epsilon = EPSILON) const;
		private:
			std::vector<C> m_coefficients;

			friend std::ostream& operator<<(std::ostream&, const polynom&);
			friend polynom divHelper(const polynom p, const polynom q, std::vector<C>& coeffs);
	};
	std::ostream& operator<<(std::ostream&, const unimath::polynom&);

	struct partial_fraction
	{
		C coefficient;
		C root;
		int multiplicity;

		C operator() (C) const;
	};
	std::ostream& operator<<(std::ostream&, const unimath::partial_fraction&);

	std::tuple<polynom, std::vector<partial_fraction>> complex_pfd(polynom p, polynom q, __float epsilon = EPSILON);
}
