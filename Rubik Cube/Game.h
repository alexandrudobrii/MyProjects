#include "Rubik.h"

/* identificatori pentru parcurgera straturilor selectate */
#define BT 0
#define LR 1
#define BF 2

class Game {
  public:
  	Game();
  	~Game();
  	void move90_layer (int *direction, float speed);
  	void move90_all (int *directon, float speed);
	int get_selected();
	void reset_selected();
	int perform_action (int layer);
  	void start();
 	void draw_score();
	bool is_finished();
	void animate (int layer, int sign);
	void increase_score();
	void reset_score();
	void restart();
	void modify_selected (int select);
	void animate_score();
	
  private:
	Rubik *rubik;
  	int selected[3];
	int iteration;
  	int random; 
  	int initial_moves;
  	int score;
	Cube score_board;
};