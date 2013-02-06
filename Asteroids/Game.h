#pragma once
#include <GL/freeglut.h>
#include <vector>
#include <stdarg.h>
#include "Camera.h"
#include "Spacecraft.h"
#include "Asteroids.h"

#define PI 3.14159265359f
#define DYN_POS 0, 0, 75
#define DELAY 2
#define DYN_SPEED 0.3
#define NONE 0

#define DYNAMIC 0
#define SPACECRAFT 1
#define ASTEROID 2

#define EASY 1
#define NORMAL 2
#define VERY_HARD 3

class Game : public Spacecraft {
 
 public:
  Game();
  ~Game(); 
  
  void switch_camera (int id);
  void perform_action (unsigned char& key, int camera);
  void print(GLfloat x, GLfloat y, GLfloat z, const char *format,...);
  void choose_level();
  void start();
  void end (int position);
  bool is_over();

  void gen_asteroids();
  void draw_asteroids();
  void draw_spacecraft();
  void destroy_asteroid();
  void open_fire();
 
  void move_DYNAMIC (unsigned char& key);	
  void move_SPACECRAFT (unsigned char& key);
  void move_ASTEROID();
  
  void set_object (int object);
  void set_selected (int selected);
  void set_level (int level);
  void set_level_proprities();
  void reset_selected();
  void reset_object(); 
 
 private:
	std::vector<Camera> camera;
	std::vector<Asteroid> asteroids;
	Spacecraft *spacecraft;

	int iteration;
	int step;
  int object;
  int selected;
  int level;
  int score;
};