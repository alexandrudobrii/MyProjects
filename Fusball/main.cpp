//-----------------------------------------------------------------------------------------------
//					LAB 1
//
//	Fisiere de interes: Transform2d.cpp main.cpp
//
//	Functii WorldDrawer2d:
//	-init e apelat o singura data, la inceput.
//	-idle e apelat per cadru de desenare
//	-onKey la apasarea unei taste.
//
//	Obiecte:
//	- un obiect este reprezentat prin punct si topologia punctelor (cum sunt legate pctele)
//	- obiectele sunt compuse din triunghiuri! de exemplu cu 4 puncte si 6 indici de topologie
//	pot crea 2 triunghiuri adiacente ce impreuna formeaza un dreptunghi.
//
//	Sisteme de coordonate:
//	- sunt 2 tipuri de sisteme de coordonate (fix - bleu&magenta) si dinamic(rosu&albastru)
//	- ca un obiect sa fie desenat trebuie sa fie atasat unui sistem de coordonate
//	- cand un obiect e atasat la un sistem de coordonate urmeaza transformarile sistemului.
//
//	Control obiecte:
//	- daca translatez/rotesc un obiect/punct direct (ex: o->translate(1,1)) o fac in coordonate globale
//	- daca translatez/rotesc un obiect printr-un sistem de coordonate o fac in coordonate locale
//	- pentru simplitate toate coordonatele mentinute in obiecte(de c++) sunt globale.
//
//	Happy coding.
//----------------------------------------------------------------------------------------------

#include "WorldDrawer2d.h"
#include "Fusball.h"
#include <unistd.h>

bool WorldDrawer2d::animation = false;

Game *game;
bool move_left = false, move_right = false;
vector<Point2d> pointsb, pointsp, points;

void WorldDrawer2d::init() {
	pointsp.push_back(Point2d(-1,1.5));
	pointsp.push_back(Point2d(1,1.5));
	pointsp.push_back(Point2d(0,3));
	
	pointsb.push_back(Point2d(-0.5f,4));
	pointsb.push_back(Point2d(0.5f,4));
	pointsb.push_back(Point2d(0,3.0f));
	
	game = new Game(pointsb, pointsp);
}	

void WorldDrawer2d::onIdle() {	
	
	if (game->is_finished()) {
		game->reset_score();
		sleep(2);
		game->init(pointsb, pointsp);
	}

	if(animation) {
		game->passBall(animation);
	}
}

void WorldDrawer2d::onKey (unsigned char key) {
	
	switch(key)
	{
		case 'a':
			if (!animation) { 
				/* pentru ca bila sa nu iasa afara din teren */
				if (game->ball_in_field() || move_left) {
					game->rotateBall("left");
					if (!game->ball_in_field()) {
						move_left = false;
					}
				} else {
					move_right = true;
				}
			}
			break;
		
		case 'd':
			if (!animation) { 
				/* pentru ca bila sa nu iasa afara din teren */
				if (game->ball_in_field() || move_right) {
					game->rotateBall("right");
					if (!game->ball_in_field()) {
						move_right = false;
					}
				} else {
					move_left = true;
				}
			}
			break;

		case 'i':
			game->movePlayer("UP");
			break;

		case 'j':
			game->movePlayer("LEFT");
			break;

		case 'k':
			game->movePlayer("DOWN");
			break;

		case 'l':
			game->movePlayer("RIGHT");
			break;
		
		case KEY_SPACE:
			game->reset();
			animation = true;
			break;
		
		default:
			break;
	}
}

int main(int argc, char **argv) {
	WorldDrawer2d wd2d(argc,argv,650, 650,200,100,std::string("Fusball"));
	
  	wd2d.init();
	
	wd2d.run();

	return 0;
}
