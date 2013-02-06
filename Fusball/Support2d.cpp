#include "Support2d.h"
#include "Transform2d.h"

//-----------------------------------------------------------------------------------------------------
//	Point2d
//-----------------------------------------------------------------------------------------------------
Point2d::Point2d(){
	x=y=0;
}
Point2d::Point2d(float x, float y){
	this->x=x;
	this->y=y;
}
Point2d::~Point2d(){}

//set
void Point2d::set(float x, float y){
	this->x=x;
	this->y=y;
}

//translate
void Point2d::translate(float x, float y){
	Transform2d::translate(this,x,y);
}
//rotate relative to origin of coord system
void Point2d::rotateRelativeToOrigin(float angleRad){
	Transform2d::rotate(this,angleRad);
}
//rotate relative to another point in the same coord system
void Point2d::rotateRelativeToPoint(Point2d p, float angleRad){
	Transform2d::rotateRelativeToAnotherPoint(this,&p,angleRad);
}
void Point2d::scale(float sx, float sy){
	Transform2d::scale(this,sx,sy);
}
void Point2d::scaleRelativeToPoint(Point2d p, float sx, float sy){
	Transform2d::scaleRelativeToAnotherPoint(this, &p, sx, sy);
}

//-----------------------------------------------------------------------------------------------------
//	Object2d
//-----------------------------------------------------------------------------------------------------
//constructor & destructor
Object2d::Object2d(std::vector<Point2d> points, std::vector<int> topology){
	this->points = points;
	this->topology = topology;
	//color
	colorx=colory=colorz=1;
	//drawaxis
	drawaxis=false;
	//set axis
	axiscenter.set(0,0);
	axisup.set(0,1);
	axisright.set(1,0);
	computeAxis();
}

Object2d::Object2d(){
}

Object2d::~Object2d(){
}		
//get center
void Object2d::getCenter(float *x, float *y){
	float centerx=0;
	float centery=0;
	if(topology.size()>0 && points.size()>0){
		for(unsigned int i=0;i<topology.size();i++){
			centerx+=points[topology[i]].x;
			centery+=points[topology[i]].y;
		}
		centerx/=(float)topology.size();
		centery/=(float)topology.size();
	}else{
		centerx=0.0f;
		centery=0.0f;
	}
	(*x)=(float)centerx;
	(*y)=(float)centery;
}
//recompute axis
void Object2d::computeAxis(){
	float centerx, centery;
	getCenter(&centerx, &centery);
	float oldux=axisup.x-axiscenter.x;
	float olduy=axisup.y-axiscenter.y;
	float oldrx=axisright.x-axiscenter.x;
	float oldry=axisright.y-axiscenter.y;
	axiscenter.set(centerx,centery);
	axisup.set(centerx+oldux,centery+olduy);
	axisright.set(centerx+oldrx,centery+oldry);
}

//rotate relative to center
std::vector<Point2d> Object2d::rotateSelf(float angleRad){
	//rotate each point
	for(unsigned int i=0;i<points.size();i++){
		points[i].rotateRelativeToPoint(axiscenter,angleRad);
	}
	//adjust axis
	axisup.rotateRelativeToPoint(axiscenter,angleRad);
	axisright.rotateRelativeToPoint(axiscenter, angleRad);

	return points;
}
//translate
void Object2d::translate(float x, float y){
	//translate each point
	for(unsigned int i=0;i<points.size();i++){
		points[i].translate(x,y);
	}
	//adjust axis
	axiscenter.translate(x,y);
	axisup.translate(x,y);
	axisright.translate(x,y);
}
//rotate relative to another point
void Object2d::rotateRelativeToPoint(Point2d pct, float angleRad){
	//rotate each pt
	for(unsigned int i=0;i<points.size();i++){
		points[i].rotateRelativeToPoint(pct,angleRad);
	}
	//adjust axis
	axiscenter.rotateRelativeToPoint(pct,angleRad);
	axisup.rotateRelativeToPoint(pct,angleRad);
	axisright.rotateRelativeToPoint(pct, angleRad);
}
void Object2d::scale(float sx, float sy){
	//scale each pt
	for(unsigned int i=0;i<points.size();i++){
		points[i].scale(sx,sy);
	}
	computeAxis();
}
void Object2d::scaleRelativeToPoint(Point2d p, float sx, float sy){
	//scale each pt
	for(unsigned int i=0;i<points.size();i++){
		points[i].scaleRelativeToPoint(p,sx,sy);
	}
	computeAxis();
}

