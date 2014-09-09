#include <algorithm>
#include <array>
#include <cmath>
#include <exception>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>

#include <boost/format.hpp>
#include <boost/math/special_functions/binomial.hpp>

#include <SDL.h>

#define SQUARE(X) ((X)*(X))

using real = float;

struct Color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
};

struct Point {
	std::string name;
	float x;
	float y;
	std::string toString() const
	{
		return (boost::format("%1%(%2%,%3%)") % name % x % y).str();
	}
};

struct Rect {
	Point topLeft;
	Point bottomRight;
	float width() const
	{
		return fabs(bottomRight.x - topLeft.x);
	}
	float height() const
	{
		return fabs(bottomRight.y - topLeft.y);
	}
	std::string toString() const
	{
		return (boost::format("[%1%, %2%, %3%, %4%]") 
					% topLeft.x % topLeft.y % width() % height()).str();
	}
};



template <unsigned int N>
struct Polynomial {
	typedef typename std::array<real, N + 1>::size_type size_type;
	typedef Polynomial<N - 1> Derived;

	real factors[N + 1];

	Polynomial()
	{
		memset(factors, 0, (N + 1)*sizeof(real));
	}

	Polynomial(const real factors[N + 1])
	{
		memcpy(this->factors, factors, (N + 1)*sizeof(real));
	}

	/// Retourne le degre du polynome
	unsigned int degree() const
	{
		for (int i = N; i > 0; --i) {
			if (fabs(factors[i]) > std::numeric_limits<real>::epsilon()) {
				return i;
			}
		}
		return 0;
	}

	/// Evalue le polynome pour une valeur donnee
	real operator()(real x) const
	{
		real v = 0;
		unsigned int i = 0;

		do {
			v = v*x + factors[N - i];
		} while (++i < (N + 1));

		return v;
	}

	/// Retourne le coefficient du degre specifie
	real & operator[](size_type i)
	{
		return factors[i];
	}

	/// Retourne le coefficient du degre specifie
	const real & operator[](size_type i) const
	{
		return (*const_cast<Polynomial<N> *>(this))[i];
	}

	/// Retourne le polynome derive
	Derived derived() const
	{
		real derived_factors[N];
		for (int i = 1; i <= N; ++i) {
			derived_factors[i - 1] = i*factors[i];
		}
		return Derived(derived_factors);
	}
};

class Bezier {
	typedef Polynomial<3> Polynomial;

	Polynomial x,  y;
	Polynomial::Derived dx, dy;

	/// Calcule les extremums locaux sur l'interval [0, 1] du polynome p de
	/// derivee d.
	static inline void
	extremum(real &min, real &max,
	const Polynomial::Derived &d, const Polynomial &p)
	{
		/// On cherche les solution de l'equation d(x) = 0.
		/// On ne considere que les valeurs appartenant a l'intervalle [0, 1].
		switch (d.degree())
		{
		case 2: {
			auto discriminant = SQUARE(d[1]) - 4*d[2]*d[0];
			if (discriminant >= 0) {
				auto sqrt_of_discriminant = std::sqrt(discriminant);

				auto r1 = (-d[1] - sqrt_of_discriminant)/(2*d[2]);
				if (r1 >= 0 && r1 <= 1) {
					auto v = p(r1);
					min = std::min(v, min);
					max = std::max(v, max);
				}

				auto r2 = (-d[1] + sqrt_of_discriminant)/(2*d[2]);
				if (r2 >= 0 && r2 <= 1) {
					auto v = p(r2);
					min = std::min(v, min);
					max = std::max(v, max);
				}
			}
		} break;

		case 1: {
			auto r = -(d[0]/d[1]);
			if (r >= 0 && r <= 1) {
				auto v = p(r);
				min = std::min(v, min);
				max = std::max(v, max);
			}
		} break;

		default: break;
		}
	}

public:
	/// Une courbe de bezier est une fonction parametrique définie sur [0,1]
	/// comme telle:
	///     [0, 1] -> ℝ×ℝ
	/// Bezier(t) = (x(t), y(t))
	///     x(t) = P0.x*(1-t)³ + 3*P1.x*(1-t)²t + 3*P2.x(1-t)t² + 3*P3.x*t³
	///     y(t) = P0.y*(1-t)³ + 3*P1.y*(1-t)²t + 3*P2.y(1-t)t² + 3*P3.y*t³
	///
	/// Nous utiliserons la forme developpee des polynomes.
	///
	/// Pour plus d'infos consulter:
	///   http://pomax.github.io/bezierinfo
	///   http://floris.briolas.nl/floris/2009/10/bounding-box-of-cubic-bezier
	Bezier(const Point &p0, const Point &p1, const Point &p2, const Point &p3)
	{
		// Initialize les coefficients du polynome pour la composante x
		x[0] = 1*p0.x;
		x[1] = 3*p1.x - 3*p0.x;
		x[2] = 3*p2.x - 6*p1.x + 3*p0.x;
		x[3] = 1*p3.x - 3*p2.x + 3*p1.x - p0.x;

		// Initialize les coefficients du polynome pour la composante y
		y[0] = 1*p0.y;
		y[1] = 3*p1.y - 3*p0.y;
		y[2] = 3*p2.y - 6*p1.y + 3*p0.y;
		y[3] = 1*p3.y - 3*p2.y + 3*p1.y - p0.y;

		dx = x.derived();
		dy = y.derived();
	}

