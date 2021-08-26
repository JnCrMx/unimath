#pragma once

#include <ostream>

namespace unimath
{
	class fraction
	{
		public:
			fraction(int p = 0, int q = 1);
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
			int m_p;
			int m_q;

			friend std::ostream& operator<<(std::ostream&, const fraction);
	};
	std::ostream& operator<<(std::ostream&, const fraction);
}