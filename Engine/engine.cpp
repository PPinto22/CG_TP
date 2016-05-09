#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "tinyxml.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
#include "ponto.h"
#include "rotacao.h"
#include "translacao.h"
#include "transformacao.h"
#include "luz.h"
#pragma comment(lib,"glew32.lib")

#define PONTOS_LINHA_ORBITA 1000

float camX = 0, camY = 0, camZ = 300;
int startX, startY, tracking = 0;

int alpha = 0, beta = 0, r = 300;

int draw_mode = 0; //0 = Fill, 1 = Line, 2 = Point

GLuint* buffers;
GLuint* normals;
GLuint* textures;
int *numVertices;
int numModelos;
vector<Transformacao> transformacoes;
vector<Luz> luzes;

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

int timebase = 0;
int frame = 0;
float fps;

void renderScene(void) {

	int time;

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	for (int i = 0; i < luzes.size(); i++) {
		switch (i) {
		case 0:
			glLightfv(GL_LIGHT0, GL_POSITION, luzes[i].pos);
			break;
		case 1:
			glLightfv(GL_LIGHT1, GL_POSITION, luzes[i].pos);
			break;
		case 2:
			glLightfv(GL_LIGHT2, GL_POSITION, luzes[i].pos);
			break;
		case 3:
			glLightfv(GL_LIGHT3, GL_POSITION, luzes[i].pos);
			break;
		case 4:
			glLightfv(GL_LIGHT4, GL_POSITION, luzes[i].pos);
			break;
		case 5:
			glLightfv(GL_LIGHT5, GL_POSITION, luzes[i].pos);
			break;
		case 6:
			glLightfv(GL_LIGHT6, GL_POSITION, luzes[i].pos);
			break;
		case 7:
			glLightfv(GL_LIGHT7, GL_POSITION, luzes[i].pos);
			break;
		}
	}
	

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

		Rotacao rotacao = transformacao.rotacao;
		int tempo_rotacao = rotacao.tempo;

		vector<Translacao> translacoes = transformacao.translacoes;
		for (int i = 0; i < translacoes.size(); i++) {
			time = glutGet(GLUT_ELAPSED_TIME);
			Translacao translacao = translacoes[i];

			glBindBuffer(GL_ARRAY_BUFFER, translacao.orbita);
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glDrawArrays(GL_LINE_LOOP, 0, PONTOS_LINHA_ORBITA);

			Ponto p = translacao.getPoint(time);
			glTranslatef(p.x(), p.y(), p.z());

			if (tempo_rotacao <= 0) {
				float* rot_matrix = translacao.getRotMatrix(time);
				glMultMatrixf(rot_matrix);
				free(rot_matrix);
			}
		}

		if (tempo_rotacao > 0) {
			time = glutGet(GLUT_ELAPSED_TIME) % tempo_rotacao;
			float angulo = (float)time / tempo_rotacao * 360.0f;
			glRotatef(angulo, rotacao.x, rotacao.y, rotacao.z);
		}

		glScalef(transformacao.sx, transformacao.sy, transformacao.sz);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, numVertices[i]);

		glPopMatrix();
	}

	//Calcular fps
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		fps = frame*1000.0 / (time - timebase);
		timebase = time;
		frame = 0;
	}

	char* fps_string = (char*)malloc(8 * sizeof(char));
	sprintf(fps_string, "%.0f fps", fps);
	glutSetWindowTitle(fps_string);

	// End of frame
	glutSwapBuffers();
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
Conta numero de modelos existentes no ficheiro .xml
*/
int countModels(int count, TiXmlElement* group) {
	int n = 0;
	for (group; group; group = group->NextSiblingElement()) {
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
Carrega todos os modelos existentes num grupo, associando
a cada um as transformacoes respetivas, guardando-as no 'vector<Transformacao> transformacoes'
Continua, recursivamente, para todos os subgrupos.
*/
void prepareGroup(TiXmlElement* group, Transformacao t) {
	for (group; group; group = group->NextSiblingElement()) {
		TiXmlElement* elemRotacao = group->FirstChildElement("rotate");
		Rotacao rotacao = Rotacao();
		if (elemRotacao) {
			float time = 0.0f, ax = 0.0f, ay = 0.0f, az = 0.0f;
			const char* str_time = elemRotacao->Attribute("time");
			if (str_time) time = atof(str_time);
			const char* str_ax = elemRotacao->Attribute("axisX");
			if (str_ax) ax = atof(str_ax);
			const char* str_ay = elemRotacao->Attribute("axisY");
			if (str_ay) ay = atof(str_ay);
			const char* str_az = elemRotacao->Attribute("axisZ");
			if (str_az) az = atof(str_az);

			rotacao = Rotacao(time, ax, ay, az);
		}
		TiXmlElement* elemTranslacao = group->FirstChildElement("translate");
		bool translacao_OK = false;
		int numPontosControlo = 0;
		float time = 0;
		vector<Ponto> pontos;
		if (elemTranslacao) {
			const char* str_time = elemTranslacao->Attribute("time");
			if (str_time) time = atof(str_time);
			TiXmlElement* elemPoint = elemTranslacao->FirstChildElement("point");
			for (elemPoint; elemPoint; elemPoint = elemPoint->NextSiblingElement()) {
				numPontosControlo++;
				float px = 0, py = 0, pz = 0;
				const char* str_x = elemPoint->Attribute("X");
				if (str_x) px = atof(str_x);
				const char* str_sy = elemPoint->Attribute("Y");
				if (str_sy) py = atof(str_sy);
				const char* str_sz = elemPoint->Attribute("Z");
				if (str_sz) pz = atof(str_sz);
				Ponto p(px, py, pz);
				pontos.push_back(p);
			}
		}
		if (numPontosControlo >= 4 && time > 0)
			translacao_OK = true;
		Translacao translacao(time, pontos);
		TiXmlElement* elemScale = group->FirstChildElement("scale");
		float sx = 1.0f, sy = 1.0f, sz = 1.0f;
		if (elemScale) {
			const char* str_sx = elemScale->Attribute("X");
			if (str_sx) sx = atof(str_sx);
			const char* str_sy = elemScale->Attribute("Y");
			if (str_sy) sy = atof(str_sy);
			const char* str_sz = elemScale->Attribute("Z");
			if (str_sz) sz = atof(str_sz);
		}

		Transformacao transformacao;
		if (translacao_OK) {
			translacao.gerarLinhaOrbita(PONTOS_LINHA_ORBITA);
			transformacao = Transformacao(rotacao, translacao, sx, sy, sz);
		}
		else
			transformacao = Transformacao(rotacao, sx, sy, sz);

		transformacao.merge(t);
		TiXmlElement* model = group->FirstChildElement("models")->FirstChildElement("model");
		for (model; model; model = model->NextSiblingElement()) {
			string fileName = model->Attribute("file");
			fileName.insert(0, xmlPath);
			ifstream inFile(fileName);
			int pontosFicheiro;
			if (inFile >> pontosFicheiro) {
				float* coordenadas = (float*)malloc(sizeof(float)*pontosFicheiro * 3);
				float* coordenadas_normais = (float*)malloc(sizeof(float)*pontosFicheiro * 3);

				for (int i = 0; i < pontosFicheiro; i++) {
					int j = i * 3;
					inFile >> coordenadas[j] >> coordenadas[j + 1] >> coordenadas[j + 2];
					inFile >> coordenadas_normais[j] >> coordenadas_normais[j + 1] >> coordenadas_normais[j + 2];
				}

				int indice = transformacoes.size();
				transformacoes.push_back(transformacao);
				numVertices[indice] = pontosFicheiro;
				glBindBuffer(GL_ARRAY_BUFFER, normals[indice]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float)*pontosFicheiro * 3, coordenadas_normais, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, buffers[indice]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float)*pontosFicheiro * 3, coordenadas, GL_STATIC_DRAW);
				free(coordenadas);
			}
		}
		TiXmlElement* group_rec = group->FirstChildElement("group");
		prepareGroup(group_rec, transformacao);
	}
}

