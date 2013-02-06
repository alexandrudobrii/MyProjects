#include "labyrinth.h"

/* destructorul clasei Labyrinth */
Labyrinth::~Labyrinth() { 
	delete player;
}

/* constructorul clasei Labyrinth */
Labyrinth::Labyrinth (const char *filename) : moved(false) { 
	ifstream in(filename);
	char x;
	
	/* se memoreaza configuratia labirintului */
	in >> n >> m >> posx >> posz;
	in >> exitx >> exitz;
	mat.resize(n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			in >> x;
			mat[i].push_back(x);
		}
	}
	in.close();
  	
  	/* se initialieaza jucatorul */
  	player = new Player();
  	player->init(posx*DIM, 0, posz*DIM, 0);
  	
  	/* se initializeaza cele trei camere */
  	camera.resize(NR_CAM);
  	camera[TD].init(TD_POS);
  	camera[FPS].init(posx*DIM, 0, posz*DIM, 0);
  	camera[TPS].init(posx*DIM, HEIGHT, posz*DIM+STEP, 0);
}

/* metoda care deseneaza solul labirintului */
void Labyrinth::draw_ground() {
	for (int i = -1; i <= n; i++) {
		for (int j = -1; j <= m; j++) {
			glPushMatrix();
			glColor3f(GREY);
			glTranslatef(DIM*i, -DIM, DIM*j);
			glutSolidCube(DIM);
			glPopMatrix();
		}
	}
}

/* metoda care deseneaza peretii labirintului */
void Labyrinth::draw_walls() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (mat[i][j] == '#') {
				glPushMatrix();
				glColor3f(BROWN);
				glTranslatef(DIM*j, 0, DIM*i);
				glutSolidCube(DIM);
				glPopMatrix();
			}
		}
	}
}

/* metoda care deseneaza portalul labirintului */
void Labyrinth::draw_exit() {
	glPushMatrix();
	glColor3f(GREEN);
	glTranslatef(DIM*exitx, 0, DIM*exitz);
	glutSolidTorus(DIM/8.0f, DIM/4.0f, 100, 100);
	glPopMatrix();
}

/* metoda care schimba pozitia camerei in functie de selectia facuta */
void Labyrinth::switch_camera (int id) {
	switch (id) {
		case TD:
			camera[TD].render(camera[TD].view(), 0);
			break;

		case FPS:
			camera[FPS].render(camera[FPS].view(),0);
			break;

		case TPS:
			if (moved == false) {
				camera[TPS].render(player->get_position(),0);
			} else {
				camera[TPS].render(player->get_position(),1);
			}
			break;
	}
}

/* metoda ce returneaza distanta dinte doua puncte */
float Labyrinth::get_distance (Vector3D v1, Vector3D v2) {
	return sqrt(pow(v1.x-v2.x, 2) + pow(v1.z-v2.z, 2));
}

/* metoda ce testeaza daca jocul s-a terminat sau nu */
bool Labyrinth::game_is_over() {
	Vector3D player_pos = player->get_position();
	Vector3D exit_pos = Vector3D(DIM*exitx,0,DIM*exitz);
	
	return get_distance(player_pos, exit_pos) < 3;
}

void Labyrinth::reset_moved() {
	moved = false;
}

/* metoda care testeaza coliziunile dintre
 * peretii labirintului si jucator */
bool Labyrinth::is_colision (char key) {
	Vector3D position;

	/* se simuleaza mutarea jucatoului intr-o anumita directie */
	switch (key) {
		case 'w':
			position = player->simulate(0.5,FORWARD)+player->get_forward();
			break;
		case 's':
			position = player->simulate(-0.5,BACK)-player->get_forward();
			break;
		case 'e':
			position = player->simulate(0.5,RIGHT)+player->get_right();
			break;
		case 'q':
			position = player->simulate(-0.5,LEFT)-player->get_right();
			break;
		default:
			return false;
			break;
	}

	/* se verifica daca exista vreo coliziune intre pereti si jucator */
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (mat[i][j] == '#') {
				if (get_distance(position, Vector3D(j*DIM,0,i*DIM)) < RAY) {
					return true;
				}
			}
		}
	}

	return false;
}

