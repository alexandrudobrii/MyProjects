#include "Game.h"

/* constructorul clasei Game 
 * care initializeaza un joc nou */
Game::Game() {
	rubik = new Rubik();
	for (int i = 0; i < 3; i++) {
		selected[i] = -1;
	}
	iteration = 0;
	random = -1;
	initial_moves = 0;
	score = 0;
}

/* ========================================================================= */

/* metoda care roteste diferite straturi ale cubului ales random 
 * pentru un numar de initial_moves mutari pentru a initializa jocul */
void Game::start() {
	if (initial_moves == 10) {
		iteration = 0;
		initial_moves = 0;
		WorldDrawer3d::animation = false;
		return;   
	} else if (iteration == 0) {
		srand(time(NULL));
		random = rand() % 8;
		initial_moves++;
	}

	move90_layer(&random, LAYER_SPEED);
}

/* metoda care muta un anumit strat al cubului cu 90 de grade */
void Game::move90_layer (int *direction, float speed) {
	if (iteration == MAX_ITERATION) {
		iteration = 0;
		*direction = NONE;
		rubik->rotate_layer(*direction, speed);
		return;
	}

	rubik->rotate_layer(*direction, speed);
	iteration++;
}

/* metoda care roteste cubul pentru 90 de grade in orice directie */
void Game::move90_all (int *direction, float speed) {
	if (iteration == MAX_ITERATION) {
		iteration = 0;
		*direction = NONE;
		rubik->rotate_all(*direction, speed);
		return;
	}
		
	rubik->rotate_all(*direction, speed);
	iteration++;
}

/* ========================================================================= */

/* metoda care deselecteaza toate straturile selectate */
void Game::reset_selected() {
	selected[BT] = selected[BF] = selected[LR] = -1;
}

/* metoda care returnaza stratul selectat */
int Game::get_selected() {
	if (selected[BT] >= 0) {
		return BT;
	}

	if (selected[LR] >= 0) {
		return LR;
	}

	if (selected[BF] >= 0) {
		return BF;
	}

	return -1;
}

/* metoda care modifica stratul selectat in functie de tasta apasata,
 * dupa care se scaleaza stratul respectiv pentru animatie */
void Game::modify_selected (int select) {
	
	if (selected[BT] != -1 || selected[LR] != -1 || selected[BF] != -1) { 
		animate(perform_action(get_selected()), -1);
	}
	
	switch (select) {
		case BT:
			selected[BT]++;
			selected[LR] = -1;
			selected[BF] = -1;

			if (selected[BT] > 2) {
				selected[BT] = 0;
			}
			break;

		case LR:
			selected[LR]++;
			selected[BT] = -1;
			selected[BF] = -1;
			
			if (selected[LR] > 2) {
				selected[LR] = 0;
			}
			break;

		case BF:
			selected[BF]++;
			selected[LR] = -1;
			selected[BT] = -1;
			
			if (selected[BF] > 2) {
				selected[BF] = 0;
			}
			break;
	}

	animate(perform_action(get_selected()), 1);
}

/* functie care intoare stratul ce trebuie mutat */
int Game::perform_action (int layer) {

	if (layer == -1) {
		return -1;
	}

	if (layer == 2 && selected[layer] == 1) {
		return CENTER_Z;
	} else {
		return 3*layer + selected[layer];
	}
}

/* fuctie care mareste respectiv micsoreaza stratul selectat, pentru animatie */
void Game::animate (int layer, int sign) {
	std::vector<int> select;

	if (layer == -1) {
		return;
	}

	select = rubik->get_layer(layer);

	for (int i = 0; i < select.size(); i++) {
		rubik->scale(select[i], sign);	
	}
}

/* ========================================================================= */

/* metoda care testeaza daca jocul s-a terminat sau nu */
bool Game::is_finished() {
	std::vector<int> cube;
	Cube faces;
	int layers[] = {0,2,3,5,6,8};
	int colors[9], count = 0;
	
	for (int i = 0; i < 6; i++) {
		cube = rubik->get_layer(layers[i]);
	
		for (int j = 0; j < 9; j++) {
			colors[j] = 0;
		}

		for (int k = 0; k < cube.size(); k++) {
			faces = rubik->get_components(cube[k]);
				
			for (int l = 0; l < faces.size(); l++) {
				if (faces[l]->color != -1) {
					colors[faces[l]->color]++;
				}
			}
		}

		int j;
		for (j = 0; j < 9; j++) {
			if (colors[j] == 9)
				break;
		}

		if (j != 9) {
			count++;
		}
	}

	if (count == 6)
		return true;
	
	return false;
}

/* metoda care deseneaza scorul din cuburi dupa terminarea jocului */
void Game::draw_score() {
	std::vector<int> topology = rubik->get_topology();
	std::vector<Point3d> points = rubik->get_points();
	CoordinateSystem3d *cs = new CoordinateSystem3d();
	glClearColor(0,1,0,1);
	WorldDrawer3d::cs_used.clear();
	WorldDrawer3d::cs_used.push_back(cs);

	for (int i = 0; i < points.size(); i++) {
		points[i] = points[i]/2;
	}

	int count = 0;
	for (int i = 0; i < score; i++) {
		if (!(i % 4)) { 
			count++;
		}
		
		Object3d *o = new Object3d(points, topology);
		o->setcolor(1,0.1,0);
		cs->objectAdd(o);
		o->translate(5*count, 5*(i % 4), 0);
		o->rotateYRelativeToPoint(cs->axiscenter,-0.8);
		o->translate(-2,0,10);
		score_board.push_back(o);
	}
}

/* metoda care roteste cuburile in jurul axei OY, pentru animatie */
void Game::animate_score() {
	for (int i = 0; i < score_board.size(); i++) {
		score_board[i]->rotateYRelativeToPoint(score_board[i]->axiscenter, 0.004f);
	}
}

/* metoda care incrementeaza cu o unitate numarul de mutari */
void Game::increase_score() {
	score++;
}

/* metoda care resteaza toti parametrii jocului */
void Game::restart() {
	score = 0;
	animate(perform_action(get_selected()), -1);
	selected[BT] = selected[BF] = selected[LR] = -1;
	glClearColor(0.4f,0.5f,1,1);
	WorldDrawer3d::cs_used.clear();
	rubik->add();
}

/* ========================================================================= */