#pragma once
#include <cmath>

//forward declaration
class Point3d;

class Transform3d{
	public:
	static void translate(Point3d *pct, float tx, float ty,float tz);
	static void rotateX(Point3d *pct, float angleInRadians);	//rotatie pe axa OX = planul YoZ
	static void rotateY(Point3d *pct, float angleInRadians);	//rotatie pe axa OY = planul XoZ
	static void rotateZ(Point3d *pct, float angleInRadians);	//rotatie pe axa OZ = planul XoY
	static void rotateXRelativeToAnotherPoint(Point3d *pct, Point3d *ref, float angleInRadians);
	static void rotateYRelativeToAnotherPoint(Point3d *pct, Point3d *ref, float angleInRadians);
	static void rotateZRelativeToAnotherPoint(Point3d *pct, Point3d *ref, float angleInRadians);
	static void scale(Point3d *pct, float sx, float sy, float sz);
	static void scaleRelativeToAnotherPoint(Point3d *pct, Point3d *ref, float sx, float sy, float sz);
};
