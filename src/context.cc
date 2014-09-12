#include "context.h"

#include "error.h"
#include "painter.h"
#include "window.h"

#include <SDL.h>

using namespace nealrame;

Context::Context()
{
	if (SDL_Init(SDL_INIT_EVENTS|SDL_INIT_VIDEO)) {
		throw  Error(SDL_GetError());
	}
}

Context & Context::instance()
{
	static std::unique_ptr<Context> instance_;
	if (instance_) {
		instance_.reset(new Context);
	}
	return *instance_;
}

Context::~Context()
{
	SDL_Quit();
}

std::shared_ptr<Window>
Context::createWindow(const std::string &title, uint16_t width, uint16_t height)
{
	return std::make_shared<Window>(title, width, height);
}

std::shared_ptr<Painter>
Context::createPainter(std::shared_ptr<Window> &window_ptr)
{
	return std::make_shared<Painter>(window_ptr);
}