/* metoda care muta jucatorul, respectiv 
 * camerele in functie de tasta apasata */
void Labyrinth::perform_action (char& key, int camera) {
	
	if (game_is_over()) {
		exit(0);
	}
	 
	if (is_colision(key)) {
		if (camera != FPS) {
			player->add();
		}
		draw_exit();
		key = NONE;
		return;
	}

	move_FPS(key);
	move_TPS(key);
	move_TD(key);
	move_player(key, camera);
	draw_exit();
	key = NONE;
}

/* metoda ce muta jucatorul in functie de tasta apasata */
void Labyrinth::move_player (char key, int camera) {
	switch (key) {
		case 'w':
			player->translate_Forward(0.5, 1);
			break;
		case 'a':
			player->rotate(0.1);
			break;
		case 's':
			player->translate_Forward(-0.5, 1);
			break;
		case 'd':
			player->rotate(-0.1);
			break;
		case 'q':
			player->translate_Right(-0.5, 1);
			break;
		case 'e':
			player->translate_Right(0.5, 1);
			break;
	}

	if (camera != FPS) {
		player->add();
	}
}

/* metoda ce translateaza camera FPS */
void Labyrinth::move_FPS (char key) {
	switch (key) {
		case 'w':
			camera[FPS].translate_Forward(0.5, 1);
			break;
		case 'a':
			camera[FPS].rotateFPS_OY(-0.1, 1);
			break;
		case 's':
			camera[FPS].translate_Forward(-0.5, 1);
			break;
		case 'd':
			camera[FPS].rotateFPS_OY(0.1, 1);
			break;
		case 'q':
			camera[FPS].translate_Right(-0.5, 1);
			break;
		case 'e':
			camera[FPS].translate_Right(0.5, 1);
			break;
	}	
}

/* metoda ce translateaza camera TPS */
void Labyrinth::move_TPS (char key) {
	switch (key) {

		/* se realizeaza rotirea camerei in functie de jucator */
		case 'w':
			moved = false;
			camera[TPS].save_position();
			camera[TPS].translate_Forward(0.5, 1);
			break;
		case 'a':
			moved = false;
			camera[TPS].save_position();
			camera[TPS].rotateTPS_OY(-0.1, DISTANCE, 1);
			break;
		case 's':
			moved = false;
			camera[TPS].save_position();
			camera[TPS].translate_Forward(-0.5, 1);
			break;
		case 'd':
			moved = false;
			camera[TPS].save_position();
			camera[TPS].rotateTPS_OY(0.1, DISTANCE, 1);
			break;
		case 'q':
			moved = false;
			camera[TPS].save_position();
			camera[TPS].translate_Right(-0.5, 1);
			break;
		case 'e':
			moved = false;
			camera[TPS].save_position();
			camera[TPS].translate_Right(0.5, 1);
			break;

		/* se realizeaza rotirea camerei fata de cele 3 directii */
		case 'j':
			moved = true;
			camera[TPS].rotateTPS_OX(-0.1, DISTANCE, 0);
			break;
		case 'k':
			moved = true;
			camera[TPS].rotateTPS_OX(0.1, DISTANCE, 0);
			break;
		case 'u':
			moved = true;
			camera[TPS].rotateTPS_OY(-0.1, DISTANCE, 0);
			break;
		case 'i':
			moved = true;
			camera[TPS].rotateTPS_OY(0.1, DISTANCE, 0);
			break;
		case 'm':
			moved = true;
			camera[TPS].rotateTPS_OZ(-0.1, DISTANCE, 0);
			break;
		case 'n':
			moved = true;
			camera[TPS].rotateTPS_OZ(0.1, DISTANCE, 0);	
			break;
	}
}

/* metoda ce realizeaza zoom asupra labirintului */
void Labyrinth::move_TD (char key) {
	switch(key) {
		case 'z':
			camera[TD].translate_Forward(0.005, 1);
			break;
		case 'x':
			camera[TD].translate_Forward(-0.005, 1);
			break;
		case 'c':
			camera[TD].translate_Right(0.5, 1);
			break;
		case 'v':
			camera[TD].translate_Right(-0.5, 1);
			break;
	}
}




