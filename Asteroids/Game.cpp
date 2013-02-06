#include "Game.h"

/* constructorul clasei Game */
Game::Game() : iteration(0), step(30), object(-1),
			   selected(-1), level(-1), score(0) {
	spacecraft = new Spacecraft("spacecraft.off");
	spacecraft->init(0,0,40);
	spacecraft->set_transparency(0.13f);

	camera.resize(3);
	camera[DYNAMIC].init(DYN_POS);
	camera[SPACECRAFT].init(0,0,40);
	camera[ASTEROID].init(0,0,0);
}

/* destructorul clasei Game */
Game::~Game() { delete spacecraft; }

/* metoda care muta camera in functie de pozitia specificata */
void Game::switch_camera (int position) {
	camera[position].render(position);
}

void Game::draw_spacecraft() {
	spacecraft->draw();
}

/* metoda care seteaza dificultatea jocului */
void Game::set_level_proprities() {
	if (level == EASY && iteration == 3000) {
		iteration = 1;
		step -= 10;	

		if (step <= 0) {
			step = 1;
		}
	}

	if (level == NORMAL && iteration == 1000) {
		iteration = 1;
		step -= 5; 

		if (step <= 0) {
			step = 1;
		}
	}
}

/* metoda ce adauga noi asteroizi in vector pentru a fi desenati */
void Game::gen_asteroids() {
	Asteroid asteroid;
	iteration++;

	set_level_proprities();

	if (iteration % step == 0) {
		float x = -35;
		float y = rand()%20-10;
		float z = rand()%11+35;
		
		asteroid.init(x,y,z);
		asteroid.set_speed((rand()%3 + 1)/10.0);
		asteroids.push_back(asteroid);
	}
}

/* metoda ce seteaza asteroidul a fi  */
void Game::set_selected (int selected) {
	this->selected = selected;
}


/* metoda ce seteaza asteroidul a fi distrus */
void Game::set_object (int object) { 
	this->object = object; 
}

/* metoda ce reseteaza asteroidul a fi distrus */
void Game::reset_object() { 
	object = -1; 
}

void Game::reset_selected() {
	this->selected = -1;
}

/* metoda ce deseneaza asteroizii si id-ul acestora */
void Game::draw_asteroids() {
	for (int i = 0; i < asteroids.size(); i++) {
		float speed = asteroids[i].get_speed();
		asteroids[i].translate_Right(-speed);
		asteroids[i].rotate(0.01);

		glPushName(i);
		asteroids[i].draw(BROWN_ID);
		glPopName();
		
		if (asteroids[i].get_position().x > 35) {
			asteroids[i].recreate();
		}

		if (spacecraft->is_impact(asteroids[i])) {
			asteroids.erase(asteroids.begin()+i);
			spacecraft->decrease_transparency(0.04f);
		}
	}
}

void Game::destroy_asteroid() {
	asteroids[object].draw(RED_ID);
	asteroids.erase(asteroids.begin()+object);
	score++;
}

bool Game::is_over() {
	return spacecraft->is_destroyed();
}

/* metoda care deseneaza o linie de la nava la asteroidul distrus */
void Game::open_fire() {
	
	if (object == -1) { return; }

	Vector3D ship = spacecraft->get_position();
	Vector3D asteroid = asteroids[object].get_position();

	glLineWidth(10);
	glBegin(GL_LINES);
		glColor3f(0,1,1);
		glVertex3f(ship.x, ship.y, ship.z); 
		glVertex3f(asteroid.x, asteroid.y, asteroid.z);
	glEnd();

	destroy_asteroid();
	object = -1;
}

/* metoda ce realizeaza toate actiunile pe care jocul le are */
void Game::perform_action (unsigned char& key, int camera) {
	
	if (spacecraft->is_colision(key)) {
   		return;
   	}	

	move_SPACECRAFT(key);
	move_ASTEROID();
	spacecraft->move(key);

	if (camera == DYNAMIC) {
		move_DYNAMIC(key);
		spacecraft->enable_blending();
	} else if (camera == SPACECRAFT) {
		spacecraft->disable_blending();
	} else {
		spacecraft->enable_blending();
	}
	
	key = NONE;
}

/* metoda ce muta camera DYNAMIC */
void Game::move_DYNAMIC (unsigned char& key) {
	switch(key) {
		case 'z':
			camera[DYNAMIC].translate_Forward(DYN_SPEED);
			break;
		
		case 'x':
			camera[DYNAMIC].translate_Forward(-DYN_SPEED);
			break;
		
		case 'c':
			camera[DYNAMIC].translate_Right(DYN_SPEED);
			break;
		
		case 'v':
			camera[DYNAMIC].translate_Right(-DYN_SPEED);
			break;
		
		case 'b':
			camera[DYNAMIC].translate_Up(DYN_SPEED);
			break;
		
		case 'n':
			camera[DYNAMIC].translate_Up(-DYN_SPEED);
			break;
	}
}

