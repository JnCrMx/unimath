#include "fourier.hpp"
#include "pretty_numbers.hpp"

#include <cmath>
#include <numbers>
#include <future>
#include <iomanip>

namespace unimath
{
	tri_polynom::tri_polynom(std::vector<double> coeffs, double T) : coefficients(coeffs), T(T), w(2*std::numbers::pi/T)
	{
	}

	double tri_polynom::operator()(double t)
	{
		double s = coefficients[0]/2.0;
		for(int i=1; i<coefficients.size(); i++)
		{
			int k = (i-1)/2 + 1;
			if(i%2 == 1)
			{
				s += coefficients[i]*std::cos(k*w*t);
			}
			else
			{
				s += coefficients[i]*std::sin(k*w*t);
			}
		}
		return s;
	}

	std::ostream& operator<<(std::ostream& out, const tri_polynom& p)
	{
		out << std::fixed << p.coefficients[0]/2;
		for(int i=1; i<p.coefficients.size(); i++)
		{
			out << " + " << std::fixed << p.coefficients[i] << " * ";

			int k = (i-1)/2 + 1;
			out << (i%2 == 1?"cos":"sin") << "(" << k << "t * 2pi/" << p.T << ")";
		}

		return out;
	}

	double integral(std::function<double(double)> function, double x1, double x2, int steps = 16*1024)
	{
		double dx = (x2-x1)/steps;
		double sum = 0.0;
		for(double i=0; i<steps; i++)
		{
			sum += function(x1+i*dx);
		}
		return (sum / ((double)steps))*(x2-x1);
	}

	std::complex<double> cintegral(std::function<std::complex<double>(double)> function, double x1, double x2, int steps = 16*1024)
	{
		std::function<double(double)> real = [&function](double t) {return std::real(function(t));};
		std::function<double(double)> imag = [&function](double t) {return std::imag(function(t));};
		return std::complex<double>(
			integral(real, x1, x2, steps),
			integral(imag, x1, x2, steps)	
		);
	}

	tri_polynom fourier(std::function<double(double)> function, double t, int n)
	{
		std::vector<double> coeffs;
		double w = (2.0*std::numbers::pi)/t;
		double p = 2.0/t;

		coeffs.push_back(p*integral(function, 0, t));
		for(double k=1.0; k<=n; k++)
		{
			coeffs.push_back(p*integral([&function, k, w](double t){
				return function(t) * std::cos(k*w*t);
			}, 0, t));
			coeffs.push_back(p*integral([&function, k, w](double t){
				return function(t) * std::sin(k*w*t);
			}, 0, t));
		}

		return tri_polynom(coeffs, t);
	}

	ctri_polynom::ctri_polynom(std::vector<std::complex<double>> coeffs, double T) : coefficients(coeffs), T(T), w(2*std::numbers::pi/T), n((coeffs.size()-1)/2)
	{

	}

	std::complex<double> ctri_polynom::operator()(double t)
	{
		std::complex<double> s{};
		for(int k=-n; k<=n; k++)
		{
			s += coefficients[k+n]*std::polar(1.0, k*w*t);
		}
		return s;
	}

	std::ostream& operator<<(std::ostream& out, const ctri_polynom& p)
	{
		for(int k=-p.n; k<=p.n; k++)
		{
			out << "(" << p.coefficients[k+p.n].real() << " + " << p.coefficients[k+p.n].imag() << "*i)*e^(" << k << "*" << p.w << "*t*i)";
			if(k != p.n)
				out << " + ";
		}
		return out;
	}

	void ctri_polynom::geogebra(std::ostream &out)
	{
		for(int k=-n; k<=n; k++)
		{
			double abs = std::abs(coefficients[k+n]);
			double angle = std::arg(coefficients[k+n]);
			out << std::fixed << abs << "*(cos(" << std::defaultfloat << k*w << "t+" << angle << "),sin("<<k*w<<"t+" << angle << "))";
			if(k != n)
				out << " + ";
		}
	}

	ctri_polynom cfourier(std::function<std::complex<double>(double)> function, double t, int n)
	{
		std::vector<std::complex<double>> coeffs(2*n+1);
		double w = (2.0*std::numbers::pi)/t;
		double p = 1.0/t;

		for(double k=-n; k<=n; k++)
		{
			auto c = p*cintegral([&function, w, k](double t){
				return function(t) * std::polar(1.0, -k*w*t);
			}, 0, t);
			coeffs[k+n] = c;
		}

		return ctri_polynom(coeffs, t);
	}

	ctri_polynom cpfourier(std::function<std::complex<double>(double)> function, double t, int n)
	{
		std::vector<std::complex<double>> coeffs(2*n+1);
		double w = (2.0*std::numbers::pi)/t;
		double p = 1.0/t;

		std::vector<std::future<void>> futures;
		for(double k=-n; k<=n; k++)
		{
			futures.push_back(std::async([&coeffs, &function, w, k, p, t, n](){
				auto c = p*cintegral([&function, w, k](double t){
					return function(t) * std::polar(1.0, -k*w*t);
				}, 0, t);
				coeffs[k+n] = c;
			}));
		}

		for(auto& f : futures) f.wait();

		return ctri_polynom(coeffs, t);
	}
}