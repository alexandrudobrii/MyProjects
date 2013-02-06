#include "WorldDrawer3d.h"

CoordinateSystem3d WorldDrawer3d::cs_basis;
std::vector<CoordinateSystem3d*> WorldDrawer3d::cs_used;

using namespace std;

void WorldDrawer3d::idleCallbackFunction(){
	//call client function
	onIdle();
	//redisplay
	glutPostRedisplay();
}

void WorldDrawer3d::reshapeCallbackFunction(int w, int h){
	glViewport(0,0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float aspect = (float)w/(float)h;
	gluPerspective(90.0f, aspect, 0.1f, 3000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(20.0, 20.0, 20.0,0.0, 0.0, 0.0, 0.0, 1.0, 0.0);  //looking at xoy
}

void WorldDrawer3d::displayCallbackFunction(){
	//Render objects
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glLineWidth(1);
	//draw basis coord system
	if(cs_basis.draw_axis){
		glBegin(GL_LINES);
			glColor3f(0,1,1);
			glVertex3f(cs_basis.axisup.x,-cs_basis.axisup.y,cs_basis.axisup.z);
			glVertex3f(cs_basis.axisup.x,cs_basis.axisup.y,cs_basis.axisup.z);
			glColor3f(1,0,1);
			glVertex3f(-cs_basis.axisright.x,cs_basis.axisright.y,cs_basis.axisright.z);
			glVertex3f(cs_basis.axisright.x,cs_basis.axisright.y,cs_basis.axisright.z);
			glColor3f(1,1,0);
			glVertex3f(cs_basis.axisfwd.x,cs_basis.axisfwd.y,-cs_basis.axisfwd.z);
			glVertex3f(cs_basis.axisfwd.x,cs_basis.axisfwd.y,cs_basis.axisfwd.z);
		glEnd();
		glPointSize(5);
		glBegin(GL_POINTS);
			glColor3f(0,1,1);	glVertex3f(cs_basis.axisup.x,cs_basis.axisup.y,cs_basis.axisup.z);
			glColor3f(1,0,1);	glVertex3f(cs_basis.axisright.x,cs_basis.axisright.y,cs_basis.axisright.z);
			glColor3f(1,1,0);	glVertex3f(cs_basis.axisfwd.x,cs_basis.axisfwd.y,cs_basis.axisfwd.z);
		glEnd();

	}

	//draw objects in cs_basis
	for(unsigned int j=0;j<cs_basis.objects.size();j++){
		//set object color
		glColor3f(cs_basis.objects[j]->colorx,cs_basis.objects[j]->colory,cs_basis.objects[j]->colorz);

		//get data
		std::vector<Point3d> points = cs_basis.objects[j]->points;
		std::vector<int> topology = cs_basis.objects[j]->topology;
		
		//draw
		if(topology.size()<2){
			std::cout<<"Folositi triunghiuri, dimensiune minima topologie =3"<<std::endl;
			continue;
		}

		//obiectul
		glBegin(GL_TRIANGLES);
			for(unsigned int k=0;k<topology.size();k++){
				int index=topology[k];
				glVertex3f(points[index].x, points[index].y,points[index].z);
			}
		glEnd();
		
		//axele obiectului
		glLineWidth(2);
		if(cs_basis.objects[j]->draw_axis){
			glBegin(GL_LINES);
				glColor3f(0,1,0);
				glVertex3f(cs_basis.objects[j]->axiscenter.x,cs_basis.objects[j]->axiscenter.y,cs_basis.objects[j]->axiscenter.z);
				glVertex3f(cs_basis.objects[j]->axisup.x,cs_basis.objects[j]->axisup.y,cs_basis.objects[j]->axisup.z);
				glColor3f(1,0,0);
				glVertex3f(cs_basis.objects[j]->axiscenter.x,cs_basis.objects[j]->axiscenter.y,cs_basis.objects[j]->axiscenter.z);
				glVertex3f(cs_basis.objects[j]->axisright.x,cs_basis.objects[j]->axisright.y,cs_basis.objects[j]->axisright.z);
				glColor3f(0,0,1);
				glVertex3f(cs_basis.objects[j]->axiscenter.x,cs_basis.objects[j]->axiscenter.y,cs_basis.objects[j]->axiscenter.z);
				glVertex3f(cs_basis.objects[j]->axisfwd.x,cs_basis.objects[j]->axisfwd.y,cs_basis.objects[j]->axisfwd.z);
			glEnd();
		}
	}

	//draw each used coord system
	for(unsigned int i=0;i<cs_used.size();i++){
		//draw used coord system
		glLineWidth(1);
		if(cs_used[i]->draw_axis){
			glBegin(GL_LINES);
				glColor3f(0,1,0);
				glVertex3f(cs_used[i]->axiscenter.x,cs_used[i]->axiscenter.y,cs_used[i]->axiscenter.z);
				glVertex3f(cs_used[i]->axisup.x,cs_used[i]->axisup.y,cs_used[i]->axisup.z);
				glColor3f(1,0,0);
				glVertex3f(cs_used[i]->axiscenter.x,cs_used[i]->axiscenter.y,cs_used[i]->axiscenter.z);
				glVertex3f(cs_used[i]->axisright.x,cs_used[i]->axisright.y,cs_used[i]->axisright.z);
				glColor3f(0,0,1);
				glVertex3f(cs_used[i]->axiscenter.x,cs_used[i]->axiscenter.y,cs_used[i]->axiscenter.z);
				glVertex3f(cs_used[i]->axisfwd.x,cs_used[i]->axisfwd.y,cs_used[i]->axisfwd.z);
			glEnd();
		}

		//draw objects
		for(unsigned int j=0;j<cs_used[i]->objects.size();j++){
			//set object color
			glColor3f(cs_used[i]->objects[j]->colorx,cs_used[i]->objects[j]->colory,cs_used[i]->objects[j]->colorz);

			//get data
			std::vector<Point3d> points = cs_used[i]->objects[j]->points;
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
					glVertex3f(points[index].x, points[index].y,points[index].z);
				}
			glEnd();
			
			//axele obiectului
			glLineWidth(2);
			if(cs_used[i]->objects[j]->draw_axis){
				glBegin(GL_LINES);
					glColor3f(0,1,0);
					glVertex3f(cs_used[i]->objects[j]->axiscenter.x,cs_used[i]->objects[j]->axiscenter.y,cs_used[i]->objects[j]->axiscenter.z);
					glVertex3f(cs_used[i]->objects[j]->axisup.x,cs_used[i]->objects[j]->axisup.y,cs_used[i]->objects[j]->axisup.z);
					glColor3f(1,0,0);
					glVertex3f(cs_used[i]->objects[j]->axiscenter.x,cs_used[i]->objects[j]->axiscenter.y,cs_used[i]->objects[j]->axiscenter.z);
					glVertex3f(cs_used[i]->objects[j]->axisright.x,cs_used[i]->objects[j]->axisright.y,cs_used[i]->objects[j]->axisright.z);
					glColor3f(0,0,1);
					glVertex3f(cs_used[i]->objects[j]->axiscenter.x,cs_used[i]->objects[j]->axiscenter.y,cs_used[i]->objects[j]->axiscenter.z);
					glVertex3f(cs_used[i]->objects[j]->axisfwd.x,cs_used[i]->objects[j]->axisfwd.y,cs_used[i]->objects[j]->axisfwd.z);
				glEnd();
			}
		}
	}

	//swap buffers
	glutSwapBuffers();
}
void WorldDrawer3d::keyboardCallbackFunction(unsigned char key, int posx, int posy){
	if(key==KEY_ESC) glutExit();
	//call client function
	onKey(key);
}
void WorldDrawer3d::keyboardSpecialCallbackFunction(int key, int posx, int posy){
	//call client function
	onKey(key);
}

WorldDrawer3d::WorldDrawer3d(int argc, char **argv, int windowWidth, int windowHeight, int windowStartX, int windowStartY, std::string windowName){
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
	glutSpecialFunc(keyboardSpecialCallbackFunction);

	glClearColor(0.4f,0.5f,1,1);

	//zbuff
	glEnable(GL_DEPTH_TEST);
}
void WorldDrawer3d::run(){
	glutMainLoop();
}
WorldDrawer3d::~WorldDrawer3d(){
}