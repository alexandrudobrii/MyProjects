//clasa camera
#pragma once
#include "Vector3D.h"

#define DYNAMIC 0
#define SPACECRAFT 1
#define ASTEROID 2

class Camera{
	public:
		Camera();
		~Camera();

		void translate_Forward(float dist);
		void translate_Up(float dist);
		void translate_Right(float dist);

		void rotateFPS_OY(float angle);
		void rotateFPS_OX(float angle);
		void rotateFPS_OZ(float angle);
		void rotateTPS_OY(float angle, float dist_to_interes);
		void rotateTPS_OX(float angle, float dist_to_interes);
		void rotateTPS_OZ(float angle, float dist_to_interes);

		void init (float x, float y, float z);
		void render (Vector3D position, Vector3D center);
		void render (int id);
		void render (Vector3D center);

		Vector3D get_position();
		Vector3D get_forward();
		Vector3D get_right();
		Vector3D get_up();

	protected:
		Vector3D forward;
		Vector3D up;
		Vector3D right;
		Vector3D position;
};