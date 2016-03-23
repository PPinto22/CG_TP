#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glew.h>
#include <GL/glut.h>
#include <tinyxml.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#pragma comment(lib,"glew32.lib")

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
	float sx, sy, sz;

	Transformacao(vector<Rotacao> rotacoes, float tx, float ty, float tz, float sx, float sy, float sz) {
		this->rotacoes = rotacoes;
		this->tx = tx;
		this->ty = ty;
		this->tz = tz;
		this->sx = sx;
		this->sy = sy;
		this->sz = sz;
	}

	Transformacao() {
		this->tx = 0.0f; this->ty = 0.0f; this->tz = 0.0f;
		this->sx = 1.0f; this->sy = 1.0f; this->sz = 1.0f;
	}

	void merge(Transformacao t) {
		this->tx += t.tx; this->ty += t.ty; this->tz += t.tz;
		this->sx *= t.sx; this->sy *= t.sy; this->sz *= t.sz;
		vector<Rotacao> rotacoesAux;
		for (int i = 0; i < t.rotacoes.size(); i++) {
			rotacoesAux.push_back(t.rotacoes[i]);
		}
		for (int i = 0; i < this->rotacoes.size(); i++) {
			rotacoesAux.push_back(this->rotacoes[i]);
		}
		rotacoes.clear();
		this->rotacoes = rotacoesAux;
	}
};

float camDistance = 260;
int nivelAlpha = 0, nivelBeta = 0;
int niveisAlpha = 50;
int niveisBeta = 50;
int draw_mode = 0; //0 = Fill, 1 = Line, 2 = Point

GLuint* buffers;
int *numVertices;
int numModelos;
vector<Transformacao> transformacoes;

string xmlPath;

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

	for (int i = 0; i < numModelos; i++) {
		glPushMatrix();

		Transformacao transformacao = transformacoes[i];
		glTranslatef(transformacao.tx, transformacao.ty, transformacao.tz);
		for (int j=0; j<transformacao.rotacoes.size(); j++) {
			Rotacao rotacao = transformacao.rotacoes[j];
			glRotatef(rotacao.angulo, rotacao.x, rotacao.y, rotacao.z);
		}
		glScalef(transformacao.sx, transformacao.sy, transformacao.sz);
		
		glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, numVertices[i]);

		glPopMatrix();
	}

	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events