/* metoda ce muta camera NAVA */
void Game::move_SPACECRAFT (unsigned char& key) {
	switch(key) {
		case 'q':
			camera[SPACECRAFT].translate_Forward(DYN_SPEED);
			break;
		
		case 'e':
			camera[SPACECRAFT].translate_Forward(-DYN_SPEED);
			break;
		
		case 'a':
			camera[SPACECRAFT].translate_Right(DYN_SPEED);
			break;
		
		case 'd':
			camera[SPACECRAFT].translate_Right(-DYN_SPEED);
			break;
		
		case 'w':
			camera[SPACECRAFT].translate_Up(DYN_SPEED);
			break;
		
		case 's':
			camera[SPACECRAFT].translate_Up(-DYN_SPEED);
			break;
	}
}

/* metoda ce pozitioneaza camera pe asteroidul selectat */
void Game::move_ASTEROID() {
	if (selected == -1) { return; }

	Vector3D center = spacecraft->get_position();
	Vector3D position = asteroids[selected].get_position();

	camera[ASTEROID].render(position, center);

	if (position.x > 30) {
		selected = -1;
	}
} 

/* metoda de afisare text pe ecran */
void Game::print(GLfloat x, GLfloat y, GLfloat z, const char *format,...) {
  va_list args;

  char buffer[1024],*p;

  va_start(args,format);

  vsprintf(buffer, format, args);

  va_end(args);

  glPushMatrix();
  
  glTranslatef(x,y,z);

  glScalef(0.0035, 0.0035, 0.0); /* 0.1 to 0.001 as required */

  for (p = buffer; *p; p++)
    glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p);

  glPopMatrix();
}

/* metoda de creeer a meniului pentru alegerea nivelului de joc */
void Game::choose_level() {
	
	if (level != -1) { return; }

	//switch_camera(DYNAMIC);
	glColor3f(1.0,1.0,0.0);
    glLineWidth(2.0);
	print(-2,0.5,70, "SELECT LEVEL");
	print(-2,-0.5,60, "EASY");
	print(-2,-1.5,60, "NORMAL");
	print(-2,-2.5,60, "VERY HARD");

	glEnable(GL_BLEND);
    glAlphaFunc(GL_EQUAL, 1);
    glDepthMask(GL_FALSE);

    glColor4f(1.0f, 0.0f, 1.0f, 0.1f);
 	glPushName(EASY);
    glRectf(-10.75f,0.75f, 6.75f, -4.0f);
    glPopName();

  	glColor4f(0.0f, 1.0f, 1.0f, 0.1f);
  	glPushName(NORMAL);
    glRectf(-10.75f,-8.75f, 6.75f, -4.0f);
    glPopName();

    glColor4f(1.0f, 0.2f, 0.0f, 0.1f);
  	glPushName(VERY_HARD);
    glRectf(-10.75f,-13.5f, 6.75f, -8.75f);
    glPopName();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glAlphaFunc(GL_LESS, 1);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

/* metoda care initializeaza jocul */
void Game::start() {
	
	if (iteration) { return; } 

	glColor3f(1.0,1.0,0.0);
    glLineWidth(2.0);

    if (level == EASY) {
    	print(-2.8,0,70, "PIECE OF CAKE!!!");
    	step = 60;
    }

   	if (level == NORMAL) {
    	print(-2,0,70, "GOOD LUCK!!!");
    	step = 30;
    }

    if (level == VERY_HARD) {
    	print(-3.2,0,70, "YOU ARE SCREWED!!!");
    	step = 1;
   	}
}

/* metoda care printeaza pe ecran mesajul de sfarsit */
void Game::end (int position) {
	
	if (position != DYNAMIC) {
		switch_camera(DYNAMIC);
	}
	
	glColor3f(1.0,0.0,0.0);
    glLineWidth(2.0);
	print(-2,0.4f,70, "GAME OVER!!!");
	
	char buffer[25];
	sprintf(buffer, "YOU SHOT %d ASTEROIDS", score);
	glColor3f(0.0,1.0,0.0);
	print(-3.5,-0.5f,60, buffer);
}

/* metoda care seteaza dificultatea jocului */
void Game::set_level (int level) {
	this->level = level;
}