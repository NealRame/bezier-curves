#include <algorithm>
#include <array>
#include <cmath>
#include <exception>
#include <functional>
#include <iostream>

#include <memory>
#include <sstream>
#include <string>
#include <type_traits>

#include "bezier.h"
#include "color.h"
#include "context.h"
#include "error.h"
#include "point.h"
#include "rect.h"
#include "painter.h"
#include "window.h"

using namespace nealrame;

class Parenthesis {
	Bezier extern_, intern_;
public:
	enum Type {
		Opening, Closing
	};

	Parenthesis(Rect box, Type type, real weight, real ratio)
	{
		Point p1, p2, B, C;

		switch (type) {
		case Opening:
			p1 = box.topRight();
			p2 = box.bottomRight();
			C = box.middleRight();
			B = box.middleLeft();
			weight = std::fabs(weight);
			break;
		case Closing:
			p1 = box.topLeft();
			p2 = box.bottomLeft();
			C = box.middleLeft();
			B = box.middleRight();
			weight = -1*std::fabs(weight);
		}
		
		auto A = Point{B.x - (C.x - B.x)/3, B.y};
		auto d = std::fabs(p2.y - p1.y)*ratio;
		auto c1 = Point{A.x, p1.y + d};
		auto c2 = Point{A.x, p2.y - d};

		extern_ = Bezier(p1, c1, c2, p2);
		intern_ = Bezier(p1, {c1.x + weight, c1.y},{c2.x + weight, c2.y}, p2);
	}

	bool draw(Painter &painter)
	{
		painter.setDrawColor(Color{0x00, 0xff, 0x00});
		painter.drawCurve(extern_);
		painter.drawCurve(intern_);
		// painter.drawPoint(p1);
		// painter.drawPoint(p2);
		// painter.setDrawColor({0xff, 0x00, 0x00});
		// painter.drawPoint(A);
		// painter.drawPoint(B);
		// painter.drawPoint(C);
		return true;
	}
};

template<unsigned int N>
Point * select(std::array<Point, N> &points, Point p) {
	auto end = points.end();
	auto it = std::find_if(points.begin(), end, [&](Point &point) {
		auto d = std::sqrt(SQUARE(point.x - p.x) + SQUARE(point.y - p.y));
		return d <= 5;
	});
	return it != end ? &(*it) : nullptr;
}

int main(int argc, char **argv) {
	try {
		auto window = Context::instance().createWindow("Hello World!", 640, 480);
		auto painter = Context::instance().createPainter(window);

		bool cont = true;
		bool drag = false;
		Point p1, p2;

		auto box = Rect({128, 64}, {256, 340});

		auto on_quit = [&](const Window::EventData &){cont = false;};

		window->on(SDL_QUIT, on_quit);
		window->on(SDL_KEYDOWN, on_quit);

		window->on(
			SDL_MOUSEBUTTONDOWN, 
			[&](const Window::EventData &data){
				drag = true;
				p1 = p2 = {
					float(data.button.x),
					float(data.button.y)
				};
			}
		);

		window->on(
			SDL_MOUSEBUTTONUP,
			[&](const Window::EventData &data){
				drag = false;
				box = {p1, p2};
			}
		);

		window->on(
			SDL_MOUSEMOTION,
			[&](const Window::EventData &data){
				if (! drag) return;
				p2 = {
					float(data.motion.x), 
					float(data.motion.y)
				};
			}
		);

		do {
			window->pollEvent();

			painter->setDrawColor({0, 0, 0});
			painter->clear();

			painter->setDrawColor({ 0xff, 0xff, 0xff });
			painter->drawRect(Rect(p1, p2));

			// painter->setDrawColor({ 0x00, 0xff, 0x00 });
			// painter->drawLine(t1, t2);

			// painter->setDrawColor({ 0xff, 0x00, 0x00 });

			// painter->drawPoint(p1);
			// painter->drawPoint(p2);

			// painter->drawPoint(A);
			// painter->drawPoint(B);
			// painter->drawPoint(C);

			// // painter->drawPoint(t1);
			// // painter->drawPoint(t2);

			// painter->drawPoint(c1);
			// painter->drawPoint(c2);

			// Bezier curve(p1, c1, c2, p2);

			Parenthesis parenthesis(box, Parenthesis::Closing, 8., 1./4);

			painter->setDrawColor({ 0x00, 0xff, 0x00 });
			parenthesis.draw(*painter);

			// painter->drawCurve(Bezier::fromBoundingBox(box, 1./4));

			// painter->setDrawColor({ 0xff, 0x00, 0x00 });
			// painter->drawCurve(Bezier::fromBoundingBox(box, 1./8));

			// painter->setDrawColor({ 0x00, 0x00, 0xff });
			// painter->drawCurve(Bezier::fromBoundingBox(box, 1./4));

			// painter->setDrawColor({ 0xff, 0x00, 0x00 });
			// painter->drawPoint(points[0]);
			// painter->drawPoint(points[1]);
			// painter->drawPoint(points[2]);
			// painter->drawPoint(points[3]);

			// painter->setDrawColor({ 0x00, 0xff, 0x00 });
			// painter->drawRect(curve.boudingBox());




			painter->present();
		} while (cont);

		return 0;
	} catch (const Error &err) {
		std::cerr << err.what() << std::endl;
	}
	return 1;
}