void prepareLights(TiXmlElement* lights) {
	if (!lights) return;

	glEnable(GL_LIGHTING);

	TiXmlElement* luz = lights->FirstChildElement("light");
	for (int i = 0; luz && i<8 ; luz = luz->NextSiblingElement(), i++) {
		float type = 1.0;
		float posX = 0.0, posY = 0.0, posZ = 0.0;
		float ambR = 0.2, ambG = 0.2, ambB = 0.2;
		float diffR = 1.0, diffG = 1.0, diffB = 1.0;

		const char* str_type = luz->Attribute("type");
		if (str_type) {
			if (strcmp(str_type, "POINT") == 0)
				type = 1.0;
			else if (strcmp(str_type, "VECTOR") == 0)
				type = 0.0;
		}

		const char* str_posX = luz->Attribute("posX");
		if (str_posX) posX = atof(str_posX);
		const char* str_posY = luz->Attribute("posY");
		if (str_posY) posY = atof(str_posY);
		const char* str_posZ = luz->Attribute("posZ");
		if (str_posZ) posZ = atof(str_posZ);

		const char* str_ambR = luz->Attribute("ambR");
		if (str_ambR) ambR = atof(str_ambR);
		const char* str_ambG = luz->Attribute("ambG");
		if (str_ambG) ambG = atof(str_ambG);
		const char* str_ambB = luz->Attribute("ambB");
		if (str_ambB) ambB = atof(str_ambB);

		const char* str_diffR = luz->Attribute("diffR");
		if (str_diffR) diffR = atof(str_diffR);
		const char* str_diffG = luz->Attribute("diffG");
		if (str_diffG) diffG = atof(str_diffG);
		const char* str_diffB = luz->Attribute("diffB");
		if (str_diffB) diffB = atof(str_diffB);

		Luz l(type,posX, posY, posZ, ambR, ambG, ambB, diffR, diffG, diffB);

		switch (i) {
		case 0:
			glEnable(GL_LIGHT0);
			glLightfv(GL_LIGHT0, GL_AMBIENT, l.amb);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, l.diff);
			break;
		case 1:
			glEnable(GL_LIGHT1);
			glLightfv(GL_LIGHT1, GL_AMBIENT, l.amb);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, l.diff);
			break;
		case 2:
			glEnable(GL_LIGHT2);
			glLightfv(GL_LIGHT2, GL_AMBIENT, l.amb);
			glLightfv(GL_LIGHT2, GL_DIFFUSE, l.diff);
			break;
		case 3:
			glEnable(GL_LIGHT3);
			glLightfv(GL_LIGHT3, GL_AMBIENT, l.amb);
			glLightfv(GL_LIGHT3, GL_DIFFUSE, l.diff);
			break;
		case 4:
			glEnable(GL_LIGHT4);
			glLightfv(GL_LIGHT4, GL_AMBIENT, l.amb);
			glLightfv(GL_LIGHT4, GL_DIFFUSE, l.diff);
			break;
		case 5:
			glEnable(GL_LIGHT5);
			glLightfv(GL_LIGHT5, GL_AMBIENT, l.amb);
			glLightfv(GL_LIGHT5, GL_DIFFUSE, l.diff);
			break;
		case 6:
			glEnable(GL_LIGHT6);
			glLightfv(GL_LIGHT6, GL_AMBIENT, l.amb);
			glLightfv(GL_LIGHT6, GL_DIFFUSE, l.diff);
			break;
		case 7:
			glEnable(GL_LIGHT7);
			glLightfv(GL_LIGHT7, GL_AMBIENT, l.amb);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, l.diff);
			break;
		}
	}
}

