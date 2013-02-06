#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <GL/freeglut.h>
#include "Vector3D.h"
#include "Camera.h"
#include "Asteroids.h"

#define NONE 0
#define SPEED 1.0f
#define FORWARD 0
#define BACK 0
#define LEFT 1
#define RIGHT 1
#define UP 2
#define DOWN 2
#define SCALE 4.0f
#define RADIUS 2.2f

typedef struct Vertex {
  float x, y, z;
} Vertex;

typedef struct Face {
  Face(void) : nverts(0), verts(0) { };
  int nverts;
  Vertex **verts;
  float normal[3];
} Face;

typedef struct Mesh {
  Mesh(void) : nverts(0), verts(0), nfaces(0), faces(0) { };
  int nverts;
  Vertex *verts;
  int nfaces;
  Face *faces;
} Mesh;

class Spacecraft : public Camera {

 public: 
  Spacecraft();
  Spacecraft (const char *filename);
  ~Spacecraft();

  void draw();
  void enable_blending();
  void disable_blending();
  void move (unsigned char& key);
  void set_transparency (float transparency);
  void decrease_transparency (float transparency);
  
  bool is_destroyed();
  bool is_colision (char key);
  bool is_impact (Asteroid asteroid);
  bool out_of_boarders (Vector3D position);
  Vector3D simulate (float dist, int id);

 private:
  Mesh *mesh;
  float transparency;
  bool blending;
};

Mesh *ReadOffFile(const char *filename);