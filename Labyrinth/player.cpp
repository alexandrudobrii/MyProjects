#include "player.h"

/* constructorul clasei Player */
Player::Player() : spin(0.0f) { }

/* destructorul clasei Player */
Player::~Player() { }

/* metoda ce adauga un con in poztita specificata */
void Player::draw_cone() {
	glPushMatrix();	
	glColor3f(GREEN);
	glTranslatef(position.x, position.y, position.z);
	glRotatef(180,0,1,0);
	glRotatef(spin,0,1,0);
	glutSolidCone(1,2.5,100,100);
	glPopMatrix();
}

/* metoda ce adauga o sfera in poztita specificata */
void Player::draw_sphere() {
	glPushMatrix();
	glColor3f(GREEN);
	glTranslatef(position.x, position.y, position.z);
	glutSolidSphere(1,100,100);
	glPopMatrix();
}

/* metoda ce adauga jucatorul in labirint */
void Player::add() {
	draw_sphere();
	draw_cone();
}

/* metoda ce roteste jucatorul */
void Player::rotate (float speed) {
	rotateFPS_OY(-speed, 1);

	speed = speed/PI*180;
	spin += speed;
}

