#include "Rubik.h"
//#include <windows.h>

/* construcorul clasei 'Rubik' care creeaza un nou cub rubik */
Rubik::Rubik() {
	cs = new CoordinateSystem3d();
	WorldDrawer3d::cs_used.push_back(cs);
	layers.resize(MOVING_LAYERS);
	faces.resize(MOVING_LAYERS);
	rubik.resize(3*MOVING_LAYERS);
	colors.resize(6);

	set_points();
	set_topology();
	set_layers();
	
	draw_faces();

	int count = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k++) {
				Object3d *o;
				o = new Object3d(points, topology);
				o->setcolor(0,0,0);
				o->color = -1;
				rubik[count].push_back(o);

				for (int l = 0; l < rubik[count].size(); l++) {
					rubik[count][l]->translate(4.5*i, 4.5*j, 4.5*k);
					cs->objectAdd(rubik[count][l]);
				}
				count++;
			}
		}
	}
}

Rubik::~Rubik() {
	for (int i = 0; i < rubik.size(); i++) {
		for (int j = 0; j < rubik[i].size(); j++) {
			delete rubik[i][j];	
		}
	}
	delete cs;
}

/* functie care deseneaza fetele cubului */
void Rubik::draw_faces() {
	Object3d *o;
	
	for (int i = 0; i < 9; i++) {
		o = new Object3d(points, faces[LEFT]);
		o->setcolor(1,0,0);
		o->color = LEFT;
		rubik[layers[LEFT][i]].push_back(o);	
	
		o = new Object3d(points, faces[RIGHT]);
		o->setcolor(0,1,0);
		o->color = RIGHT;
		rubik[layers[RIGHT][i]].push_back(o);	
	
		o = new Object3d(points, faces[FRONT]);
		o->setcolor(0,0,1);
		o->color = FRONT;
		rubik[layers[FRONT][i]].push_back(o);	
	
		o = new Object3d(points, faces[BACK]);
		o->setcolor(0.5,0.5,0);
		o->color = BACK;
		rubik[layers[BACK][i]].push_back(o);

		o = new Object3d(points, faces[BOTTOM]);
		o->setcolor(0.5,0,0.5);
		o->color = BOTTOM;
		rubik[layers[BOTTOM][i]].push_back(o);	

		o = new Object3d(points, faces[TOP]);
		o->setcolor(0,0.5,0.5);
		o->color = TOP;
		rubik[layers[TOP][i]].push_back(o);		
	}
}

/* ========================================================================= */

/* functie care returneaza stratul corespunzator variabilei 'layer' */
std::vector<int> Rubik::get_layer (int layer) {
	std::vector<int> to_move;
	Point3d center;
		
	for (int i = 0; i < rubik.size(); i++) {
		center = rubik[i].back()->axiscenter;

		if (layer == LEFT && center.x < -1) {
			to_move.push_back(i);	
		}	
				
		if (layer == RIGHT && center.x > 1) {
			to_move.push_back(i);
		}

		if (layer == TOP && center.y > 1) {
			to_move.push_back(i);
		}
			
		if (layer == BOTTOM && center.y < -1) {
			to_move.push_back(i);
		}
			
		if (layer == FRONT && center.z > 1) {
			to_move.push_back(i);
		}

		if (layer == BACK && center.z < -1) {
			to_move.push_back(i);
		}

		if (layer == CENTER_Y && fabs(center.x) < 0.2) {
			to_move.push_back(i);
		}
		
		if (layer == CENTER_X && fabs(center.y) < 0.2) {
			to_move.push_back(i);
		}

		if (layer == CENTER_Z && fabs(center.z) < 0.2) {
			to_move.push_back(i);
		}
	}

	return to_move;
}

/* functie care scaleaza un strat corespunzator variabilei 'i' */
void Rubik::scale (int i, int sign) {
	float x, y, z;

	if (sign > 0) {
		x = y = z = 1.05;
	} else {
		x = y = z = 0.95;
	}

	for (int j = 0; j < rubik[i].size(); j++) {
		rubik[i][j]->scale(x, y, z);
	}
}

/* functii de rotire al fiecarui cub compoment al cubul rubik */
void Rubik::rotateX (int i, Point3d center, float speed) {
	for (int j = 0; j < rubik[i].size(); j++) {
		rubik[i][j]->rotateXRelativeToPoint(center, speed);
	}
}

void Rubik::rotateY (int i, Point3d center, float speed) {
	for (int j = 0; j < rubik[i].size(); j++) {
		rubik[i][j]->rotateYRelativeToPoint(center, speed);
	}
}

void Rubik::rotateZ (int i, Point3d center, float speed) {
	for (int j = 0; j < rubik[i].size(); j++) {
		rubik[i][j]->rotateZRelativeToPoint(center, speed);
	}
}

