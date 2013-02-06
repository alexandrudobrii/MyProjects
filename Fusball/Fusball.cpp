#include "Fusball.h"
#include <math.h>
#include <time.h>

/* ========================================================================== */

/* constructor al clasei "Ball" care adauga mingea in teren intr-o pozitie data */
Ball::Ball (vector<Point2d> points, string color) : points(points) {
	cs = new CoordinateSystem2d();
	ball = new Object2d *[NR_TRIANGLES];

	topology.push_back(0);
	topology.push_back(1);
	topology.push_back(2); 
		
	for (int i = 0; i < NR_TRIANGLES; i++) {
		ball[i] = new Object2d(points, topology);
		points = ball[i]->rotateSelf(1);
		if (color == "BLUE") {
			ball[i]->setcolor(0.2,0.2,0.8);
		} else if (color == "RED") {
			ball[i]->setcolor(0.8,0.2,0.2);
		}
		cs->objectAdd(ball[i]);
	}
	
	Point2d p = getCenter();
	ray = getDist(p, points[2]);
}

/* metoda care calculeaza distanta dintre doua puncte */
float Ball::getDist (Point2d p1, Point2d p2) {
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

/* metoda care elimina mingea din teren */
void Ball::remove() {
	cs_used.erase(cs_used.begin());
}

/* metoda care adauga mingea din teren */
void Ball::addOnField() {
	cs_used.push_back(cs);
}

Ball::~Ball() {
	delete cs;
	delete[] ball;
	delete ball;
}

/* metoda care returneaza coordonatele centrului mingii */
Point2d Ball::getCenter() {
	return Point2d(ball[0]->axiscenter.x, ball[0]->axiscenter.y);
}


/* metoda care returneaza raza mingii */
float Ball::getRay() {
	return ray;
}

/* metoda care translateaza mingea */
void Ball::translate (float sx, float sy) {
	for (int i = 0; i < NR_TRIANGLES; i++) {
		ball[i]->translate(sx,sy);
		cs->objectAdd(ball[i]);
	}	
}

/* metoda care roteste mingea in jurul unui punct */
void Ball::rotate (Point2d ref, float angleInRadians) {
	for (int i = 0; i < NR_TRIANGLES; i++) {
		ball[i]->rotateRelativeToPoint(ref, angleInRadians);
		cs->objectAdd(ball[i]);
	}
}

/* ========================================================================== */

/* constructorul clasei player */
Player::Player (vector<Point2d> points, string color) : Ball(points, color) { 
	reset_parameters();
}

Player::~Player() {}

/* metoda care reseteaza parametrii folositi pentru a determina directia mingii */
void Player::reset_parameters() {
	left_side = false;
	right_side = false;

	multiply_x = 1;
	multiply_y = 1;
}

/* metoda care calculeaza viteza si directia mingii in functie de pozitia acesteia pe teren */
void Player::set_parameters (Ball *ball) {
	float x, y, z = 0.0f;

	Point2d player_center = getCenter();
	Point2d ball_center = ball->getCenter();
	
	x = ball_center.x-player_center.x;
	y = ball_center.y-player_center.y;

	if (*(int *)&y != *(int *)&z) {
		scale = x/y;
	}

	if (*(int *)&x < *(int *)&z) {
		if (*(int *)&y == *(int *)&z) {
			multiply_y = 0;
			scale = 1;
		}
		left_side = true;
	} else if (*(int *)&x > *(int *)&z) {
		if (*(int *)&y == *(int *)&z) {
			multiply_y = 0;
			scale = 1;
		}
		right_side = true;
	} else if (*(int *)&x == *(int *)&z) {
		if (*(int *)&y > *(int *)&z) {
			right_side = true;
		} else if (*(int *)&y <= *(int *)&z) {
			left_side = true;
		} 
		multiply_x = 0;
		scale = 1;
	}
}

/* metoda care translateaza mingea in directia corespunzatoare fara ca  aceasta
 * sa iasa din teren */
void Player::pass (Ball *ball) {
	float x;
	int aproximation;
	
	Point2d player_center = getCenter();
	Point2d ball_center = ball->getCenter();
	
	x = ball_center.x-player_center.x;
	
	if (abs(ball_center.x) + ball->getRay() >= MAX_RIGHT) {
		multiply_x *= -1;
	} 
	
	if ((abs(ball_center.y) + ball->getRay() >= MAX_UP) &&
		(abs(ball_center.x) + ball->getRay() >= NET_COORD+1)) {
		multiply_y *= -1;
	}

	if (fabs(SPEED/scale) > 1) {
		aproximation = abs(round(SPEED/scale)*2);
	} else {
		aproximation = 1;
	}

	if ((x > 0 && !left_side) || right_side) {
		ball->translate(SPEED*multiply_x/aproximation, (SPEED/scale)*multiply_y/aproximation);
		return;
	} 

	if (x < 0 || left_side) {
		ball->translate(-SPEED*multiply_x/aproximation, (-SPEED/scale)*multiply_y/aproximation);
	}
}

/* ========================================================================== */

/* constructorul clasei "Game" */
Game::Game (vector<Point2d> ball_points, vector<Point2d> player_points) {
	init(ball_points, player_points);
}

/* metoda care initializeaza un joc nou */
void Game::init(vector<Point2d> ball_points, vector<Point2d> player_points) {
	cs_used.clear();
	
	ball = new Ball(ball_points, "WHITE");
	ball->addOnField();
	ball->translate(0, -1);

	players = new Player *[NR_PLAYERS];
	for (int i = 0; i < NR_PLAYERS/2; i++) {
		players[i] = new Player(player_points, "RED");
		players[i]->addOnField();
	}

	for (int i = NR_PLAYERS/2; i < NR_PLAYERS; i++) {
		players[i] = new Player(player_points, "BLUE");
		players[i]->addOnField();
	}
	
	players[0]->translate(0,15.5f);
	players[1]->translate(0,9);
	players[2]->translate(-9,9);
	players[3]->translate(9,9);
	players[4]->translate(0,2.5f);

	players[5]->translate(0,-18.5f);
	players[6]->translate(0,-12);
	players[7]->translate(-9,-12);
	players[8]->translate(9,-12);
	players[9]->translate(0,-5.5f);

	red_table.push_back(Point2d(2+MAX_RIGHT,1));
	red_table.push_back(Point2d(2+MAX_RIGHT,5));
	red_table.push_back(Point2d(3+MAX_RIGHT,1));
	red_table.push_back(Point2d(3+MAX_RIGHT,5));

	blue_table.push_back(Point2d(2+MAX_RIGHT,-1));
	blue_table.push_back(Point2d(2+MAX_RIGHT,-5));
	blue_table.push_back(Point2d(3+MAX_RIGHT,-1));
	blue_table.push_back(Point2d(3+MAX_RIGHT,-5));
	
	has_ball = 4;
	red_score = 0;
	blue_score = 0;

	field = new Field();
	field->markField();
}

Game::~Game() {
	delete field;
	delete ball;
	delete[] players;
}

/* metoda care roteste mingea in directia corespunzatoare... */
void Game::rotateBall (string direction) {
	if (direction == "left") {
		ball->rotate(players[has_ball]->getCenter(), 0.1f);
	} else if (direction == "right") {
		ball->rotate(players[has_ball]->getCenter(), -0.1f);
	}
}
	
/* metoda care translateaza mingea si determina daca un joc s-a terminat sau nu,
 * respectiv modifica scorul pentru echipa care a dat gol */
void Game::passBall (bool &animation) {
	players[has_ball]->pass(ball);
	
	if (abs(ball->getCenter().y) - ball->getRay() > MAX_UP) {
		animation = false;

		ball->remove();

		if (ball->getCenter().y > 0) {
			modifyScore("BLUE");
			restore_ball("RED");
		}

		if (ball->getCenter().y < 0) {
			modifyScore("RED");
			restore_ball("BLUE");
		}

		if (is_finished()) {
			restart();
		} 

		return;
	}

	for (int i = 0; i < NR_PLAYERS; i++) {
		if (ball->getDist(ball->getCenter(), players[i]->getCenter()) <=
			ball->getRay() + players[i]->getRay()) {
			animation = false;
			has_ball = i;
		}
	}
}

/* metoda care translateaza un jucator si determina coliziunile intre jucatori */
void Game::movePlayer (string direction) {
	float x, y;
	int i;

	if (direction == "RIGHT") {
		x = 0.2;
		y = 0;
	}
	
	if (direction == "LEFT") {
		x = -0.2;
		y = 0;
	}

	if (direction == "UP") {
		x = 0;
		y = 0.2;
	}

	if (direction == "DOWN") {
		x = 0;
		y = -0.2;
	}

	/* coliziuni juacator-jucator */
	if (!players[has_ball]->forbidden[direction[0]]) {
		players[has_ball]->translate(x,y);
		ball->translate(x,y);
	}

	for (i = 0; i < NR_PLAYERS; i++) {
		if (i != has_ball && players[has_ball]->getRay() + players[i]->getRay() >= 
			ball->getDist(players[has_ball]->getCenter(), players[i]->getCenter())) {
				break;
		}
	}

	if (i != NR_PLAYERS || !player_in_field()) {
		players[has_ball]->forbidden[direction[0]] = true;
	} else {
		players[has_ball]->forbidden[direction[0]] = false;
	}

	/* coliziuni jucator-minge */
	for (i = 0; i < NR_PLAYERS; i++) {
		if (i != has_ball && ball->getRay() + players[i]->getRay() >= 
			ball->getDist(ball->getCenter(), players[i]->getCenter())) {
			has_ball = i;
			break;
		}
	}
}

/* metoda care determina pozitia mingii fata de jucator */
bool Game::ball_position() {
	return ball->getCenter().x < players[has_ball]->getCenter().x;
}

/* metoda care determinca daca mingea si jucatorul se afla in teren */
bool Game::player_in_field() {
	return (players[has_ball]->getRay() + abs(players[has_ball]->getCenter().x) < MAX_RIGHT-0.5) && 
		   (players[has_ball]->getRay() + abs(players[has_ball]->getCenter().y) < MAX_UP-0.5) &&
		   (ball->getRay() + abs(ball->getCenter().y) < MAX_UP-0.5) &&
		   (ball->getRay() + abs(ball->getCenter().x) < MAX_RIGHT-0.5);
}

/* metoda care determina daca mingea se afla in teren */
bool Game::ball_in_field() {
	return (ball->getRay() + abs(ball->getCenter().y) < MAX_UP-0.5) &&
		   (ball->getRay() + abs(ball->getCenter().x) < MAX_RIGHT-0.5);
}

/* metoda care reseteaza parametrii necesari calculului directiei mingii */
void Game::reset() {
	players[has_ball]->reset_parameters();
	players[has_ball]->set_parameters(ball);
}

/* metoda care adauga o noua minge in teren dupa ce o echipa a inscris */
void Game::restore_ball (string player) {
	vector<Point2d> ball_points;
	vector<CoordinateSystem2d *>::iterator it;
	float x, y;
	Point2d p, b;

	ball_points.push_back(Point2d(-0.5f,4));
	ball_points.push_back(Point2d(0.5f,4));
	ball_points.push_back(Point2d(0,3.0f));
	
	ball = new Ball(ball_points, "WHITE");
	it = cs_used.begin();
	cs_used.insert(it, ball->cs);

	if (player == "RED") {
		has_ball = rand() % 5;
		
		p = players[has_ball]->getCenter();
		b = ball->getCenter();

		x = p.x - b.x - players[has_ball]->getRay() - ball->getRay() + 1.1;
		y = p.y - b.y - players[has_ball]->getRay() - ball->getRay();
	}
	
	if (player == "BLUE") {
		has_ball = rand() % 5 + 5;
			
		p = players[has_ball]->getCenter();
		b = ball->getCenter();

		x = p.x - b.x - players[has_ball]->getRay() - ball->getRay() + 1.1;
		y = p.y - b.y - players[has_ball]->getRay() - ball->getRay();
	}

	ball->translate(x, y);
}

/* metoda care creeaza un nou joc atunci cand acesta s-au tereminat */
void Game::restart() {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	vector<Point2d> points;
	vector<int> topology;
	CoordinateSystem2d *cs = new CoordinateSystem2d();

	points.push_back(Point2d(-width, -height));
	points.push_back(Point2d(-width, height));
	points.push_back(Point2d(width, -height));
	points.push_back(Point2d(width, height));
	topology.push_back(0);
	topology.push_back(1);
	topology.push_back(2);
	topology.push_back(1);
	topology.push_back(2);
	topology.push_back(3);

	Object2d *obj = new Object2d(points, topology);
	obj->setcolor(0.5, 0.4, 0.3);
	
	cs_used.clear();
	cs->objectAdd(obj);
	cs_used.push_back(cs);
}	

/* metoda care determina daca jocul s-a terminat sau nu */
bool Game::is_finished() {
	return (red_score == 3 || blue_score == 3); 
}

/* metoda care reseteaza scorul jocului */
void Game::reset_score() {
	red_score = 0;
	blue_score = 0;
}

/* metoda care modifica scorul pentru echipa care a marcat */
void Game::modifyScore (string color) {
	Object2d *score;
	vector<int> topology;
	vector<Point2d> points;

	topology.push_back(0);
	topology.push_back(1);
	topology.push_back(2);
	topology.push_back(1);
	topology.push_back(2);
	topology.push_back(3);

	score = new Object2d(points, topology);
	if (color == "RED") {
		points = red_table;
		for (int i = 0; i < red_table.size(); i++) {
			points[i].x += 2*red_score;;		
		}
		score = new Object2d(points, topology);
		score->setcolor(0.8,0.2,0.2);
		red_score += 1;
	} else if (color == "BLUE") {
		points = blue_table;
		for (int i = 0; i < blue_table.size(); i++) {
			points[i].x += 2*blue_score;	
		}
		score = new Object2d(points, topology);
		score->setcolor(0.2,0.2,0.8);
		blue_score += 1;
	}
	
	field->cs->objectAdd(score);
}

/* ========================================================================== */

/* constructorul clasei "Field" */
Field::Field () {
	cs = new CoordinateSystem2d();
}

Field::~Field() {
	delete cs;
}

/* metoda care traseaza terenul */
void Field::markField() {
	drawNet();
	drawBoarders();
	drawField();
	cs_used.push_back(cs);
}

/* metode ajutatoare pentru trasarea terenului */
void Field::drawBoarders() {
	Object2d *boarders;
	vector<Point2d> points;
	vector<int> topology;

	setBoarderCoord(points);
	setTopology4Boarders(topology);
	boarders = new Object2d(points, topology);
	boarders->setcolor(0.7,0,0);
	cs->objectAdd(boarders);
}

void Field::drawNet() {
	Object2d *net;
	vector<Point2d> points;
	vector<int> topology;

	setNetCoord(points);
	setTopology4Net(topology);
	net = new Object2d(points, topology);
	cs->objectAdd(net);
}

void Field::drawField() {
	vector<Point2d> points;
	vector<int> topology;
	Object2d *obj;

	points.push_back(Point2d(-14,-19));
	points.push_back(Point2d(14,-19));;
	points.push_back(Point2d(-14,19)); 
	points.push_back(Point2d(14,19)); 

	topology.push_back(0);
	topology.push_back(1);
	topology.push_back(2);
	topology.push_back(1);
	topology.push_back(2);
	topology.push_back(3);

	obj = new Object2d(points, topology);
	obj->setcolor(0,0.8,0);
	cs->objectAdd(obj);
}

void Field::setBoarderCoord (vector<Point2d>& points) {
	points.push_back(Point2d(-14,-19));  //0
	points.push_back(Point2d(-14,-18.8f)); //1
	points.push_back(Point2d(14,-19)); 
	points.push_back(Point2d(14,-18.8f)); 

	points.push_back(Point2d(-14,19)); 
	points.push_back(Point2d(-13.8f,19)); 
	points.push_back(Point2d(-13.8f,-19)); 
	
	points.push_back(Point2d(-14,18.8f)); 
	points.push_back(Point2d(14,19)); 
	points.push_back(Point2d(14,18.8f));

	points.push_back(Point2d(13.8f,19)); 
	points.push_back(Point2d(13.8f,-19)); 
}

void Field::setNetCoord (vector<Point2d>& points) {
	points.push_back(Point2d(-6.0f,18.8f));  
	points.push_back(Point2d(-6.0f,19)); 
	points.push_back(Point2d(6.0f,19)); 
	points.push_back(Point2d(6.0f,18.8f)); 

	points.push_back(Point2d(-6.0f,-18.8f));  
	points.push_back(Point2d(-6.0f,-19)); 
	points.push_back(Point2d(6.0f,-19)); 
	points.push_back(Point2d(6.0f,-18.8f)); 

	points.push_back(Point2d(-13.8f,0.1f)); 
	points.push_back(Point2d(-13.8f,-0.1f)); 
	points.push_back(Point2d(13.8f,-0.1f));
	points.push_back(Point2d(13.8f,0.1f));
}
	
void Field::setTopology4Boarders (vector<int>& topology) {
	topology.push_back(0);
	topology.push_back(1);
	topology.push_back(2);
	topology.push_back(1);
	topology.push_back(2);
	topology.push_back(3);

	topology.push_back(0);
	topology.push_back(4);
	topology.push_back(6);
	topology.push_back(4);
	topology.push_back(6);
	topology.push_back(5);

	topology.push_back(4);
	topology.push_back(9);
	topology.push_back(7);
	topology.push_back(4);
	topology.push_back(9);
	topology.push_back(8);

	topology.push_back(10);
	topology.push_back(2);
	topology.push_back(8);
	topology.push_back(10);
	topology.push_back(2);
	topology.push_back(11);
}
	
void Field::setTopology4Net (vector<int>& topology) {
	topology.push_back(0);
	topology.push_back(1);
	topology.push_back(3);
	topology.push_back(1);
	topology.push_back(3);
	topology.push_back(2);

	topology.push_back(4);
	topology.push_back(6);
	topology.push_back(5);
	topology.push_back(4);
	topology.push_back(6);
	topology.push_back(7);

	topology.push_back(8);
	topology.push_back(10);
	topology.push_back(9);
	topology.push_back(8);
	topology.push_back(10);
	topology.push_back(11);
}

/* ========================================================================== */
