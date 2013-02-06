#include "WorldDrawer2d.h"

//static member init
CoordinateSystem2d WorldDrawer2d::cs_basis;
std::vector<CoordinateSystem2d*> WorldDrawer2d::cs_used;

void WorldDrawer2d::idleCallbackFunction(){
	//call client function
	onIdle();
	//redisplay
	glutPostRedisplay();
}

void WorldDrawer2d::reshapeCallbackFunction(int w, int h){
	glViewport(0,0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float aspect = (float)w/(float)h;
	gluPerspective(90.0f, aspect, 0.1f, 300.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 0.0, 20.0,0.0, 0.0, 0.0,	0.0, 1.0, 0.0);  //looking at xoy
}

void WorldDrawer2d::displayCallbackFunction(){
	//Render objects
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	if(cs_basis.draw_axis){
		//draw basis coord system
		glBegin(GL_LINES);
			glColor3f(0,1,1);
			glVertex3f(-cs_basis.axisup.x,-cs_basis.axisup.y,0.01f);
			glVertex3f(cs_basis.axisup.x,cs_basis.axisup.y,0.01f);
			glColor3f(1,0,1);
			glVertex3f(-cs_basis.axisright.x,-cs_basis.axisright.y,0.01f);
			glVertex3f(cs_basis.axisright.x,cs_basis.axisright.y,0.01f);
		glEnd();
	}

	//draw each used coord system
	for(unsigned int i=0;i<cs_used.size();i++){
		//draw used coord system
		if(cs_used[i]->draw_axis){
			glBegin(GL_LINES);
				glColor3f(0,1,0);
				glVertex3f(cs_used[i]->axiscenter.x,cs_used[i]->axiscenter.y,0.01f);
				glVertex3f(cs_used[i]->axisup.x,cs_used[i]->axisup.y,0.01f);
				glColor3f(1,0,0);
				glVertex3f(cs_used[i]->axiscenter.x,cs_used[i]->axiscenter.y,0.01f);
				glVertex3f(cs_used[i]->axisright.x,cs_used[i]->axisright.y,0.01f);
			glEnd();
		}

		//draw objects
		for(unsigned int j=0;j<cs_used[i]->objects.size();j++){
			//set object color
			glColor3f(cs_used[i]->objects[j]->colorx,cs_used[i]->objects[j]->colory,cs_used[i]->objects[j]->colorz);

			//get data
			std::vector<Point2d> points = cs_used[i]->objects[j]->points;
			std::vector<int> topology = cs_used[i]->objects[j]->topology;
			
			//draw
			if(topology.size()<2){
				std::cout<<"Folositi triunghiuri, dimensiune minima topologie =3"<<std::endl;
				continue;
			}

			//obiectul
			glBegin(GL_TRIANGLES);
				for(unsigned int k=0;k<topology.size();k++){
					int index=topology[k];
					glVertex3f(points[index].x, points[index].y,0);
				}
			glEnd();
			
			//axele obiectului
			if(cs_used[i]->objects[j]->drawaxis){
				glBegin(GL_LINES);
					glColor3f(0,1,0);
					glVertex3f(cs_used[i]->objects[j]->axiscenter.x,cs_used[i]->objects[j]->axiscenter.y,0.01f);
					glVertex3f(cs_used[i]->objects[j]->axisup.x,cs_used[i]->objects[j]->axisup.y,0.01f);
					glColor3f(1,0,0);
					glVertex3f(cs_used[i]->objects[j]->axiscenter.x,cs_used[i]->objects[j]->axiscenter.y,0.01f);
					glVertex3f(cs_used[i]->objects[j]->axisright.x,cs_used[i]->objects[j]->axisright.y,0.01f);
				glEnd();
			}
		}
	}

	//swap buffers
	glutSwapBuffers();
}
void WorldDrawer2d::keyboardCallbackFunction(unsigned char key, int posx, int posy){
	if(key==KEY_ESC) glutExit();
	//call client function
	onKey(key);
}

WorldDrawer2d::WorldDrawer2d() { }

WorldDrawer2d::WorldDrawer2d(int argc, char **argv, int windowWidth, int windowHeight, int windowStartX, int windowStartY, std::string windowName){
	//init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(windowWidth,windowHeight);
	glutInitWindowPosition(windowStartX,windowStartY);
	glutCreateWindow(windowName.c_str());
	
	//bind funcs
	glutDisplayFunc(displayCallbackFunction);
	glutReshapeFunc(reshapeCallbackFunction);
	glutIdleFunc(idleCallbackFunction);
	glutKeyboardFunc(keyboardCallbackFunction);

	glClearColor(0.4f,0.5f,1,1);
	//glClearColor(0.0f, 0.7f, 0.05f, 1);

	//zbuff
	glEnable(GL_DEPTH_TEST);
}
void WorldDrawer2d::run(){
	glutMainLoop();
	
}
WorldDrawer2d::~WorldDrawer2d(){
}