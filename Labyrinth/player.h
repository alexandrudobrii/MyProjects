#pragma once
#include "camera.h"

#define GREEN 0.4, 1.0, 0.0
#define PI 3.14159265359f

/* clasa Jucator */
class Player : public Camera {
  public:
  	Player();
  	~Player();

    void rotate (float speed);
  	void draw_cone();
  	void draw_sphere(); 
  	void add();
  	
  private:
  	float spin;
};


