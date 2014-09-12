#include "rect.h"

using namespace nealrame;

Rect::Rect(Point top_left, Point bottom_right) :
	topLeft_({
		std::min(top_left.x, bottom_right.x),
		std::min(top_left.y, bottom_right.y)
	}),
	bottomRight_({
		std::max(top_left.x, bottom_right.x),
		std::max(top_left.y, bottom_right.y)
	})
{ }

Rect::Rect(Point top_left, real width, real height)
{
	*this = Rect(
		top_left,
		Point{top_left.x + width, top_left.y + height}
	);
}

Rect & Rect::operator=(const Rect &rhs)
{
	topLeft_ = rhs.topLeft_;
	bottomRight_ = rhs.bottomRight_;
	return *this;
}

float Rect::width() const
{ return fabs(bottomRight_.x - topLeft_.x); }

float Rect::height() const
{ return fabs(bottomRight_.y - topLeft_.y); }

Point Rect::bottomLeft() const
{ return {topLeft_.x, bottomRight_.y}; }

Point Rect::bottomRight() const 
{ return bottomRight_; }

Point Rect::bottomMiddle() const
{ return {(topLeft_.x + bottomRight_.x)/2, bottomRight_.y}; }

Point Rect::topLeft() const
{ return topLeft_; }

Point Rect::topRight() const
{ return {bottomRight_.x, topLeft_.y}; }

Point Rect::topMiddle() const
{ return {(topLeft_.x + bottomRight_.x)/2, bottomRight_.y}; }

Point Rect::middleLeft() const
{ return {topLeft_.x, (topLeft_.y + bottomRight_.y)/2}; }

Point Rect::middleRight() const
{ return {bottomRight_.x, (topLeft_.y + bottomRight_.y)/2}; }

Point Rect::center() const
{ return {(topLeft_.x + bottomRight_.x)/2, (topLeft_.y + bottomRight_.y)/2}; }

Size Rect::size() const
{ return {width(), height()}; }

Rect & Rect::translate(real x, real y)
{
	topLeft_.translate(x, y);
	bottomRight_.translate(x, y);
	return *this;
}

Rect & Rect::translate(const Point &p)
{
	topLeft_.translate(p);
	bottomRight_.translate(p);
	return *this;
}

Rect Rect::translated(real x, real y) const
{
	return Rect(*this).translate(x, y);
}

Rect Rect::translated(const Point &p) const
{
	return Rect(*this).translate(p);
}

std::string Rect::toString() const
{
	return (boost::format("%s[%1%, %2%, %3%, %4%]")
			% name
			% topLeft_.x
			% topLeft_.y
			% width() % height()).str();
}