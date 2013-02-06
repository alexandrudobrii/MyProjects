#include "labyrinth.h"
#include <ctime>

Labyrinth *labyrinth;
int position = TD;
char pressed = NONE;
char files[][10] = {"Labirint1", "Labirint2",
				    "Labirint3", "Labirint1",
				   };

/* se initializeaza labirintul */
void init (const char *filename) {
	labyrinth =  new Labyrinth(filename);
}

/* se deseneaza labirintul */
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	labyrinth->switch_camera(position);
	labyrinth->perform_action(pressed, position);
	labyrinth->draw_ground();
	labyrinth->draw_walls();

	glutSwapBuffers();
}

/* se seteaza campul de vizualizare */
void reshape (int width, int height) {
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(float)width/(float)height,0.2,3000);
}

/* se afiseaza modificarile facute la fiecare pas */
void idle() {
	glutPostRedisplay();
}

/* se realizeaza o actiune in functie de tasta apasata */
void keyboard (unsigned char key, int x, int y) {
	switch(key) {
		case '1':
			position = TD;
			labyrinth->reset_moved();
			return;
		case '2': 
			position = FPS;
			labyrinth->reset_moved();
			return;
		case '3':
			position = TPS;
			return;

		case ESC:
			delete labyrinth;
			exit(0);
			break;
	}

	pressed = key;	
}

int main (int argc, char *argv[]) {
	//init glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	//init window
	glutInitWindowSize(600,600);
	glutInitWindowPosition(200,200);
	glutCreateWindow("Labyrinth");
	
	srand(time(NULL));
	int random = rand()%3;
	init(files[random]);
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	//z test on
	glEnable(GL_DEPTH_TEST);

	//set background
	glClearColor(BLACK,1.0);
	
	//init lumina
	const GLfloat lumina_position4f[] = {3.0, 3.0, 0.5, 0.0};
	const GLfloat lumina_color4f[] = {0.9, 0.9, 0.9, 1.0};
	const GLfloat globalAmbientColor4f[] = {0.2, 0.2, 0.2, 1.0};
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientColor4f);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lumina_color4f);
	glLightfv(GL_LIGHT0, GL_POSITION, lumina_position4f);
	glEnable(GL_LIGHT0);
	
	//init culoare=material
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	
	//loop
	glutMainLoop();

	return 0;
}

