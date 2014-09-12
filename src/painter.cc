#include "bezier.h"
#include "color.h"
#include "error.h"
#include "painter.h"
#include "point.h"
#include "rect.h"
#include "window.h"

#include <functional>

#include <SDL.h>

using namespace nealrame;

struct Painter::Impl {
	Impl(std::shared_ptr<Window> window, SDL_Renderer *renderer) :
		window(window),
		renderer(renderer, SDL_DestroyRenderer)
	{ }
	std::shared_ptr<Window> window;
	std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer *)>> renderer;
};

Painter::Painter(std::shared_ptr<Window> window) :
	d_(new Impl(
		window,
		SDL_CreateRenderer(
			static_cast<SDL_Window *>(window->get()), -1,
			SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC
		)
	))
{
	if (! d_->renderer) {
		throw Error(SDL_GetError());
	}
}

Painter::Painter(Painter &&rhs)
{
	*this = std::move(rhs);
}

Painter::~Painter()
{ }

Painter & Painter::operator=(Painter &&rhs)
{
	d_  = std::move(rhs.d_);
	return *this;
}

bool Painter::clear()
{
	return SDL_RenderClear(d_->renderer.get()) >= 0;
}

bool Painter::setDrawColor(const Color &c)
{
	return SDL_SetRenderDrawColor(
		d_->renderer.get(), c.red, c.green, c.blue, c.alpha
	) >= 0;
}

bool Painter::drawPoint(const Point &p1) {
	return drawRect({{p1.x - 2, p1.y - 2}, {p1.x + 2, p1.y + 2}});
}

bool Painter::drawLine(const Point &p1, const Point &p2) {
	return SDL_RenderDrawLine(
		d_->renderer.get(), p1.x, p1.y, p2.x, p2.y
	) >= 0;
}

bool Painter::drawCurve(const Bezier &c)
{
	float t = 0;
	do {
		if (! drawLine(c(t), c(t + 0.05))) {
			return false;
		}
	} while ((t += 0.05) <= 1);

	// drawPoint(c.p1());
	// drawPoint(c.p2());

	// drawPoint(c.ctrl1());
	// drawPoint(c.ctrl2());

	return true;
}

bool Painter::drawRect(const Rect &r)
{
	SDL_Rect rect = { 
		int16_t(r.topLeft().x),
		int16_t(r.topLeft().y),
		int16_t(r.width()),
		int16_t(r.height())
	};
	return SDL_RenderDrawRect(d_->renderer.get(), &rect);
}

void Painter::present()
{
	SDL_RenderPresent(d_->renderer.get());
}
