#pragma once

#include "common.h"
#include "point.h"
#include "size.h"

#include <cmath>

namespace nealrame
{
class Rect {
	Point topLeft_;
	Point bottomRight_;
public:
	std::string name;

	Rect() 
	{ }

	Rect(Point top_left, Point bottom_right);
	Rect(Point top_left, real width, real height);

	Rect & operator=(const Rect &rhs);

	float width() const;
	float height() const;

	Point bottomLeft() const;
	Point bottomRight() const;
	Point bottomMiddle() const;

	Point topLeft() const;
	Point topRight() const;
	Point topMiddle() const;

	Point middleLeft() const;
	Point middleRight() const;

	Point center() const;

	Size size() const;

	/// Tranformations
	Rect & translate(real x, real y);
	Rect & translate(const Point &p);

	Rect translated(real x, real y) const;
	Rect translated(const Point &p) const;

	std::string toString() const;
};
}
