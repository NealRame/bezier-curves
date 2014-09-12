#include "window.h"

#include "error.h"

#include <algorithm>
#include <array>
#include <functional>
#include <string>
#include <vector>

using namespace nealrame;

using EventHandler = std::function<void(const Window::EventData &)>;
using HandlersMap  = std::array<std::vector<EventHandler>, SDL_LASTEVENT>;

struct Window::Impl {
	Impl(const std::string &title, SDL_Window *window) :
		title(title),
		window(window, SDL_DestroyWindow)
	{
		if (! window) {
			throw Error(SDL_GetError());
		}
	}
	std::string title;
	std::unique_ptr<SDL_Window, std::function<void(SDL_Window *)>> window;
	HandlersMap eventHandlers;
};

Window::Window(const std::string &title, uint16_t width, uint16_t height) :
	d_(new Impl(
		title, 
		SDL_CreateWindow(
			title.c_str(), 
			0, 0, width, height, 
			SDL_WINDOWEVENT_SHOWN&~SDL_WINDOW_FULLSCREEN
	)))
{ }

Window::Window(Window &&rhs)
{
	*this = std::move(rhs);
}

Window::~Window()
{ }

Window & Window::operator=(Window &&rhs)
{
	d_ = std::move(rhs.d_);
	return *this;
}

std::string Window::title() const
{
	return SDL_GetWindowTitle(d_->window.get());
}

Size Window::size() const
{
	int w, h;
	SDL_GetWindowSize(d_->window.get(), &w, &h);
	return Size{static_cast<float>(w), static_cast<float>(h)};
}

void Window::on(Event event, std::function<void(const EventData &)> handler)
{
	d_->eventHandlers[event].push_back(handler);
}

void Window::pollEvent()
{
	SDL_Event ev;

	while (SDL_PollEvent(&ev)) {
		for (auto handler: *(d_->eventHandlers.begin() + ev.type)) {
			handler(ev);
		}
	}
}

void * Window::get()
{
	return d_->window.get();
}
