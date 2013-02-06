#include "Support3d.h"
#include "Transform3d.h"

//-------------------------------------------------------------------------------------
//	Point3D
//-------------------------------------------------------------------------------------
Point3d::Point3d(){
	x=y=z=0;
}
Point3d::Point3d(float x, float y,float z){
	this->x=x;
	this->y=y;
	this->z=z;
}
Point3d::~Point3d(){}

//set
void Point3d::set(float x, float y, float z){
	this->x=x;
	this->y=y;
	this->z=z;
}

Point3d Point3d::operator/ (int div) {
	return Point3d(this->x/div, this->y/div, this->z/div); 
}

//translate
void Point3d::translate(float tx, float ty, float tz){
	Transform3d::translate(this,tx,ty,tz);
}
//rotate relative to origin of coord system
void Point3d::rotateXRelativeToOrigin(float angleRad){
	Transform3d::rotateX(this,angleRad);
}
void Point3d::rotateYRelativeToOrigin(float angleRad){
	Transform3d::rotateY(this,angleRad);
}
void Point3d::rotateZRelativeToOrigin(float angleRad){
	Transform3d::rotateZ(this,angleRad);
}
//rotate relative to another point in the same coord system
void Point3d::rotateXRelativeToPoint(Point3d p, float angleRad){
	Transform3d::rotateXRelativeToAnotherPoint(this,&p,angleRad);
}
void Point3d::rotateYRelativeToPoint(Point3d p, float angleRad){
	Transform3d::rotateYRelativeToAnotherPoint(this,&p,angleRad);
}
void Point3d::rotateZRelativeToPoint(Point3d p, float angleRad){
	Transform3d::rotateZRelativeToAnotherPoint(this,&p,angleRad);
}
void Point3d::scale(float sx, float sy, float sz){
	Transform3d::scale(this,sx,sy,sz);
}
void Point3d::scaleRelativeToPoint(Point3d p, float sx, float sy, float sz){
	Transform3d::scaleRelativeToAnotherPoint(this,&p,sx,sy,sz);
}




//-------------------------------------------------------------------------------------
//	Object3D
//-------------------------------------------------------------------------------------
//constructor & destructor
Object3d::Object3d(std::vector<Point3d> points, std::vector<int> topology){
	this->points = points;
	this->topology = topology;
	//color
	colorx=colory=colorz=1;
	draw_axis=false;
	//compute axis
	axiscenter.set(0,0,0);
	axisup.set(0,2,0);
	axisright.set(2,0,0);
	axisfwd.set(0,0,2);
	computeAxis();
}
Object3d::~Object3d(){
}		
//get center
void Object3d::getCenter(float *x, float *y, float *z){
	float centerx=0;
	float centery=0;
	float centerz=0;
	if(topology.size()>0 && points.size()>0){
		for(unsigned int i=0;i<topology.size();i++){
			centerx+=points[topology[i]].x;
			centery+=points[topology[i]].y;
			centerz+=points[topology[i]].z;
		}
		centerx/=(float)topology.size();
		centery/=(float)topology.size();
		centerz/=(float)topology.size();
	}else{
		centerx=0;
		centery=0;
		centerz=0;
	}
	(*x)=centerx;
	(*y)=centery;
	(*z)=centerz;
}
//recompute axis
void Object3d::computeAxis(){
	float centerx, centery, centerz;
	getCenter(&centerx, &centery, &centerz);
	float oldux=axisup.x-axiscenter.x;
	float olduy=axisup.y-axiscenter.y;
	float olduz=axisup.z-axiscenter.z;
	float oldrx=axisright.x-axiscenter.x;
	float oldry=axisright.y-axiscenter.y;
	float oldrz=axisright.z-axiscenter.z;
	float oldfx=axisfwd.x-axiscenter.x;
	float oldfy=axisfwd.y-axiscenter.y;
	float oldfz=axisfwd.z-axiscenter.z;
	axiscenter.set(centerx,centery,centerz);
	axisup.set(centerx+oldux,centery+olduy,centerz+olduz);
	axisright.set(centerx+oldrx,centery+oldry,centerz+oldrz);
	axisfwd.set(centerx+oldfx,centery+oldfy,centerz+oldfz);
}

//translate
void Object3d::translate(float x, float y, float z){
	//translate each point
	for(unsigned int i=0;i<points.size();i++){
		points[i].translate(x,y,z);
	}
	//adjust axis
	axiscenter.translate(x,y,z);
	axisup.translate(x,y,z);
	axisright.translate(x,y,z);
	axisfwd.translate(x,y,z);
}

