#pragma once

#include <vector>
#include <functional>
#include <ostream>
#include <complex>

namespace unimath
{
	class tri_polynom
	{
		public:
			tri_polynom(std::vector<double> coeffs, double T);

			double operator()(double t);

			std::vector<double> coefficients;
			double T;
			double w;

			friend std::ostream& operator<<(std::ostream& out, const tri_polynom& p);
	};

	class ctri_polynom
	{
		public:
			ctri_polynom(std::vector<std::complex<double>> coeffs, double T);

			std::complex<double> operator()(double t);

			std::vector<std::complex<double>> coefficients;
			int n;
			double T;
			double w;

			friend std::ostream& operator<<(std::ostream& out, const ctri_polynom& p);
			void geogebra(std::ostream& out);
	};

	tri_polynom fourier(std::function<double(double)> function, double t, int n);

	ctri_polynom cfourier(std::function<std::complex<double>(double)> function, double t, int n);
	ctri_polynom cpfourier(std::function<std::complex<double>(double)> function, double t, int n);
}
