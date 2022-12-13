#pragma once

#include "latex.hpp"

#include <functional>
#include <iomanip>
#include <iterator>
#include <limits>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include <iostream>

namespace unimath
{
	template<typename K>
	class matrix
	{
		public:
			matrix(std::vector<std::vector<K>> entries) : m_entries(entries)
			{
				m_columns = 0;
				m_rows = m_entries.size();
				for(auto i=0; i<m_rows; i++)
					m_columns = std::max(m_columns, (int)m_entries[i].size());
				for(auto i=0; i<m_rows; i++)
					m_entries[i].resize(m_columns);
			}

			static matrix<K> identity(int n)
			{
				std::vector<std::vector<K>> entries(n);
				for(int i=0; i<n; i++)
				{
					entries[i].resize(n);
					entries[i][i] = K(1);
				}
				return matrix<K>(entries);
			}

			void zsf(int row=0, int col=0, std::ostream* out = nullptr)
			{
				if(row == m_entries.size()-1)
					return;
				if(out) *out << *this;

				int c = std::numeric_limits<int>::max();
				for(int j=row; j<m_entries.size(); j++)
				{
					auto& row = m_entries[j];
					for(int i=col; i<std::min((int)row.size(), c); i++)
						if(row[i] != K(0)) c = i;
				}
				if(c == std::numeric_limits<int>::max())
					return;
				
				{
					int r;
					for(int i=row; i<m_entries.size(); i++)
						if(m_entries[i][c] != K(0)) {r = i; break;}
					if(r != row)
					{
						swap(row, r, out);
						if(out) *out << *this;
					}
				}

				K pivot = m_entries[row][c];
				for(int i=row+1; i<m_entries.size(); i++)
				{
					K sc = m_entries[i][c];
					K cc = (-sc/pivot);
					if(cc != K(0))
					{
						add(i, cc, row, out);
						if(out) *out << *this;
					}
				}
				if(out) { if(is_latex(*out)) *out << "\\\\" << std::endl; else *out << std::endl; }

				zsf(row+1, c+1, out);
			}

			void nzsf(std::ostream* out = nullptr)
			{
				zsf(0, 0, out);
				if(out) { if(is_latex(*out)) *out << "\\cline{-}" << std::endl; else *out << "--------------------\n" << std::endl; }

				for(int i=0; i<m_entries.size(); i++)
				{
					if(out) *out << *this;

					auto& row = m_entries[i];
					int c = 0;
					for(c=0; c<row.size() && row[c] == K(0); c++);

					if(c==row.size() || row[c] == K(0))
						continue;

					if(row[c] != K(1))
					{
						multiply(i, K(1)/row[c], out);
						if(out) *out << *this;
					}

					for(int j=i-1; j>=0; j--)
					{
						auto& row2 = m_entries[j];
						if(row2[c] != K(0))
						{
							add(j, -row2[c], i, out);
							if(out) *out << *this;
						}
					}
					if(out && i!=m_entries.size()-1) { if(is_latex(*out)) *out << "\\\\" << std::endl; else *out << std::endl; }
				}
			}

			matrix<K> inverse(std::ostream* out = nullptr)
			{
				if(m_rows != m_columns)
					throw std::logic_error("matrix is not quadratic");

				int n = m_rows;

				matrix<K> work = concat(*this, matrix<K>::identity(n));
				work.nzsf(out);

				if(work.rank(n) != n)
					throw std::logic_error("matrix is not invertible");

				return work.submat(0, n);
			}

			matrix<K> operator+(const matrix<K> other) const
			{
				return binary_transform(*this, other, std::plus<K>());
			}
			matrix<K> operator-(const matrix<K> other) const
			{
				return binary_transform(*this, other, std::minus<K>());
			}
			matrix<K> operator*(const matrix<K> other) const
			{
				if(m_columns != other.m_rows)
					throw std::logic_error("colum count of A does not match row count of B");

				int r = m_rows;
				int c = other.m_columns;

				std::vector<std::vector<K>> entries(r);
				for(int i=0; i<r; i++)
				{
					auto& row = entries[i];
					row.resize(c);
					for(int j=0; j<c; j++)
					{
						K sum = K(0);
						for(int a=0; a<m_columns; a++)
							sum += m_entries[i][a] * other.m_entries[a][j];
						row[j] = sum;
					}
				}
				return matrix<K>(entries);
			}
		protected:
			void swap(int a, int b, std::ostream* out = nullptr)
			{
				if(a==b)
					return;

				std::swap(m_entries[a], m_entries[b]);

				if(out)
				{
					if(is_latex(*out))
					{
						*out 	<< "\\xrightarrow{\\text{\\MakeUppercase{\\romannumeral" << (a+1) 
								<< "}}\\leftrightarrow\\text{\\MakeUppercase{\\romannumeral" << (b+1) << "}}}";
					}
					else
					{
						*out << "swap (" << (a+1) << ") <-> (" << (b+1) << ")" << std::endl;
					}
				}
			}

			void add(int a, K c, int b, std::ostream* out = nullptr)
			{
				auto& va = m_entries[a];
				auto& vb = m_entries[b];

				for(int i=0; i<va.size(); i++)
					va[i] += c * vb[i];

				if(out)
				{
					if(is_latex(*out))
					{
						*out 	<< "\\xrightarrow{\\text{\\MakeUppercase{\\romannumeral" << (a+1) 
								<< "}}";
						*out << (has_leading_minus(c)?"-":"+");
						if(c != K(1) && c != K(-1))
							*out << strip_leading_minus(c) << "\\cdot";
						*out << "\\text{\\MakeUppercase{\\romannumeral" << (b+1) << "}}}";
					}
					else
					{
						*out << "add (" << (a+1) << ") + " << c << " * (" << (b+1) << ")" << std::endl;
					}
				}
			}