void keyPressed(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':case 'W':
		if (nivelBeta < niveisBeta-1) nivelBeta++;
		break;
	case 's':case 'S':
		if (nivelBeta > -niveisBeta+1) nivelBeta--;
		break;
	case 'a':case 'A':
		nivelAlpha--;
		break;
	case 'd':case 'D':
		nivelAlpha++;
		break;
	case 'z':case 'Z':
		camDistance += 4;
		break;
	case 'x':case'X':
		camDistance -= 4;
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

/*
	Conta número de modelos existentes no ficheiro .xml
*/
int countModels(int count, TiXmlElement* group) {
	int n = 0;
	for (group; group; group= group->NextSiblingElement()) {
		TiXmlElement* model = group->FirstChildElement("models")->FirstChildElement("model");
		for (model; model; model = model->NextSiblingElement()) {
			n++;
		}
		TiXmlElement* group_rec = group->FirstChildElement("group");
		for (group_rec; group_rec; group_rec = group_rec->NextSiblingElement()) {
			n += countModels(n, group_rec);
		}
	}
	return n;
}

/*
	Carrega todos os modelos existentes num grupo para 'buffers', 
	associando a cada um as transformações respetivas, guardando-as no 'vector<Transformacao> transformacoes'
	Continua, recursivamente, para todos os subgrupos.
*/
void prepareGroup(TiXmlElement* group, Transformacao t) {
	for (group; group; group=group->NextSiblingElement()) {
		TiXmlElement* rotacoes = group->FirstChildElement("rotate");
		vector<Rotacao> vectorR;
		for (rotacoes; rotacoes; rotacoes=rotacoes->NextSiblingElement("rotate")) {
			float angle = 0.0f, ax = 0.0f, ay = 0.0f, az = 0.0f;
			const char* str_angle = rotacoes->Attribute("angle");
			if (str_angle) angle = atof(str_angle);
			const char* str_ax = rotacoes->Attribute("axisX");
			if (str_ax) ax = atof(str_ax);
			const char* str_ay = rotacoes->Attribute("axisY");
			if (str_ay) ay = atof(str_ay);
			const char* str_az = rotacoes->Attribute("axisZ");
			if (str_az) az = atof(str_az);

			Rotacao rotacao(angle, ax, ay, az);
			vectorR.push_back(rotacao);
		}
		TiXmlElement* translacoes = group->FirstChildElement("translate");
		float tx = 0.0f, ty = 0.0f, tz = 0.0f;
		if (translacoes) {
			const char* str_tx = translacoes->Attribute("X");
			if (str_tx) tx = atof(str_tx);
			const char* str_ty = translacoes->Attribute("Y");
			if (str_ty) ty = atof(str_ty);
			const char* str_tz = translacoes->Attribute("Z");
			if (str_tz) tz = atof(str_tz);
		}
		TiXmlElement* escalas = group->FirstChildElement("scale");
		float sx = 1.0f, sy = 1.0f, sz = 1.0f;
		if (escalas) {
			const char* str_sx = escalas->Attribute("X");
			if (str_sx) sx = atof(str_sx);
			const char* str_sy = escalas->Attribute("Y");
			if (str_sy) sy = atof(str_sy);
			const char* str_sz = escalas->Attribute("Z");
			if (str_sz) sz = atof(str_sz);
		}
		Transformacao transformacao(vectorR, tx, ty, tz, sx, sy, sz);
		vectorR.clear();
		transformacao.merge(t);
		TiXmlElement* model = group->FirstChildElement("models")->FirstChildElement("model");
		for (model; model; model=model->NextSiblingElement()) {
			string fileName = model->Attribute("file");
			fileName.insert(0, xmlPath);
			ifstream inFile(fileName);
			int pontosFicheiro;
			if (inFile >> pontosFicheiro) {
				float* coordenadas = (float*)malloc(sizeof(float)*pontosFicheiro * 3);
				for (int i = 0; i < pontosFicheiro*3; i++) {
					inFile >> coordenadas[i];
				}
				int indice = transformacoes.size();
				transformacoes.push_back(transformacao);
				numVertices[indice] = pontosFicheiro;
				glBindBuffer(GL_ARRAY_BUFFER, buffers[indice]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float)*pontosFicheiro*3 , coordenadas, GL_STATIC_DRAW);
				free(coordenadas);
			}
		}
		TiXmlElement* group_rec = group->FirstChildElement("group");
		for (group_rec; group_rec; group_rec = group_rec->NextSiblingElement()) {
			prepareGroup(group_rec, transformacao);
		}
	}
}

/*
	Lê o ficheiro .xml, e guarda todos os modelos e respetivas transformações em memória
*/
void prepareScene(TiXmlHandle doc) {
	TiXmlElement* group = doc.FirstChild("scene").FirstChild("group").ToElement();
	if (!group) return;

	numModelos = countModels(0, group);
	buffers = (GLuint*)malloc(sizeof(GLuint)*numModelos);
	glGenBuffers(numModelos, buffers);
	numVertices = (int*)malloc(sizeof(int)*numModelos);

	Transformacao transformacao = Transformacao();
	prepareGroup(group, transformacao);

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
	xmlPath = str.substr(0, indice);

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("3D Engine");

	//init glew
	glewInit();

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
	glEnableClientState(GL_VERTEX_ARRAY);

	// Ler xml e preparar cena
	TiXmlHandle docHandle(&doc);
	prepareScene(docHandle);

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}