	/// Evalue la courbe pour la valeur donnee
	Point operator()(real t) const
	{
		return {"", x(t), y(t)};
	}

	/// Calcul et retourne la bouding box de la courbe.
	Rect boudingBox() const
	{
		auto min_x = std::min(x(0), x(1));
		auto max_x = std::max(x(0), x(1));
		auto min_y = std::min(y(0), y(1));
		auto max_y = std::max(y(0), y(1));

		extremum(min_x, max_x, dx, x);
		extremum(min_y, max_y, dy, y);

		return Rect{{"", min_x, min_y}, {"", max_x, max_y}};
	}
	};

template<typename T>
Rect boundingBox(const T &);

Rect boundingBox(const Rect &r)
{
	return r;
}

class SDLError : public std::exception {
	std::string msg_;
public:
	SDLError() noexcept : msg_(SDL_GetError())
	{ }
	SDLError(const std::string &msg) noexcept : 
		msg_((boost::format("%1%: %2%") % msg % SDL_GetError()).str())
	{ }
	virtual ~SDLError()
	{ }
	virtual const char * what() const noexcept
	{
		return msg_.c_str();
	}
};

class Renderer;
class Window;

class SDL {
	SDL()
	{
		if (SDL_Init(SDL_INIT_EVENTS|SDL_INIT_VIDEO)) {
			throw  SDLError("Error while initializing SDL");
		}
	}

	SDL(const SDL &) = delete;
	SDL(SDL &&) =  delete;

	SDL & operator=(const SDL&);
	SDL & operator=(SDL &&);

public:
	static SDL & instance()
	{
		static std::unique_ptr<SDL> instance_;
		if (instance_) {
			instance_.reset(new SDL);
		}
		return *instance_;
	}
public:
	virtual ~SDL()
	{
		SDL_Quit();
	}

	std::shared_ptr<Window>
	createWindow(const std::string &title, uint16_t width, uint16_t height)
	{
		return std::make_shared<Window>(title, width, height);
	}

	std::shared_ptr<Renderer>
	createRenderer(std::shared_ptr<Window> &window_ptr)
	{
		return std::make_shared<Renderer>(window_ptr);
	}
};

class Window {
	std::string title_;
	std::unique_ptr<SDL_Window, std::function<void(SDL_Window *)>> window_;

	Window(const Window &) = delete;
	Window & operator=(const Window &) = delete;

public:
	Window(const std::string &title, uint16_t width, uint16_t height) :
		title_(title),
		window_(SDL_CreateWindow(
				title_.c_str(), 
				0, 0, width, height, 
				SDL_WINDOWEVENT_SHOWN&~SDL_WINDOW_FULLSCREEN
			), SDL_DestroyWindow)
	{
		if (! window_) {
			throw SDLError("SDL_CreateWindow Error");
		}
	}

	Window(Window &&rhs)
	{
		*this = std::move(rhs);
	}

