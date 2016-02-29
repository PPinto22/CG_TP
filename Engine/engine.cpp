#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <tinyxml.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

TiXmlDocument doc;
string xmlFolder;
float ax = 0;
float ay = 0;
float az = 0;
float xx = 0;
float zz = 0;
int draw_mode = 0; //0 = Fill, 1 = Line, 2 = Point

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 10.0,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	// put the geometric transformations here
	glTranslatef(xx, 0, zz);
	glRotatef(ax, 1, 0, 0);
	glRotatef(ay, 0, 1, 0);
	glRotatef(az, 0, 0, 1);

	// Drawing Mode
	switch (draw_mode) {
	case 0:
		glPolygonMode(GL_FRONT, GL_FILL);
		break;
	case 1:
		glPolygonMode(GL_FRONT, GL_LINE);
		break;
	case 2:
		glPolygonMode(GL_FRONT, GL_POINT);
		break;
	}

	//Leitura de ficheiros e desenho
	TiXmlHandle docHandle(&doc);
	TiXmlElement* elem = docHandle.FirstChild("scene").FirstChild("model").ToElement();
	string fileName;
	double x1,x2,x3,y1,y2,y3,z1,z2,z3;
	int numVertices;
	ifstream inFile;
	for (elem; elem; elem = elem->NextSiblingElement()) {
		fileName = elem->Attribute("file");
		fileName.insert(0, xmlFolder);
		inFile = ifstream(fileName);
		if (inFile >> numVertices) {
			for (int i = 0; i < numVertices / 3; i++) {
				inFile >> x1 >> y1 >> z1;
				inFile >> x2 >> y2 >> z2;
				inFile >> x3 >> y3 >> z3;
				glBegin(GL_TRIANGLES);
				glVertex3f(x1, y1, z1);
				glVertex3f(x2, y2, z2);
				glVertex3f(x3, y3, z3);
				glEnd();
			}
		}
	}
	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events
void keyPressed(unsigned char key, int x, int y) {
	if (key == 'w' || key == 'W') ax -= 5;
	if (key == 's' || key == 'S') ax += 5;
	if (key == 'a' || key == 'A') ay += 5;
	if (key == 'd' || key == 'D') ay -= 5;
	if (key == 'q' || key == 'Q') az += 5;
	if (key == 'e' || key == 'E') az -= 5;
	glutPostRedisplay();
}

void arrowPressed(int key_code, int x, int y) {
	if (key_code == GLUT_KEY_UP) zz-=0.2;
	else if (key_code == GLUT_KEY_DOWN) zz+=0.2;
	else if (key_code == GLUT_KEY_LEFT) xx-=0.2;
	else if (key_code == GLUT_KEY_RIGHT) xx+=0.2;
	glutPostRedisplay();
}

// write function to process menu events
void menuHandler(int id_op) {
	switch (id_op) {
	case 1:
		draw_mode = 0;
		break;
	case 2:
		draw_mode = 1;
		break;
	case 3:
		draw_mode = 2;
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv) {

	if (argc < 2) {
		cout << "Indique como argumento o path para o ficheiro .xml de configuracao\n";
		return 1;
	}

	doc = TiXmlDocument(argv[1]);
	bool loadOkay = doc.LoadFile();
	if (!loadOkay) {
		cout << "Falha ao carregar o ficheiro \"" << argv[1] <<"\"\n";
		return 2;
	}
	
	// Obter a pasta onde se encontra o ficheiro xml
	string str(argv[1]);
	int indice = str.find_last_of("/\\");
	if (indice == -1) indice = 0;
	else indice++;
	xmlFolder = str.substr(0, indice);

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("3D Engine");


	// Required callback registry 
	glutSpecialFunc(arrowPressed);
	glutKeyboardFunc(keyPressed);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);


	// put here the definition of the menu 
	glutCreateMenu(menuHandler);
	glutAddMenuEntry("Fill", 1);
	glutAddMenuEntry("Line", 2);
	glutAddMenuEntry("Point", 3);
	glutAttachMenu(GLUT_LEFT_BUTTON);


	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}
