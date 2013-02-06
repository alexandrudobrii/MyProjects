#include "Asteroids.h"

/* constructorul clasei Asteroid */
Asteroid::Asteroid() : spin(0.0), speed(0.0) { }

/* destructorul clasei Asteroid */
Asteroid::~Asteroid() { }

/* metoda care deseneaza un asteroid avand culoarea specificata */
void Asteroid::draw (int color) {
	glPushMatrix();

	if (color == BROWN_ID) {
		glColor4f(BROWN, 0.5);
	} 

	if (color == RED_ID) {
		glColor4f(RED, 0.5);
	}
	
	glTranslatef(position.x, position.y, position.z);
	glRotatef(spin, 0, 1, 0);
	glutSolidDodecahedron();
	glPopMatrix();
}

/* metoda care roteste asteroidul */
void Asteroid::rotate (float speed) {
	speed = speed/PI*180;
	spin += speed;
}

/* metoada ce returneaza viteza ateroidului */
float Asteroid::get_speed() {
	return speed;
}

/* metoda care seteaza viteza asteroidului */
void Asteroid::set_speed (float speed) {
	this->speed = speed;
}

/* metoda ce initializeaza un asteroid intr-o pozitie random  */
void Asteroid::recreate() {
	float x = -35;
	float y = rand()%20-10;
	float z = rand()%11+35;

	init(x,y,z);
}
