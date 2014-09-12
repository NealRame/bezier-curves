#pragma once

#include "common.h"

#include <string>
#include <boost/format.hpp>

namespace nealrame
{
struct Point {
	float x;
	float y;
	std::string name;

	Point & translate(real x, real y);
	Point & translate(const Point &p);

	Point translated(real x, real y) const;
	Point translated(const Point &p) const;

	std::string toString() const;
};
}