#pragma once
#include <cmath>

//forward declaration (Point2d are nevoie de Transform2d si Transform2d are nevoie de Point2d, dar Transform 2d
//					   este pus in alt fisier pentru convenienta de implementare a labului, deci se foloseste
//					   mecanismul de forward declaration)
class Point2d;

class Transform2d{
	public:
	static void translate(Point2d *pct, float tx, float ty);
	static void rotate(Point2d *pct, float angleInRadians);
	static void rotateRelativeToAnotherPoint(Point2d *pct, Point2d *ref, float angleInRadians);
	static void scale(Point2d *pct, float sx, float sy);
	static void scaleRelativeToAnotherPoint(Point2d *pct, Point2d *ref, float sx, float sy);
};