			void multiply(int a, K c, std::ostream* out = nullptr)
			{
				auto& va = m_entries[a];
				for(int i=0; i<va.size(); i++)
					va[i] *= c;

				if(out)
				{
					if(is_latex(*out))
					{
						*out 	<< "\\xrightarrow{\\text{\\MakeUppercase{\\romannumeral" << (a+1) 
								<< "}}\\cdot ";
						if(has_leading_minus(c)) *out << "\\left(";
						*out << c;
						if(has_leading_minus(c)) *out << "\\right)";
						*out << "}";
					}
					else
					{
						*out << "multiply (" << (a+1) << ") * " << c << std::endl;
					}
				}
			}

			int rank(int cols = 0)
			{
				if(cols == 0) cols = m_columns;
				int rank = 0;
				for(auto& row : m_entries)
				{
					for(int c=0; c<cols; c++)
					{
						if(row[c] != K(0))
						{
							rank++;
							break;
						}
					}
				}
				return rank;
			}

			matrix<K> submat(int r=0, int c=0, int rr=0, int cc=0)
			{
				if(rr==0) rr = m_rows;
				if(cc==0) cc = m_columns;

				std::vector<std::vector<K>> entries(rr-r);
				for(int i=0; i<entries.size(); i++)
				{
					auto& v = entries[i];
					v.resize(cc-c);
					std::copy(m_entries[i+r].begin()+c, m_entries[i+r].begin()+cc, v.begin());
				}
				return matrix<K>(entries);
			}
		private:
			int m_rows;
			int m_columns;
			std::vector<std::vector<K>> m_entries;

			template<typename T>
			friend std::ostream& operator<<(std::ostream& out, const matrix<T>& matrix);
			
			template<typename M, typename N>
			friend matrix<std::common_type_t<M, N>> concat(matrix<M> m, matrix<N> n);
			
			template<typename A, typename B, class BinaryOperation>
			friend auto binary_transform(const matrix<A>& m1, const matrix<B>& m2, const BinaryOperation& function) -> matrix<decltype(function(0, 0))>;
	};

	template<typename A, typename B, class BinaryOperation>
	auto binary_transform(const matrix<A>& m1, const matrix<B>& m2, const BinaryOperation& function) -> matrix<decltype(function(0, 0))>
	{
		if(m1.m_columns != m2.m_columns)
			throw std::logic_error("column count does not match");
		if(m1.m_rows != m2.m_rows)
			throw std::logic_error("row count does not match");
			
		int rows = m1.m_rows;
		int columns = m1.m_columns;

		using C = decltype(function(0, 0));
		std::vector<std::vector<C>> entries(rows);
		for(int i=0; i<rows; i++)
		{
			auto& v = entries[i];
			v.reserve(columns);

			std::transform(m1.m_entries[i].begin(), m1.m_entries[i].end(), m2.m_entries[i].begin(), 
              		std::back_inserter(v), function);
		}
		return matrix<C>(entries);
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const matrix<T>& matrix)
	{			
		if(is_latex(out))
		{
			out << "\\begin{bmatrix}";
			for(int j=0; j<matrix.m_entries.size(); j++)
			{
				auto& row = matrix.m_entries[j];
				for(int i=0; i<row.size(); i++)
				{
					out << row[i];
					if(i != row.size() - 1)
						out << " & ";
				}
				if(j != matrix.m_entries.size()-1)
					out << "\\\\";
			}
			out << "\\end{bmatrix}";
		}
		else
		{
			int len=0;
			for(auto& row : matrix.m_entries)
			{
				for(auto e : row)
				{
					std::ostringstream oss;
					oss << e;
					len = std::max(len, (int)oss.tellp());
				}
			}

			for(auto& row : matrix.m_entries)
			{	
				if(row == matrix.m_entries.front())
					out << "⎡ ";
				else if(row == matrix.m_entries.back())
					out << "⎣ ";
				else
					out << "⎢ ";
				
				for(auto e : row)
				{
					std::ostringstream oss;
					oss << e;
					out << std::setw(len) << oss.str() << " ";
				}

				if(row == matrix.m_entries.front())
					out << "⎤" << std::endl;
				else if(row == matrix.m_entries.back())
					out << "⎦" << std::endl;
				else
					out << "⎥" << std::endl;
			}
		}
		return out;
	}

	template<typename M, typename N>
	matrix<std::common_type_t<M, N>> concat(matrix<M> m, matrix<N> n)
	{
		if(m.m_rows != n.m_rows)
			throw std::logic_error("row count not identical");

		using X = std::common_type_t<M, N>;
		int rows = m.m_rows;

		std::vector<std::vector<X>> entries(rows);
		for(int i=0; i<rows; i++)
		{
			auto& v = entries[i];
			v.resize(m.m_columns + n.m_columns);
			std::copy(m.m_entries[i].begin(), m.m_entries[i].end(), v.begin());
			std::copy(n.m_entries[i].begin(), n.m_entries[i].end(), v.begin() + m.m_columns);
		}

		return matrix<X>(entries);
	}
}