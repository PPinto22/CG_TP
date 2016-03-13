#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <tinyxml.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Ponto {
private: float xval, yval, zval;

public:
	Ponto(float x, float y, float z) {
		xval = x;
		yval = y;
		zval = z;
	}
	float x() { return xval; }
	float y() { return yval; }
	float z() { return zval; }
};

class Rotacao {
public:
	float angulo, x, y, z;

	Rotacao(float angulo, float x, float y, float z) {
		this->angulo = angulo;
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

class Transformacao{
public:
	vector<Rotacao> rotacoes;
	float tx, ty, tz;
	float scale;

	Transformacao(vector<Rotacao> rotacoes, float tx, float ty, float tz, float scale) {
		this->rotacoes = rotacoes;
		this->tx = tx;
		this->ty = ty;
		this->tz = tz;
		this->scale = scale;
	}

	void addRotacao(Rotacao rotacao) {
		rotacoes.push_back(rotacao);
	}
};

float camDistance = 10;
int nivelAlpha, nivelBeta;
int niveisAlpha = 16;
int niveisBeta = 16;
int draw_mode = 0; //0 = Fill, 1 = Line, 2 = Point

/*vector<Ponto> pontos;
int numPontos;*/



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
	float beta = nivelBeta * M_PI_2/niveisBeta;
	float alpha = nivelAlpha * M_PI/niveisAlpha;
	float z = camDistance * cos(beta) * cos(alpha);
	float x = camDistance * cos(beta) * sin(alpha);
	float y = camDistance * sin(beta);
	gluLookAt(x, y, z,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	// put the geometric transformations here

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

	//Desenho
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < numPontos; i+=3) {
		Ponto p1(pontos[i]);
		Ponto p2(pontos[i + 1]);
		Ponto p3(pontos[i + 2]);
		glVertex3f(p1.x(), p1.y(), p1.z());
		glVertex3f(p2.x(), p2.y(), p2.z());
		glVertex3f(p3.x(), p3.y(), p3.z());
	}
	glEnd();

	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events
void keyPressed(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':case 'W':
		if (nivelBeta < niveisBeta) nivelBeta++;
		break;
	case 's':case 'S':
		if (nivelBeta > -niveisBeta) nivelBeta--;
		break;
	case 'a':case 'A':
		nivelAlpha--;
		break;
	case 'd':case 'D':
		nivelAlpha++;
		break;
	case 'z':case 'Z':
		camDistance += 0.5;
		break;
	case 'x':case'X':
		camDistance -= 0.5;
		break;
	}
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

void prepareScene(TiXmlHandle doc, string doc_path) {
	TiXmlElement* elem = doc.FirstChild("scene").FirstChild("model").ToElement();
	string fileName;
	float x, y, z;
	ifstream inFile;
	for (elem; elem; elem = elem->NextSiblingElement()) {
		fileName = elem->Attribute("file");
		fileName.insert(0, doc_path);
		inFile = ifstream(fileName);
		int pontosFicheiro;
		if (inFile >> pontosFicheiro) {
			numPontos += pontosFicheiro;
			for (int i = 0; i < pontosFicheiro; i++) {
				inFile >> x >> y >> z;
				Ponto p(x, y, z);
				pontos.push_back(p);
			}
		}
	}
}

int main(int argc, char **argv) {

	if (argc < 2) {
		cout << "Indique como argumento o path para o ficheiro .xml de configuracao\n";
		return 1;
	}

	TiXmlDocument doc(argv[1]);
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
	string xmlFolder = str.substr(0, indice);

	TiXmlHandle docHandle(&doc);

	prepareScene(docHandle, xmlFolder);

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("3D Engine");


	// Required callback registry
	glutKeyboardFunc(keyPressed);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);


	// put here the definition of the menu 
	glutCreateMenu(menuHandler);
	glutAddMenuEntry("Fill", 1);
	glutAddMenuEntry("Line", 2);
	glutAddMenuEntry("Point", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}
