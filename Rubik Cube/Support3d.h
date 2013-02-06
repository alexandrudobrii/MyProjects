#pragma once
#include <cmath>
#include <iostream>
#include <vector>

//forward declaration
class Transform3d;

//attach points to objects...
class Point3d{
	public:
		Point3d();
		Point3d(float tx, float ty, float tz);
		~Point3d();

		Point3d operator/ (int div);
		void set(float x, float y, float z);					//set
		void translate(float tx, float ty, float tz);			//translatie
		void rotateXRelativeToOrigin(float angleRad);			//rotatie relativa la originea sist de coord global
		void rotateYRelativeToOrigin(float angleRad);			
		void rotateZRelativeToOrigin(float angleRad);			
		void rotateXRelativeToPoint(Point3d p, float angleRad);	//rotatie relativ la punctul p
		void rotateYRelativeToPoint(Point3d p, float angleRad);	
		void rotateZRelativeToPoint(Point3d p, float angleRad);	
		void scale(float sx,float sy,float sz);
		void scaleRelativeToPoint(Point3d p, float sx,float sy, float sz);
	public:
		float x,y,z;											// (x,y)
};

//object2d
class Object3d{
	public:
		Object3d(std::vector<Point3d> points, std::vector<int> topology);
		~Object3d();

		void translate(float tx, float ty, float tz);			//translatie
		void rotateXSelf(float angleRad);						//rotatie a obiectului relativ la centrul sau
		void rotateYSelf(float angleRad);						
		void rotateZSelf(float angleRad);						
		void rotateXRelativeToPoint(Point3d pct, float angleRad);	//rotatie relativa la punctul p
		void rotateYRelativeToPoint(Point3d pct, float angleRad);
		void rotateZRelativeToPoint(Point3d pct, float angleRad);
		void scale(float sx,float sy,float sz);
		void scaleRelativeToPoint(Point3d p, float sx,float sy, float sz);
		void setcolor(float colorx, float colory, float colorz);//set culoare
	public:
		bool draw_axis;
		Point3d axiscenter, axisup, axisright, axisfwd;			//axe obiect
		std::vector<Point3d> points;							//punctele obiectului
		std::vector<int> topology;								//topologia punctelor obiectului
		float colorx, colory, colorz;
		int color;
		int position;	
private:
		void getCenter(float *x, float *y, float *z);//functii helper
		void computeAxis();	
};

//sistem de coordonate
class CoordinateSystem3d{
	public:
		CoordinateSystem3d();
		~CoordinateSystem3d();

		//functii de adaugare si scoatere din lista de obiecte din sistemul de coordonate
		//un obj poate fi in mai multe sist de coordonate dar amandoua vor efectua transformarile pe el.
		void objectAdd(Object3d *obj);
		void objectRemove(Object3d *obj);

		//translatie rotatie relativa la THIS sistem de coordonate. Obj nu trebuie sa fie adaugat la THIS.
		void objectTranslate(Object3d *obj, float tx, float ty, float tz);
		void objectRotateXSelf(Object3d* obj, float angleRad);
		void objectRotateYSelf(Object3d* obj, float angleRad);
		void objectRotateZSelf(Object3d* obj, float angleRad);
		//no exista rotatii relative la axe ale sist de coordonate (dificultate prea mare pt lab)
		void objectScale(Object3d *obj,float sx,float sy,float sz);
		void objectScaleRelativeToPoint(Object3d *obj,Point3d *p, float sx,float sy, float sz);

		//translatie rotatie a sistemului de coord
		void translate(float tx, float ty, float tz);
		void rotateXSelf(float angleRad);
		void rotateYSelf(float angleRad);
		void rotateZSelf(float angleRad);
		
	public:
		bool draw_axis;
		Point3d axiscenter,axisright, axisup, axisfwd;	//axe
		std::vector<Object3d*> objects;					//obiecte
	private:
		//helper func
		void computeRelativeAxis(float *rx, float *ry, float *rz, float *ux, float *uy, float *uz, float *fx, float *fy, float *fz);
};
