#include <fstream>
#include <vector>
#include "GL/freeglut.h"
#include "camera.h"
#include "player.h"

#define BLACK 0.0, 0.0, 0.0
#define WHITE 1.0, 1.0, 1.0
#define BLUE 0.0, 0.0, 1.0
#define BROWN 0.4, 0.2, 0.0
#define GREY 0.6, 0.6, 0.6
#define YELLOW 0.8, 1.0, 0.0
#define GREEN 0.4, 1.0, 0.0
#define TD_POS 38,107.5,40,1

#define ESC 27
#define DIM 4
#define RAY 3.5f
#define HEIGHT 10
#define STEP 10
#define DISTANCE (sqrt ((pow((HEIGHT),(2))) + (pow((STEP),(2)))))
#define NR_CAM 4

#define FORWARD 0
#define LEFT 1
#define RIGHT 1
#define BACK 0

#define NONE 0
#define TD 1
#define FPS 2
#define TPS 3

/* clasa Labirint */
class Labyrinth {
  public:	
  	Labyrinth (const char *);
	~Labyrinth();
	
	void draw_walls();
	void draw_ground();
	void draw_exit();
	void switch_camera (int id);
	void move_FPS (char key);
	void move_TPS (char key);
	void move_TD (char key);
	void move_player (char key, int camera);
	void perform_action (char& key, int camera);
	void reset_moved();
	float get_distance(Vector3D v1, Vector3D v2);
	bool is_colision (char key);
	bool game_is_over();

	
  private:
	int n, m, posx, posz, exitx, exitz;
	bool moved;
 	vector< vector<char> > mat;
 	vector<Camera> camera;
 	Player *player;
};