	Window & operator=(Window &&rhs)
	{
		title_ = std::move(rhs.title_);
		window_ = std::move(rhs.window_);
		return *this;
	}

	SDL_Window * get()
	{
		return window_.get();
	}
};

class Renderer {
	std::shared_ptr<Window> window_;
	std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer *)>> renderer_;

	Renderer(const Renderer &) = delete;
	Renderer & operator=(const Renderer &) = delete;

public:
	Renderer(std::shared_ptr<Window> &win) :
		window_(win),
		renderer_(SDL_CreateRenderer(
				win->get(), -1,
				SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC
		), SDL_DestroyRenderer)
	{
		if (! renderer_) {
			throw SDLError("SDL_CreateRenderer Error");
		}
	}

	Renderer(Renderer &&rhs)
	{
		*this = std::move(rhs);
	}

	Renderer & operator=(Renderer &&rhs)
	{
		renderer_ = std::move(rhs.renderer_);
		return *this;
	}

public:
	bool clear()
	{
		return SDL_RenderClear(renderer_.get()) >= 0;
	}

	bool setDrawColor(Color c)
	{
		return SDL_SetRenderDrawColor(
			renderer_.get(), c.red, c.green, c.blue, c.alpha
		) >= 0;
	}

	bool drawPoint(Point p1) {
		return drawRect({{"", p1.x - 2, p1.y - 2}, {"", p1.x + 2, p1.y + 2}});
	}

	bool drawLine(Point p1, Point p2) {
		return SDL_RenderDrawLine(
			renderer_.get(), p1.x, p1.y, p2.x, p2.y
		) >= 0;
	}

	bool drawCurve(Bezier c)
	{
		float t = 0;
		do {
			if (! drawLine(c(t), c(t + 0.05))) {
				return false;
			}
		} while ((t += 0.05) <= 1);
		return true;
	}

	// bool drawCurve(Point p1, Point c1, Point c2, Point p2)
	// {
	// 	return drawCurve<4>(Bezier<4>({{p1, c1, c2, p2}}));
	// }

	bool drawRect(Rect r)
	{
		SDL_Rect rect = { 
			int16_t(r.topLeft.x),
			int16_t(r.topLeft.y),
			int16_t(r.width()),
			int16_t(r.height())
		};
		return SDL_RenderDrawRect(renderer_.get(), &rect);
	}

	void present()
	{
		SDL_RenderPresent(renderer_.get());
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
		auto window = SDL::instance().createWindow("Hello World!", 640, 480);
		auto renderer = SDL::instance().createRenderer(window);

		bool cont = true;

		std::array<Point, 4> points = {{ {"p1", 10, 10}, {"c1", 110, 110}, {"c2", 210, 210}, {"p2", 310, 310} }};
		Point *selected = nullptr;

		do {
			SDL_Event ev;
			while (SDL_PollEvent(&ev)) {
				switch (ev.type) {
					case SDL_QUIT:
					case SDL_KEYDOWN:
						cont = false;
						break;

					case SDL_MOUSEBUTTONDOWN:
						selected =
							select<4>(points, {"", float(ev.button.x), float(ev.button.y)});
						break;

					case SDL_MOUSEBUTTONUP:
						selected = nullptr;
						break;

					case SDL_MOUSEMOTION:
						if (selected != nullptr) {
							selected->x = int16_t(ev.motion.x);
							selected->y = int16_t(ev.motion.y);
						}
						break;

					default:
						break;
				}

				renderer->setDrawColor({0});
				renderer->clear();

				renderer->setDrawColor({ 0xff, 0xff, 0xff });

				Bezier curve(points[0], points[1], points[2], points[3]);

				renderer->drawCurve(curve);
				renderer->setDrawColor({ 0xff, 0x00, 0x00 });
				renderer->drawPoint(points[0]);
				renderer->drawPoint(points[1]);
				renderer->drawPoint(points[2]);
				renderer->drawPoint(points[3]);

				renderer->setDrawColor({ 0x00, 0xff, 0x00 });
				renderer->drawRect(curve.boudingBox());

				renderer->present();
			}
		} while (cont);

		return 0;
	} catch (const SDLError &err) {
		std::cerr << err.what() << std::endl;
	}
	return 1;
}
