#pragma once

#include "common.h"

namespace  nealrame
{

struct Color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;

	Color(real r, real g, real b, real a = 0xff) :
		red(r), green(g), blue(b), alpha(a)
	{ }

	static const Color White;
	static const Color Black;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
};
}
