#include "Camera.h"

Camera::Camera() { }
Camera::~Camera() { }

void Camera::init (float x, float y, float z) {
	position = Vector3D(x,y,z);
	forward = Vector3D(0,0,-1);
	up = Vector3D(0,1,0);
	right = Vector3D(-1,0,0);
}

void Camera::translate_Forward (float dist) {
	position = position + forward*dist;
}

void Camera::translate_Up (float dist) {
	position = position + up*dist;
}

void Camera::translate_Right (float dist) {
	position = position + right*dist;
}

void Camera::rotateFPS_OY (float angle) {
	forward = forward*cos(angle) + right*sin(angle);
	forward.Normalize();
	right = forward.CrossProduct(up);
	
}

void Camera::rotateFPS_OX (float angle) {
	up = up*cos(angle) + forward*sin(angle);
	up.Normalize();
	forward = up.CrossProduct(right);	
}

void Camera::rotateFPS_OZ (float angle) {
	right = right*cos(angle) + up*sin(angle);
	right.Normalize();
	up = right.CrossProduct(forward);
}

void Camera::rotateTPS_OY (float angle, float dist_to_interes) {
	translate_Forward(dist_to_interes);
	rotateFPS_OY(angle);
	translate_Forward(-dist_to_interes);
}

void Camera::rotateTPS_OX (float angle, float dist_to_interes) {
	translate_Forward(dist_to_interes);
	rotateFPS_OX(angle);
	translate_Forward(-dist_to_interes);
}

void Camera::rotateTPS_OZ (float angle, float dist_to_interes) {
	translate_Forward(dist_to_interes);
	rotateFPS_OZ(angle);
	translate_Forward(-dist_to_interes);
}

void Camera::render (int id) {
	Vector3D center;
	
	if (id == DYNAMIC) {
		center = position + forward;	
	}

	if (id == SPACECRAFT) {
		center = position + right;
	}

	gluLookAt(position.x, position.y, position.z, 
			  center.x, center.y, center.z,
			  up.x, up.y, up.z);
}

void Camera::render (Vector3D position, Vector3D center) {
	gluLookAt(position.x, position.y, position.z, 
			  center.x, center.y, center.z,
			  up.x, up.y, up.z);
}

void Camera::render (Vector3D center) {
	gluLookAt(position.x, position.y, position.z, 
			  center.x, center.y, center.z,
			  up.x, up.y, up.z);
}

Vector3D Camera::get_forward() {
	return forward;
}

Vector3D Camera::get_right() {
	return right;
}

Vector3D Camera::get_up() {
	return up;
}

Vector3D Camera::get_position() {
	return position;
}
