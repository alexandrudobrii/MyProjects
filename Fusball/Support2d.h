#pragma once
#include <cmath>
#include <iostream>
#include <vector>

//forward declaration (Point2d are nevoie de Transform2d si Transform2d are nevoie de Point2d, dar Transform 2d
//					   este pus in alt fisier pentru convenienta de implementare a labului, deci se foloseste
//					   mecanismul de forward declaration)
class Transform2d;

//-----------------------------------------------------------------------------------------------------
//	Point2d
//-----------------------------------------------------------------------------------------------------
class Point2d{
	public:
		Point2d();
		Point2d(float x, float y);
		~Point2d();

		void set(float x, float y);								//set
		void translate(float x, float y);						//translatie
		void rotateRelativeToOrigin(float angleRad);			//rotatie relativa la originea sist de coord global
		void rotateRelativeToPoint(Point2d p, float angleRad);	//rotatie relative la punctul p
		void scale(float sx, float sy);							//scalare
		void scaleRelativeToPoint(Point2d p, float sx, float sy);//scalare relativa la punctul p
	public:
		float x,y;												// (x,y)
};


//-----------------------------------------------------------------------------------------------------
//	Object2d
//-----------------------------------------------------------------------------------------------------
class Object2d{
	public:
		Object2d();
		Object2d(std::vector<Point2d> points, std::vector<int> topology);
		~Object2d();

		std::vector<Point2d> rotateSelf(float angleRad);						//rotatie a obiectului relativ la centrul sau
		void translate(float x, float y);						//translatie
		void rotateRelativeToPoint(Point2d pct, float angleRad);//rotatie relativa la punctul p
		void scale(float sx, float sy);							//scalare
		void scaleRelativeToPoint(Point2d p, float sx, float sy);//scalare relativa la punctul p
		void setcolor(float colorx, float colory, float colorz);//set culoare
	public:
		bool drawaxis;
		Point2d axiscenter, axisup, axisright;					//axe obiect
		std::vector<Point2d> points;							//punctele obiectului
		std::vector<int> topology;								//topologia punctelor obiectului
		float colorx, colory, colorz;							//culoare
		void getCenter(float *x, float *y);	//functii helper
		void computeAxis();					//
};


//-----------------------------------------------------------------------------------------------------
//	CoordinateSystem2d
//	in afara de translate/rotate/scale aplicabile asupra sa contine si functii de tipul
//	objectTranslate()  care translateaza un obiect relativ la axele sistemului de coordonate
//-----------------------------------------------------------------------------------------------------
class CoordinateSystem2d{
	public:
		CoordinateSystem2d();
		~CoordinateSystem2d();

		//functii de adaugare si scoatere din lista de obiecte din sistemul de coordonate
		//un obj poate fi in mai multe sist de coordonate dar amandoua vor efectua transformarile pe el.
		void objectAdd(Object2d *obj);
		void objectRemove(Object2d *obj);

		//translatie rotatie relativa la acest sistem de coordonate. 
		//obiectul transformat nu trebuie sa fie adaugat la acest sistem de coordonate pentru a apela functia.
		void objectTranslate(Object2d *obj, float tx, float ty);
		void objectRotateSelf(Object2d* obj, float angleRad);
		void objectRotateRelativeToPoint(Object2d *obj, Point2d *pct, float angleRad);
		void objectScale(Object2d *obj, float sx, float sy);
		void objectScaleRelativeToPoint(Object2d *obj, Point2d *pct, float sx, float sy);

		//translatie rotatie a sistemului de coord
		void translate(float x, float y);
		void rotateSelf(float angleRad);
		void rotateRelativeToCoordinateSystem(CoordinateSystem2d cd, float angleRad);
	public:
		bool draw_axis;
		Point2d axiscenter,axisright, axisup;	//axe
		std::vector<Object2d*> objects;			//obiecte
};
