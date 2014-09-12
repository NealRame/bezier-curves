#pragma once

#include "common.h"

namespace nealrame
{
struct Color;
struct Point;
class Rect;
class Bezier;
class Window;
class Painter {
	PIMPL;

	Painter(const Painter &) = delete;
	Painter & operator=(const Painter &) = delete;

public:
	Painter(std::shared_ptr<Window> win);
	Painter(Painter &&rhs);
	virtual ~Painter();

	Painter & operator=(Painter &&rhs);

public:
	bool clear();
	bool setDrawColor(const Color &);
	bool drawPoint(const Point &);
	bool drawLine(const Point &, const Point &);
	bool drawCurve(const Bezier &);
	bool drawRect(const Rect &);
	void present();
};
}
