#pragma once
#include "Vector3D.h"

using namespace std;

class Camera {
	public:
		Camera();
		~Camera();

		void translate_Forward(float dist, int);
		void translate_Up(float dist, int);
		void translate_Right(float dist, int);

		void rotateFPS_OY(float angle, int);
		void rotateFPS_OX(float angle, int);
		void rotateFPS_OZ(float angle, int);
		void rotateTPS_OY(float angle, float dist_to_interes, int);
		void rotateTPS_OX(float angle, float dist_to_interes, int);
		void rotateTPS_OZ(float angle, float dist_to_interes, int);

		void init(int, int, int, int);
		void render (Vector3D center, int id);
		void save_position();
		Vector3D view();
		Vector3D get_position();
		Vector3D get_forward();
		Vector3D get_right();
		Vector3D simulate (float dist, int id);

	protected:
		Vector3D forward;
		Vector3D up;
		Vector3D right;
		Vector3D position;

		Vector3D tps_forward;
		Vector3D tps_up;
		Vector3D tps_right;
		Vector3D tps_position;
};