//rotate relative to center
void Object3d::rotateXSelf(float angleRad){
	//rotate each point
	for(unsigned int i=0;i<points.size();i++){
		points[i].rotateXRelativeToPoint(axiscenter,angleRad);
	}
	//adjust axis
	axisup.rotateXRelativeToPoint(axiscenter,angleRad);
	axisright.rotateXRelativeToPoint(axiscenter, angleRad);
	axisfwd.rotateXRelativeToPoint(axiscenter,angleRad);
}
void Object3d::rotateYSelf(float angleRad){
	//rotate each point
	for(unsigned int i=0;i<points.size();i++){
		points[i].rotateYRelativeToPoint(axiscenter,angleRad);
	}
	//adjust axis
	axisup.rotateYRelativeToPoint(axiscenter,angleRad);
	axisright.rotateYRelativeToPoint(axiscenter, angleRad);
	axisfwd.rotateYRelativeToPoint(axiscenter,angleRad);
}
void Object3d::rotateZSelf(float angleRad){
	//rotate each point
	for(unsigned int i=0;i<points.size();i++){
		points[i].rotateZRelativeToPoint(axiscenter,angleRad);
	}
	//adjust axis
	axisup.rotateZRelativeToPoint(axiscenter,angleRad);
	axisright.rotateZRelativeToPoint(axiscenter, angleRad);
	axisfwd.rotateZRelativeToPoint(axiscenter, angleRad);
}

//rotate relative to another point
void Object3d::rotateXRelativeToPoint(Point3d pct, float angleRad){
	//rotate each pt
	for(unsigned int i=0;i<points.size();i++){
		points[i].rotateXRelativeToPoint(pct,angleRad);
	}
	//adjust axis
	axiscenter.rotateXRelativeToPoint(pct,angleRad);
	axisup.rotateXRelativeToPoint(pct,angleRad);
	axisright.rotateXRelativeToPoint(pct, angleRad);
	axisfwd.rotateXRelativeToPoint(pct, angleRad);
}
void Object3d::rotateYRelativeToPoint(Point3d pct, float angleRad){
	//rotate each pt
	for(unsigned int i=0;i<points.size();i++){
		points[i].rotateYRelativeToPoint(pct,angleRad);
	}
	//adjust axis
	axiscenter.rotateYRelativeToPoint(pct,angleRad);
	axisup.rotateYRelativeToPoint(pct,angleRad);
	axisright.rotateYRelativeToPoint(pct, angleRad);
	axisfwd.rotateYRelativeToPoint(pct, angleRad);
}
void Object3d::rotateZRelativeToPoint(Point3d pct, float angleRad){
	//rotate each pt
	for(unsigned int i=0;i<points.size();i++){
		points[i].rotateZRelativeToPoint(pct,angleRad);
	}
	//adjust axis
	axiscenter.rotateZRelativeToPoint(pct,angleRad);
	axisup.rotateZRelativeToPoint(pct,angleRad);
	axisright.rotateZRelativeToPoint(pct, angleRad);
	axisfwd.rotateZRelativeToPoint(pct, angleRad);
}
void Object3d::scale(float sx, float sy, float sz){
	//scale each pt
	for(unsigned int i=0;i<points.size();i++){
		points[i].scale(sx,sy,sz);
	}
	computeAxis();
}
void Object3d::scaleRelativeToPoint(Point3d p, float sx, float sy, float sz){
	//scale each pt
	for(unsigned int i=0;i<points.size();i++){
		points[i].scaleRelativeToPoint(p,sx,sy,sz);
	}
	computeAxis();
}

//color
void Object3d::setcolor(float colorx, float colory, float colorz){
	this->colorx= colorx;
	this->colory= colory;
	this->colorz= colorz;
}




//-------------------------------------------------------------------------------------
//	CoordinateSystem3D
//-------------------------------------------------------------------------------------
//constructor & destructor
CoordinateSystem3d::CoordinateSystem3d(){
	axiscenter.set(0,0,0);
	axisright.set(20,0,0);
	axisup.set(0,20,0);
	axisfwd.set(0,0,20);
	draw_axis=false;
}
CoordinateSystem3d::~CoordinateSystem3d(){
}

//add
void CoordinateSystem3d::objectAdd(Object3d *obj){
	objects.push_back(obj);
}
void CoordinateSystem3d::objectRemove(Object3d *obj){
	if(!obj) return;
	for(unsigned int i=0;i<objects.size();i++){
		if(objects[i]==obj)	objects.erase(objects.begin()+i);
		//multiple ptr friendly
	}
}

