#include "bezier.h"

using namespace nealrame;

/// Calcule les extremums locaux sur l'interval [0, 1] du polynome p de
/// derivee d.
void
extremum(real &min, real &max, const Bezier::Polynomial::Derived &d, const Bezier::Polynomial &p)
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


Bezier Bezier::fromBoundingBox(const Rect &box, real ratio) {
	auto p1 = box.topRight(), p2 = box.bottomRight();

	auto B = box.middleLeft();
	auto C = box.middleRight();
	auto A = Point{B.x - (C.x - B.x)/3, B.y};

	auto d = std::fabs(p2.y - p1.y)*ratio;

	auto c1 = Point{A.x, p1.y + d};
	auto c2 = Point{A.x, p2.y - d};

	return Bezier(p1, c1, c2, p2);
}


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
Bezier::Bezier(const Point &p0, const Point &p1, const Point &p2, const Point &p3) :
	p1_(p0), p2_(p3), c1_(p1), c2_(p2)
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
Point Bezier::operator()(real t) const
{
	return {x(t), y(t)};
}

/// Calcul et retourne la bouding box de la courbe.
Rect Bezier::boudingBox() const
{
	auto min_x = std::min(x(0), x(1));
	auto max_x = std::max(x(0), x(1));
	auto min_y = std::min(y(0), y(1));
	auto max_y = std::max(y(0), y(1));

	extremum(min_x, max_x, dx, x);
	extremum(min_y, max_y, dy, y);

	return Rect({min_x, min_y}, {max_x, max_y});
}

Point Bezier::p1() const 
{ return p1_; }

Point Bezier::p2() const
{ return p2_; }

Point Bezier::ctrl1() const
{ return c1_; }

Point Bezier::ctrl2() const
{ return c2_; }