/* functie care roteste un strat al cubului rubik in orice directie */
void Rubik::rotate_layer (int layer, float speed) {
	Point3d center;
	std::vector<int> to_move = get_layer(layer);

	for (int i = 0; i < to_move.size(); i++) {
		if (layer == LEFT || layer == RIGHT) {
			rotateX(to_move[i], cs->axiscenter, speed);
		}
			
		if (layer == TOP || layer == BOTTOM) {
			rotateY(to_move[i], cs->axiscenter, speed);
		}

		if (layer == BACK || layer == FRONT) {
			rotateZ(to_move[i], cs->axiscenter, speed);
		}

		if (layer == CENTER_X) {
			rotateY(to_move[i], cs->axiscenter, speed);
		}
			
		if (layer == CENTER_Y) {
			rotateX(to_move[i], cs->axiscenter, speed);
		}

		if (layer == CENTER_Z) {
			rotateZ(to_move[i], cs->axiscenter, speed);
		}
	}
}

/* functie care roteste intreg cubul rubik in orice directie */
void Rubik::rotate_all (int direction, float speed) {
	
	for (int i = 0; i < rubik.size(); i++) {
		switch (direction) 
		{
			case LEFT:
				rotateY(i, cs->axiscenter, speed);
				break;

			case RIGHT:	
				rotateY(i, cs->axiscenter, -speed);
				break;

			case UP:
				rotateZ(i, cs->axiscenter, speed);
				break;

			case DOWN:
				rotateZ(i, cs->axiscenter, -speed);
				break;

			case BOTTOM:
				rotateX(i, cs->axiscenter, speed);
				break;

			case TOP:
				rotateX(i, cs->axiscenter, -speed);
				break;
		}	
	}
}

/* metoda care retunreaza topologia cubului */
std::vector<int> Rubik::get_topology() {
	return topology;
}

/* metoda care returneaza punctele de referinta a cubului */
std::vector<Point3d> Rubik::get_points() {
	return points;
}

/* metoda care returneaza fiecare cub component al cubului rubik */
Cube Rubik::get_components (int i) {
	return rubik[i];
}

/* metoda care adauga cubul rubik in fereastra */
void Rubik::add() {
	WorldDrawer3d::cs_used.push_back(cs);
}

/* metoda care initialzeaza punctele cubului rubik */
void Rubik::set_points() {
	points.push_back(Point3d(2,2,2)); //0 
	points.push_back(Point3d(2,2,-2)); //1
	points.push_back(Point3d(-2,2,-2)); //2
	points.push_back(Point3d(-2,2,2)); //3
	points.push_back(Point3d(2,-2,2)); //4
	points.push_back(Point3d(2,-2,-2)); //5
	points.push_back(Point3d(-2,-2,-2)); //6
	points.push_back(Point3d(-2,-2,2)); //7
}

/* metoda care seteaza topologia cubului rubik */
void Rubik::set_topology() {
	topology.push_back(0);topology.push_back(1);topology.push_back(2);	//top
	topology.push_back(2);topology.push_back(3);topology.push_back(0);
	faces[TOP].assign(topology.begin(), topology.end());

	topology.push_back(6);topology.push_back(5);topology.push_back(4);	//bottom
	topology.push_back(7);topology.push_back(4);topology.push_back(6);
	faces[BOTTOM].assign(topology.begin()+6, topology.end());

	topology.push_back(2);topology.push_back(3);topology.push_back(6);	//left
	topology.push_back(7);topology.push_back(3);topology.push_back(6);
	faces[LEFT].assign(topology.begin()+12, topology.end());

	topology.push_back(0);topology.push_back(1);topology.push_back(5);	//right
	topology.push_back(0);topology.push_back(5);topology.push_back(4);
	faces[RIGHT].assign(topology.begin()+18, topology.end());

	topology.push_back(5);topology.push_back(1);topology.push_back(2);	//back
	topology.push_back(6);topology.push_back(2);topology.push_back(5);
	faces[BACK].assign(topology.begin()+24, topology.end());

	topology.push_back(0);topology.push_back(3);topology.push_back(4);	//front
	topology.push_back(7);topology.push_back(3);topology.push_back(4);
	faces[FRONT].assign(topology.begin()+30, topology.end());
}

/* metoda care seteaza straturile cubului rubik */
void Rubik::set_layers () {
	int left[] = {0,1,2,3,4,5,6,7,8};
	layers[LEFT].assign(left,left+9); 
	
	int right[] = {18,19,20,21,22,23,24,25,26};
	layers[RIGHT].assign(right,right+9);
	
	int front[] = {2,5,8,11,14,17,20,23,26};
	layers[FRONT].assign(front,front+9);
	
	int back[] = {0,3,6,9,12,15,18,21,24};
	layers[BACK].assign(back,back+9); 
	
	int top[] = {6,7,8,15,16,17,24,25,26};
	layers[TOP].assign(top,top+9); 
	
	int bottom[] = {0,1,2,9,10,11,18,19,20}; 
	layers[BOTTOM].assign(bottom,bottom+9);
}

