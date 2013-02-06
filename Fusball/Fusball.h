#pragma once
#include "WorldDrawer2d.h"
#include <vector>

#define MAX_RIGHT 14
#define MAX_UP 19
#define NET_COORD 6
#define SPEED 0.2f
#define PLAYER_SPEED 0.5f
#define NR_TRIANGLES 100
#define NR_PLAYERS 10

using namespace std;

/* ========================================================================== */

class Ball : WorldDrawer2d {
  public:
	Ball (vector<Point2d> points, string color);
	~Ball();
	void addOnField();
	void translate (float sx, float sy);
	void rotate (Point2d ref, float angleInRadians);
	void remove();
	Point2d getCenter();
	float getRay();
	float getDist(Point2d p1, Point2d p2);
  
  public:
	vector<Point2d> points;
	vector<int> topology;
	CoordinateSystem2d *cs;
	Object2d **ball;
	float ray;
};

/* ========================================================================== */

class Field : WorldDrawer2d {
  public:
	Field();
	~Field();
	void markField();
  private:
	void drawBoarders();
	void drawNet();
	void setBoarderCoord (vector<Point2d>& points);
	void setNetCoord (vector<Point2d>& points);
	void setTopology4Boarders (vector<int>& topology);
	void setTopology4Net (vector<int>& topology);
	void drawField();
 
  public:
	CoordinateSystem2d *cs;
};

/* ========================================================================== */

class Player : public Ball {
  public:
	Player (vector<Point2d> points, string color);
	~Player();
	void pass (Ball *ball);
	void set_parameters (Ball *ball);
	void reset_parameters();

	float scale;
	bool right_side, left_side;
	int multiply_x, multiply_y;
	bool forbidden[256];
};

/* ========================================================================== */

class Game : WorldDrawer2d {
  public:	
	Game (vector<Point2d> ball_points, vector<Point2d> player_points);
	~Game();
	void rotateBall (string direction);
	void passBall (bool& animation);
	void reset();
	void modifyScore (string color);
	void restart();
	bool is_finished();
	void movePlayer (string direction);
	bool player_in_field();
	bool ball_in_field();
	bool ball_position(); 
	void attack();
	void reset_score();	
	void restore_ball (string player);
	void init (vector<Point2d> ball_points, vector<Point2d> player_points);

private:
	CoordinateSystem2d *table;
	vector<CoordinateSystem2d *> backup;
  	Field *field;
  	Ball *ball;
  	Player **players;
  	int has_ball, red_score, blue_score;
	vector<Point2d> red_table, blue_table;
};

/* ========================================================================== */
