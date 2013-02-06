#include "Game.h"
#include <unistd.h>

Game game;
int position = DYNAMIC, obiect = -1;
unsigned char pressed = NONE;
bool start = false, init = false;

void pick(int x, int y);
void processhits (GLint hits, GLuint buffer[]);
void output(GLfloat x, GLfloat y, GLfloat z, const char *format,...);

/* functie ce deseneaza lumea */
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	game.switch_camera(position);
	game.choose_level();
	game.start();
	
	if (!game.is_over() && start) {
		game.open_fire();
		game.perform_action(pressed, position);
		game.draw_asteroids();
		game.draw_spacecraft();
	} else if (game.is_over()) {
		game.end(position);
		position = DYNAMIC;
		start = false;
	}

	glutSwapBuffers();
}

/* se realizeaza o actiune in functie de tasta apasata */
void keyboard (unsigned char key, int x, int y) {
	
	switch (key) {
		case '1':
			position = DYNAMIC;
			game.reset_selected();
			break;

		case '2':
			position = SPACECRAFT;
			game.reset_selected();
			break;

		case 27: 
			exit(0);
	}

	pressed = key;	
}

/* functie de cu ajutorul careia se deseneaza asteroizi */
void idle() {
	if (init == true) {
		sleep(2);
		start = true;
		init = false;
	} 

	if (start == true) {
		game.gen_asteroids();
	}
	
	glutPostRedisplay();
}

/* functie cu ajutorul careia se realizeaza selectie asteroizilor */
void mouse(int buton, int stare, int x, int y) {
	if (buton == GLUT_LEFT_BUTTON) { 
		if (stare == GLUT_DOWN) {
			pick(x,y);
			
			if (obiect != -1 && start) {
				game.set_object(obiect);
			}

			if (obiect != -1 && !start) {
				game.set_level(obiect);
				init = true;
			}
		}
	}

	if (buton == GLUT_RIGHT_BUTTON) { 
		if (stare == GLUT_DOWN) {
			pick(x,y);
			
			if (obiect != -1) {
				game.set_selected(obiect);
			}
		}
	}
}

void reshape(int w, int h) {
	glViewport(0,0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w/h, 1.0, 3000.0); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main (int argc, char *argv[]) {
	//init glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	//init window
	glutInitWindowSize(1366,768);
	glutInitWindowPosition(200,200);
	glutCreateWindow("ASTEROIDS");
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);

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

void processhits (GLint hits, GLuint buffer[]) {
   int i;
   GLuint names, *ptr, minZ,*ptrNames, numberOfNames;

   // pointer la inceputul bufferului ce contine hit recordurile
   ptr = (GLuint *) buffer;
   // se doreste selectarea obiectului cel mai aproape de observator
   minZ = 0xffffffff;
   for (i = 0; i < hits; i++) 
   {
      // numarul de nume numele asociate din stiva de nume
      names = *ptr;
	  ptr++;
	  // Z-ul asociat hitului - se retine 
	  if (*ptr < minZ) {
		  numberOfNames = names;
		  minZ = *ptr;
		  // primul nume asociat obiectului
		  ptrNames = ptr+2;
	  }
	  
	  // salt la urmatorul hitrecord
	  ptr += names+2;
  }

  // identificatorul asociat obiectului
  ptr = ptrNames;
  
  obiect = *ptr;
}

//
void pick(int x, int y) {
	// buffer de selectie
	GLuint buffer[1024];

	// numar hituri
	GLint nhits;

	// coordonate viewport curent
	GLint	viewport[4];

	// se obtin coordonatele viewportului curent
	glGetIntegerv(GL_VIEWPORT, viewport);
	// se initializeaza si se seteaza bufferul de selectie
	memset(buffer,0x0,1024);
	glSelectBuffer(1024, buffer);
	
	// intrarea in modul de selectie
	glRenderMode(GL_SELECT);

	// salvare matrice de proiectie curenta
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// se va randa doar intr-o zona din jurul cursorului mouseului de [1,1]
	glGetIntegerv(GL_VIEWPORT,viewport);
	gluPickMatrix(x,viewport[3]-y,1.0f,1.0f,viewport);

	gluPerspective(45,(viewport[2]-viewport[0])/(GLfloat) (viewport[3]-viewport[1]),0.1,1000);
	glMatrixMode(GL_MODELVIEW);

	// se "deseneaza" scena : de fapt nu se va desena nimic in framebuffer ci se va folosi bufferul de selectie
	display();
	
	// restaurare matrice de proiectie initiala
	glMatrixMode(GL_PROJECTION);						
	glPopMatrix();				

	glMatrixMode(GL_MODELVIEW);
	
	// restaurarea modului de randare uzual si obtinerea numarului de hituri
	nhits = glRenderMode(GL_RENDER);	
	
	// procesare hituri
	if (nhits != 0)
		processhits(nhits,buffer);
	else
		obiect = -1;
}