void CoordinateSystem3d::computeRelativeAxis(float *rx, float *ry, float *rz, float *ux, float *uy, float *uz, float *fx, float *fy, float *fz){
	float vux,vuy,vuz,vrx,vry,vrz,vfx,vfy,vfz;			//relative axis

	vux=axisup.x-axiscenter.x;
	vuy=axisup.y-axiscenter.y;
	vuz=axisup.z-axiscenter.z;
	float dimu=sqrt(vux*vux+vuy*vuy+vuz*vuz);
	vux/=dimu;vuy/=dimu;vuz/=dimu;

	vrx=axisright.x-axiscenter.x;
	vry=axisright.y-axiscenter.y;
	vrz=axisright.z-axiscenter.z;
	float dimr=sqrt(vrx*vrx+vry*vry+vrz*vrz);
	vrx/=dimr; vry/=dimr; vrz/=dimr;

	vfx=axisfwd.x-axiscenter.x;
	vfy=axisfwd.y-axiscenter.y;
	vfz=axisfwd.z-axiscenter.z;
	float dimf=sqrt(vfx*vfx+vfy*vfy+vfz*vfz);
	vfx/=dimf; vfy/=dimf; vfz/=dimf; 
	(*rx)=vrx;	(*ry)=vry;	(*rz)=vrz;
	(*ux)=vux;	(*uy)=vuy;	(*uz)=vuz;
	(*fx)=vfx;	(*fy)=vfy;	(*fz)=vfz;
}

void CoordinateSystem3d::objectTranslate(Object3d *obj, float tx, float ty,float tz){
	float vux,vuy,vuz,vrx,vry,vrz,vfx,vfy,vfz;			//relative axis
	computeRelativeAxis(&vrx,&vry,&vrz,&vux,&vuy,&vuz,&vfx,&vfy,&vfz);
	
	float newtx=tx*vrx+ty*vux+tz*vfx;
	float newty=tx*vry+ty*vuy+tz*vfy;
	float newtz=tx*vrz+ty*vuz+tz*vfz;
	obj->translate(newtx,newty,newtz);
}
void CoordinateSystem3d::objectRotateXSelf(Object3d *obj, float angleRad){
	obj->rotateXSelf(angleRad);
}
void CoordinateSystem3d::objectRotateYSelf(Object3d *obj, float angleRad){
	obj->rotateYSelf(angleRad);
}
void CoordinateSystem3d::objectRotateZSelf(Object3d *obj, float angleRad){
	obj->rotateZSelf(angleRad);
}
void CoordinateSystem3d::objectScale(Object3d *obj, float sx, float sy, float sz){
	obj->scale(sx,sy,sz);
}
void CoordinateSystem3d::objectScaleRelativeToPoint(Object3d *obj,Point3d *p,float sx, float sy, float sz){
	obj->scaleRelativeToPoint((*p),sx,sy,sz);
}


//translate
void CoordinateSystem3d::translate(float x, float y, float z){
	for(unsigned int i=0;i<objects.size();i++) objects[i]->translate(x,y,z);
	axiscenter.translate(x,y,z);
	axisup.translate(x,y,z);
	axisright.translate(x,y,z);
	axisfwd.translate(x,y,z);
}
//rotate
void CoordinateSystem3d::rotateXSelf(float angleRad){
	for(unsigned int i=0;i<objects.size();i++) objects[i]->rotateXRelativeToPoint(axiscenter,angleRad);
	axisup.rotateXRelativeToPoint(axiscenter, angleRad);
	axisright.rotateXRelativeToPoint(axiscenter, angleRad);
	axisfwd.rotateXRelativeToPoint(axiscenter, angleRad);
}
void CoordinateSystem3d::rotateYSelf(float angleRad){
	for(unsigned int i=0;i<objects.size();i++) objects[i]->rotateYRelativeToPoint(axiscenter,angleRad);
	axisup.rotateYRelativeToPoint(axiscenter, angleRad);
	axisright.rotateYRelativeToPoint(axiscenter, angleRad);
	axisfwd.rotateYRelativeToPoint(axiscenter, angleRad);
}
void CoordinateSystem3d::rotateZSelf(float angleRad){
	for(unsigned int i=0;i<objects.size();i++) objects[i]->rotateZRelativeToPoint(axiscenter,angleRad);
	axisup.rotateZRelativeToPoint(axiscenter, angleRad);
	axisright.rotateZRelativeToPoint(axiscenter, angleRad);
	axisfwd.rotateZRelativeToPoint(axiscenter, angleRad);
}


