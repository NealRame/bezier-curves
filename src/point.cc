#include "point.h"

using namespace nealrame;

Point & Point::translate(real x, real y)
{
	this->x += x; this->y += y;
	return *this;
}

Point & Point::translate(const Point &p)
{
	return translate(p.x, p.y);
}

Point Point::translated(real x, real y) const
{
	return Point(*this).translate(x, y);
}

Point Point::translated(const Point &p) const
{
	return Point(*this).translate(p);
}

std::string Point::toString() const
{
	return (boost::format("%1%(%2%,%3%)") % name % x % y).str();
}
