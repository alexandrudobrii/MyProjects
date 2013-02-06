//-----------------------------------------------------------------------------------------------
//					LAB 2
//
//	Fisiere de interes: Transform3d.cpp main.cpp
//
//	Functii WorldDrawer3d:
//	-init e apelat o singura data, la inceput.
//	-idle e apelat per frame
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


	/* !!! Pentru a putea roti cubul mai intai este nevoie ca
	 * tasta 'SPACE' sa fie apasata pentru ca fetele sa fie 
	 * amestecate,  dupa care se trece in modul rezolvare !!! */


#include "Game.h"

bool WorldDrawer3d::animation = false;
bool game_over = false, first_touch = true, pressed = false;
int rubik_direction = NONE, layer_direction = NONE, once = 0;
float speed;
Game *game;

void WorldDrawer3d::init() {
	/* se creeaza cubul rubik */
	game = new Game();
}

void WorldDrawer3d::onIdle() {	
	
	/* se initializeaza jocul prin rotirea 
	 * aleatoare a diferitelor straturi */
	if (animation) {
		game->start();
		game_over = true;
		once = 0;

	/* permite rotirea in orice directie a cubului rubik */
	} else if (rubik_direction != NONE) {
		game->move90_all(&rubik_direction, RUBIK_SPEED);
	
	/* permite rotirea in ambele directii a fiecarui strat din cubul rubik */
	} else if (layer_direction != NONE) {
		game->move90_layer(&layer_direction, speed);
		
	/* se afiseaza scorul in cazul in care jocul s-a incheiat */
	} else if (game->is_finished() && game_over) {
		if (!once) {
			game->draw_score();
			once = 1;
		}
		game->animate_score();
		first_touch = true;
		 
	/* in timpul mutarii straturilor sau cubului 
	 * nu se mai pot face alte mutari */
	} else {
		pressed = false;
	}
}

void WorldDrawer3d::onKey (unsigned char key) {
	int layer, selected;

	/* daca un strat al cubului sau intreg cubul este in miscare apasarea 
	 *	unei taste nu are niciun efect, doar daca se da restart jocului */
	
	if (pressed && key != 'r') {
		return;
	}

	if (first_touch && key != KEY_SPACE && key != 'r') {
		return;
	}

	/* se memoreaza stratul selectat */
	selected = game->perform_action(game->get_selected());
	
	/* daca se apasa una din tastele 'q', 'w', 'e', 'a', 's', 'd' 
	 * atunci cubul se roteste in directia respectiva */

	/* daca se apasa una din tastele 'z', 'x', 'c', se 
	 * selecteaza stratul dorit pentru a fi rotit */

	/* daca se apasa 'j' sau 'l' atunci stratul selectat 
	 * se va roti spre stanga, respecctiv spre dreapta */

	/* daca se apasa tasta 'r' se va da restart jocului */
	switch (key)
	{
		case 's':
			game->animate(selected, -1);
			game->reset_selected();
			rubik_direction = DOWN;
			pressed = true;
			break;

		case 'd':
			game->animate(selected, -1);
			game->reset_selected();
			rubik_direction = RIGHT;
			pressed = true;
			break;

		case 'w':
			game->animate(selected, -1);
			game->reset_selected();
			rubik_direction = UP;
			pressed = true;
			break;

		case 'a':
			game->animate(selected, -1);
			game->reset_selected();
			rubik_direction = LEFT;
			pressed = true;
			break;

		case 'q':
			game->animate(selected, -1);
			game->reset_selected();
			rubik_direction = BOTTOM;
			pressed = true;
			break;

		case 'e':
			game->animate(selected, -1);
			game->reset_selected();
			rubik_direction = TOP;
			pressed = true;
			break;

		case 'z':
			game->modify_selected(BT);
			pressed = true;
			break;

		case 'x':
			game->modify_selected(LR);
			pressed = true;
			break;

		case 'c':
			game->modify_selected(BF);
			pressed = true;
			break;

		case 'j':
			layer = game->get_selected();
			layer_direction = game->perform_action(layer);
			speed = LAYER_SPEED;
			if (selected != -1)
				game->increase_score();
			pressed = true;
			break;

		case 'l':
			layer = game->get_selected();
			layer_direction = game->perform_action(layer);
			speed = -LAYER_SPEED;
			if (selected != -1)
				game->increase_score();
			pressed = true;
			break;

		case 'r':
			game->restart();
			pressed = true;
			game_over = false;
			break;

		case KEY_SPACE:
			if (selected == -1)
				animation = !animation;
			pressed = true;
			first_touch = false;
			break;

		default:
			break;
	}
}

int main (int argc, char** argv) {
	WorldDrawer3d wd3d(argc,argv,600,600,200,100,std::string("Rubik"));
	
	wd3d.init();
	
	wd3d.run();
	
	return 0;
}
