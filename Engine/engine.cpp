#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
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
	Ponto(float* p) {
		xval = p[0];
		yval = p[1];
		zval = p[2];
	}
	float x() { return xval; }
	float y() { return yval; }
	float z() { return zval; }
	float* toArray() {
		float pontos[3];
		pontos[0] = xval;
		pontos[1] = yval;
		pontos[2] = zval;
		return pontos;
	}
};

class Rotacao {
public:
	int tempo; //milisegundos
	float x, y, z;

	Rotacao() {
		tempo = 0;
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	Rotacao(float segundos, float x, float y, float z) {
		this->tempo = (int) (segundos*1000);
		this->x = x;
		this->y = y;
		this->z = z;
		this->normalizar();
	}

	void normalizar() {
		float l = sqrt(x*x + y*y + z*z);
		x = x / l;
		y = y / l;
		z = z / l;
	}
};

class Translacao {
public:
	int tempo; //milisegundos
	vector<Ponto> pontos;

	Translacao() {
		this->tempo = 0;
		this->pontos = vector<Ponto>();
	}

	Translacao(float segundos, vector<Ponto> pontos) {
		this->tempo = (int) (segundos*1000);
		this->pontos = pontos;
	}

	// given  global t, returns the point in the curve
	Ponto getPoint(int milisegundos) {
		float gt = (float)(milisegundos % this->tempo) / this->tempo;
		float t = gt * pontos.size(); // this is the real global t
		int index = floor(t);  // which segment
		t = t - index; // where within  the segment

		int indices[4];
		indices[0] = (index + pontos.size() - 1) % pontos.size();
		indices[1] = (indices[0] + 1) % pontos.size();
		indices[2] = (indices[1] + 1) % pontos.size();
		indices[3] = (indices[2] + 1) % pontos.size();

		return getCatmullRomPoint(t, indices);
	}

private:
	//Catmull-rom matrix
	float M[4][4] = { { -0.5f,  1.5f, -1.5f,  0.5f },
					  { 1.0f, -2.5f,  2.0f, -0.5f  },
					  { -0.5f,  0.0f,  0.5f,  0.0f },
					  { 0.0f,  1.0f,  0.0f,  0.0f  } };

	Ponto getCatmullRomPoint(float t, int* indices) {

		float T[1][4] =  { { pow(t,3),pow(t,2),t,1.0f  } };

		float res[3];
		res[0] = 0.0; res[1] = 0.0; res[2] = 0.0;

		float P[4][3];
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++) {
				P[i][j] = pontos[indices[i]].toArray()[j];
			}
		}

		//Calcular T*M
		float TM[1][4] = { { 0.0,0.0,0.0,0.0 } };
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				TM[0][j] += T[0][k] * M[k][j];
			}
		}

		//Calcular T*M*P
		for (int xyz = 0; xyz < 3; xyz++) {
			for (int i = 0; i < 4; i++) {
				res[xyz] += TM[0][i] * P[i][xyz];
			}
		}

		return Ponto(res);
	}
};

class Transformacao {
public:
	Rotacao rotacao;
	vector<Translacao> translacoes;
	float sx, sy, sz;

	Transformacao() {
		this->rotacao = Rotacao();
		this->translacoes = vector<Translacao>();
		this->sx = 1.0f; this->sy = 1.0f; this->sz = 1.0f;
	}

	Transformacao(Rotacao rotacao, float sx, float sy, float sz) {
		this->rotacao = rotacao;
		this->translacoes = vector<Translacao>();
		this->sx = sx;
		this->sy = sy;
		this->sz = sz;
	}

	Transformacao(Rotacao rotacao, Translacao translacao, float sx, float sy, float sz) {
		this->rotacao = rotacao;
		this->translacoes = vector<Translacao>();
		this->translacoes.push_back(translacao);
		this->sx = sx;
		this->sy = sy;
		this->sz = sz;
	}

	void merge(Transformacao t) {
		// Mantem rotacao propria

		// Multiplica escalas
		this->sx *= t.sx; this->sy *= t.sy; this->sz *= t.sz;

		// Adquire translacoes de 't' e 
		// acrescenta as proprias no final
		vector<Translacao> translacoesAux;
		for (int i = 0; i < t.translacoes.size(); i++) {
			translacoesAux.push_back(t.translacoes[i]);
		}
		for (int i = 0; i < this->translacoes.size(); i++) {
			translacoesAux.push_back(this->translacoes[i]);
		}
		this->translacoes.clear();
		this->translacoes = translacoesAux;
	}
};

float camDistance = 300;
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
	float beta = nivelBeta * M_PI_2 / niveisBeta;
	float alpha = nivelAlpha * M_PI / niveisAlpha;
	float z = camDistance * cos(beta) * cos(alpha);
	float x = camDistance * cos(beta) * sin(alpha);
	float y = camDistance * sin(beta);
	int time;
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
		vector<Translacao> translacoes = transformacao.translacoes;
		for (int i = 0; i < translacoes.size(); i++) {
			Translacao translacao = translacoes[i];
			Ponto p = translacao.getPoint(glutGet(GLUT_ELAPSED_TIME));
			//printf("%f %f %f\n", p.x(), p.y(), p.z());
			glTranslatef(p.x(), p.y(), p.z());
		}

		glScalef(transformacao.sx, transformacao.sy, transformacao.sz);

		Rotacao rotacao = transformacao.rotacao;
		int tempo_rotacao = rotacao.tempo;
		if (tempo_rotacao > 0) {
			time = glutGet(GLUT_ELAPSED_TIME) % tempo_rotacao;
			float angulo = (float)time / tempo_rotacao * 360.0f;
			glRotatef(angulo, rotacao.x, rotacao.y, rotacao.z);
		}

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
		if (nivelBeta < niveisBeta - 1) nivelBeta++;
		break;
	case 's':case 'S':
		if (nivelBeta > -niveisBeta + 1) nivelBeta--;
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
Carrega todos os modelos existentes num grupo para 'buffers',
associando a cada um as transformacoes respetivas, guardando-as no 'vector<Transformacao> transformacoes'
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
		if(translacao_OK)
			transformacao = Transformacao(rotacao, translacao, sx, sy, sz);
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
				for (int i = 0; i < pontosFicheiro * 3; i++) {
					inFile >> coordenadas[i];
				}
				int indice = transformacoes.size();
				transformacoes.push_back(transformacao);
				numVertices[indice] = pontosFicheiro;
				glBindBuffer(GL_ARRAY_BUFFER, buffers[indice]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float)*pontosFicheiro * 3, coordenadas, GL_STATIC_DRAW);
				free(coordenadas);
			}
		}
		TiXmlElement* group_rec = group->FirstChildElement("group");
		prepareGroup(group_rec, transformacao);
	}
}

/*
Le o ficheiro .xml, e guarda todos os modelos e respetivas transformacoes em memoria
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
	glutKeyboardFunc(keyPressed);
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
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