/*
Le o ficheiro .xml, e guarda todos os modelos e respetivas transformacoes em memoria
*/
void prepareScene(TiXmlHandle doc) {
	TiXmlElement* scene = doc.FirstChild("scene").ToElement();
	TiXmlElement* group = scene->FirstChildElement("group");
	if (!group) return;

	TiXmlElement* lights = scene->FirstChildElement("lights");
	prepareLights(lights);

	numModelos = countModels(0, group);
	buffers = (GLuint*)malloc(sizeof(GLuint)*numModelos);
	glGenBuffers(numModelos, buffers);
	normals = (GLuint*)malloc(sizeof(GLuint)*numModelos);
	glGenBuffers(numModelos, normals);
	textures = (GLuint*)malloc(sizeof(GLuint)*numModelos);
	glGenBuffers(numModelos, textures);
	numVertices = (int*)malloc(sizeof(int)*numModelos);

	Transformacao transformacao = Transformacao();
	prepareGroup(group, transformacao);

}

void processMouseButtons(int button, int state, int xx, int yy)
{
	if (state == GLUT_DOWN) {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {

			r -= yy - startY;
			if (r < 3)
				r = 3.0;
		}
		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy)
{
	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {

		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux *							     sin(betaAux * 3.14 / 180.0);
}

int main(int argc, char **argv) {

	if (argc < 2) {
		cout << "Indique como argumento o path para o ficheiro .xml de configuracao\n";
		return 1;
	}

	TiXmlDocument doc(argv[1]);
	bool loadOkay = doc.LoadFile();
	if (!loadOkay) {
		cout << "Falha ao carregar o ficheiro \"" << argv[1] << "\"\n";
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
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	// put here the definition of the menu 
	glutCreateMenu(menuHandler);
	glutAddMenuEntry("Fill", 1);
	glutAddMenuEntry("Line", 2);
	glutAddMenuEntry("Point", 3);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnableClientState(GL_VERTEX_ARRAY);
	// ? glEnableClientState(GL_NORMAL_ARRAY); ?

	// Ler xml e preparar cena
	TiXmlHandle docHandle(&doc);
	prepareScene(docHandle);

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}