#include "camera.h"

Camera::Camera() { }
Camera::~Camera() { }

void Camera::init (int x, int y, int z, int multyply) {
	position = Vector3D(x,y,z);
	forward = Vector3D(0,-y*multyply,-1);
	up = Vector3D(0,1,0);
	right = Vector3D(1,0,0);

	tps_position = position;
	tps_forward = forward;
	tps_up = up;
	tps_right = right;
}

void Camera::translate_Forward (float dist, int id) {
	if (id == 1) {
		position = position + forward*dist;
	}
	tps_position = tps_position + tps_forward*dist;
}

void Camera::translate_Up (float dist, int id) {
	if (id == 1) {
		position = position + up*dist;
	}
	tps_position = tps_position + tps_up*dist;
}

void Camera::translate_Right (float dist, int id) {
	if (id == 1) {
		position = position + right*dist;
	}
	tps_position = tps_position + tps_right*dist;
}

void Camera::rotateFPS_OY (float angle, int id) {
	tps_forward = tps_forward*cos(angle) + tps_right*sin(angle);
	tps_forward.Normalize();
	tps_right = tps_forward.CrossProduct(tps_up);

	if (id == 1) {
		forward = forward*cos(angle) + right*sin(angle);
		forward.Normalize();
		right = forward.CrossProduct(up);
	}
}

void Camera::rotateFPS_OX (float angle, int id) {
	tps_up = tps_up*cos(angle) + tps_forward*sin(angle);
	tps_up.Normalize();
	tps_forward = tps_up.CrossProduct(tps_right);	
	
	if (id == 1) {
		up = up*cos(angle) + forward*sin(angle);
		up.Normalize();
		forward = up.CrossProduct(right);	
	}
}

void Camera::rotateFPS_OZ (float angle, int id) {
	tps_right = tps_right*cos(angle) + tps_up*sin(angle);
	tps_right.Normalize();
	tps_up = tps_right.CrossProduct(tps_forward);

	if (id == 1) {
		right = right*cos(angle) + up*sin(angle);
		right.Normalize();
		up = right.CrossProduct(forward);
	}
}

void Camera::rotateTPS_OY (float angle, float dist_to_interes, int id) {
	translate_Forward(dist_to_interes, id);
	rotateFPS_OY(angle, id);
	translate_Forward(-dist_to_interes, id);
}

void Camera::rotateTPS_OX (float angle, float dist_to_interes, int id) {
	translate_Forward(dist_to_interes, id);
	rotateFPS_OX(angle, id);
	translate_Forward(-dist_to_interes, id);
}

void Camera::rotateTPS_OZ (float angle, float dist_to_interes, int id) {
	translate_Forward(dist_to_interes, id);
	rotateFPS_OZ(angle, id);
	translate_Forward(-dist_to_interes, id);
}

void Camera::render (Vector3D center, int id) {
	if (id == 0) {
		gluLookAt(position.x, position.y, position.z, 
			  	  center.x, center.y, center.z,
			  	  up.x, up.y, up.z);
	}

	if (id == 1) {
		gluLookAt(tps_position.x, tps_position.y, tps_position.z, 
			  	  center.x, center.y, center.z,
			  	  tps_up.x, tps_up.y, tps_up.z);
	}
}

void Camera::save_position() {
	tps_position = position;
	tps_forward = forward;
	tps_up = up;
	tps_right = right;
}

Vector3D Camera::simulate (float dist, int id) {
	if (id == 0) {
		return position + forward*dist;
	}

	if (id == 1) {
		return position + right*dist;
	}
}	

Vector3D Camera::view() {
	return position + forward;
} 

Vector3D Camera::get_forward() {
	return forward;
}

Vector3D Camera::get_right() {
	return right;
}

Vector3D Camera::get_position() {
	return position;
}