//color
void Object2d::setcolor(float colorx, float colory, float colorz){
	this->colorx= colorx;
	this->colory= colory;
	this->colorz= colorz;
}



//-----------------------------------------------------------------------------------------------------
//	CoordinateSystem2d
//-----------------------------------------------------------------------------------------------------
//constructor & destructor
CoordinateSystem2d::CoordinateSystem2d(){
	axiscenter.set(0,0);
	axisright.set(20,0);
	axisup.set(0,20);
	draw_axis=false;
}
CoordinateSystem2d::~CoordinateSystem2d(){
}

//add
void CoordinateSystem2d::objectAdd(Object2d *obj){
	objects.push_back(obj);
}
void CoordinateSystem2d::objectRemove(Object2d *obj){
	if(!obj) return;
	for(unsigned int i=0;i<objects.size();i++){
		if(objects[i]==obj)	objects.erase(objects.begin()+i);
		//multiple ptr friendly
	}
}
void CoordinateSystem2d::objectTranslate(Object2d *obj, float tx, float ty){
	float vux,vuy,vrx,vry;			//relative axis
	vux=axisup.x-axiscenter.x;
	vuy=axisup.y-axiscenter.y;
	float dimu=sqrt(vux*vux+vuy*vuy);
	vrx=axisright.x-axiscenter.x;
	vry=axisright.y-axiscenter.y;
	float dimr=sqrt(vrx*vrx+vry*vry);
	vrx/=dimr;
	vry/=dimr;
	vux/=dimu;
	vuy/=dimu;
	float newtx=tx*vrx+ty*vux;
	float newty=ty*vuy+tx*vry;
	obj->translate(newtx,newty);

}
void CoordinateSystem2d::objectRotateSelf(Object2d *obj, float angleRad){
	obj->rotateSelf(angleRad);
}
void CoordinateSystem2d::objectRotateRelativeToPoint(Object2d *obj, Point2d *pct,float angleRad){
	obj->rotateRelativeToPoint((*pct),angleRad);
}
void CoordinateSystem2d::objectScale(Object2d *obj,float sx,float sy){
	obj->scale(sx,sy);
}
void CoordinateSystem2d::objectScaleRelativeToPoint(Object2d* obj, Point2d *ref, float sx, float sy){
	obj->scaleRelativeToPoint((*ref),sx,sy);
}


//translate
void CoordinateSystem2d::translate(float x, float y){
	for(unsigned int i=0;i<objects.size();i++) objects[i]->translate(x,y);
	axiscenter.translate(x,y);
	axisup.translate(x,y);
	axisright.translate(x,y);
}
//rotate
void CoordinateSystem2d::rotateSelf(float angleRad){
	for(unsigned int i=0;i<objects.size();i++) objects[i]->rotateRelativeToPoint(axiscenter,angleRad);
	axisup.rotateRelativeToPoint(axiscenter, angleRad);
	axisright.rotateRelativeToPoint(axiscenter, angleRad);
}
//rotate
void CoordinateSystem2d::rotateRelativeToCoordinateSystem(CoordinateSystem2d cd, float angleRad){
	for(unsigned int i=0;i<objects.size();i++) objects[i]->rotateRelativeToPoint(cd.axiscenter,angleRad);
	axiscenter.rotateRelativeToPoint(cd.axiscenter,angleRad);
	axisup.rotateRelativeToPoint(cd.axiscenter, angleRad);
	axisright.rotateRelativeToPoint(cd.axiscenter, angleRad);
}

