#pragma once

#include "common.h"
#include "size.h"

#include <functional>
#include <SDL.h>

namespace nealrame
{
class Window {
	PIMPL;
	Window(const Window &) = delete;
	Window & operator=(const Window &) = delete;

public:
	using Event = SDL_EventType;
	using EventData = SDL_Event;

public:
	Window(const std::string &title, uint16_t width, uint16_t height);
	Window(Window &&);
	virtual ~Window();
	
	Window & operator=(Window &&);

	std::string title() const;
	Size size() const;

public:
	void on(Event, std::function<void(const EventData &)>);
	void pollEvent();

public:
	void * get();
};
}