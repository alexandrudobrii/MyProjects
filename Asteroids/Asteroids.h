#pragma once
#include <vector>
#include "GL/freeglut.h"
#include "Camera.h"

#define BLACK 0.0, 0.0, 0.0
#define WHITE 1.0, 1.0, 1.0
#define BLUE 0.0, 0.0, 1.0
#define BROWN 0.4, 0.2, 0.0
#define GREY 0.6, 0.6, 0.6
#define YELLOW 0.8, 1.0, 0.0
#define GREEN 0.4, 1.0, 0.0
#define RED 1.0, 0.0, 0.0

#define PI 3.14159265359f
#define BROWN_ID 0
#define RED_ID 1

class Asteroid : public Camera {
  public:
	Asteroid();
	~Asteroid();

	void draw (int color);
	void rotate (float speed);
	void set_speed (float speed);
	void recreate();
	float get_speed();

  private:
  	float spin;
  	float speed;
};