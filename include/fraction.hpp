#pragma once

#include <ostream>

namespace unimath
{
	class fraction
	{
		public:
			fraction(int p = 0, int q = 1);
			fraction(long p, long q);
			fraction(float f);

			bool operator==(const fraction other) const;
			std::strong_ordering operator<=>(const fraction other) const;

			fraction operator+(const fraction other) const;
			fraction operator-(const fraction other) const;
			fraction operator*(const fraction other) const;
			fraction operator/(const fraction other) const;

			fraction& operator+=(const fraction other);
			fraction& operator-=(const fraction other);
			fraction& operator*=(const fraction other);
			fraction& operator/=(const fraction other);

			fraction operator-() const;
		protected:
			void clean();
		private:
			long m_p;
			long m_q;

			friend std::ostream& operator<<(std::ostream&, const fraction);
	};
	std::ostream& operator<<(std::ostream&, const fraction);
}