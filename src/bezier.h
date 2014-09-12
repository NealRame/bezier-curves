#pragma once

#include "common.h"
#include "point.h"
#include "polynomial.h"
#include "rect.h"

namespace nealrame
{
class Bezier {
public:
	typedef Polynomial<3> Polynomial;

public:
	static Bezier fromBoundingBox(const Rect &, real ratio = 1/8);

public:
	Bezier()
	{ }

	Bezier(const Point &, const Point &, const Point &, const Point &);

	/// Evalue la courbe pour la valeur donnee
	Point operator()(real) const;

	/// Calcul et retourne la bouding box de la courbe.
	Rect boudingBox() const;

	Point p1() const;
	Point p2() const;
	Point ctrl1() const;
	Point ctrl2() const;

private:
	Polynomial x,  y;
	Polynomial::Derived dx, dy;
	Point p1_, p2_, c1_, c2_; 
};	
}