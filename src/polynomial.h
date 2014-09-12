#pragma once

#include <array>
#include <cmath>
#include <limits>

#include "common.h"

namespace nealrame
{
template <unsigned int N>
struct Polynomial {
	typedef typename std::array<real, N + 1>::size_type size_type;
	typedef Polynomial<N - 1> Derived;

	real factors[N + 1];

	Polynomial()
	{
		memset(factors, 0, (N + 1)*sizeof(real));
	}

	Polynomial(const real factors[N + 1])
	{
		memcpy(this->factors, factors, (N + 1)*sizeof(real));
	}

	/// Retourne le degre du polynome
	unsigned int degree() const
	{
		for (int i = N; i > 0; --i) {
			if (fabs(factors[i]) > std::numeric_limits<real>::epsilon()) {
				return i;
			}
		}
		return 0;
	}

	/// Evalue le polynome pour une valeur donnee
	real operator()(real x) const
	{
		real v = 0;
		unsigned int i = 0;

		do {
			v = v*x + factors[N - i];
		} while (++i < (N + 1));

		return v;
	}

	/// Retourne le coefficient du degre specifie
	real & operator[](size_type i)
	{
		return factors[i];
	}

	/// Retourne le coefficient du degre specifie
	const real & operator[](size_type i) const
	{
		return (*const_cast<Polynomial<N> *>(this))[i];
	}

	/// Retourne le polynome derive
	Derived derived() const
	{
		real derived_factors[N];
		for (int i = 1; i <= N; ++i) {
			derived_factors[i - 1] = i*factors[i];
		}
		return Derived(derived_factors);
	}
};
}