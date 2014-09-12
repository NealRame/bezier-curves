#pragma once

#include <memory>

namespace nealrame
{
class Window;
class Painter;
class Context {
	Context();

	Context(const Context &) = delete;
	Context(Context &&) =  delete;
	Context & operator=(const Context&);
	Context & operator=(Context &&);

public:
	static Context & instance();
public:
	virtual ~Context();

	std::shared_ptr<Window>
	createWindow(const std::string &title, uint16_t width, uint16_t height);

	std::shared_ptr<Painter>
	createPainter(std::shared_ptr<Window> &window_ptr);
};
}