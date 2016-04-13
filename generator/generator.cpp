#define _USE_MATH_DEFINES
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>

using namespace std;

string floatToString(float number) {
	ostringstream buff;
	buff << number;
	return buff.str();
}

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

class Patch {
private: int[] indices;
		 Ponto[] pontos;

public:
	Patch(){
		this->indices = new int[16];
		this->pontos = new Ponto[16];
	}
	Patch(int[] ind){
		this->indices = ind;
		this->pontos = new Ponto[16];
	}
	int[] getIndices(){
		return this.indices;
	}
	Ponto[] getPontos(){
		return this->pontos;
	}
	int[] getIndices(){
		return this->indices;
	}
	void setIndices(int[] ind){
		this->indices = ind;
	}
	void preenche(vector<Ponto> pts){
		for(int i = 0;i<16;i++){
			this->pontos[i]=pts[indices[i]];
		}
	}
};

class PatchSet {
private: vector<Patch> patches;
		 vector<Ponto> pontos;
public:
	PatchSet(){
		this->patches = new vector<Patch>();
		this->pontos = new vector<Ponto>();
	}
	PatchSet(vector<Patch> p,vector<Ponto> pts){
		this->patches = p;
		this->pontos = pts;
	}
	vector<Patch> getPatches(){
		return this.patches;
	}
	vector<Patch> getPontos(){
		return this.pontos;
	}
	void setPatches(vector<Patch> p){
		this.patches = p;
	}
	void setPontos(vector<Patch> p){
		this.pontos = p;
	}
	void addPatch(Patch p){
		this.patches.push_back(p);
	}
	void addPonto(Ponto p){
		this.pontos.push_back(p);
	}
	void preenche(){
		for(int i = 0;patches->size();i++){
			patches[i]->preenche(pontos);
		}
	}


};


public:
	Patch(vector<Ponto> p) {
		this.pontos = p;
	}

	vector<Ponto> getPontos() {
		return this.pontos;
	}
};

bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

void readBezier(string s,int i){
	ifstream file;


}

string escreveTriangulo(Ponto ponto1, Ponto ponto2, Ponto ponto3) {
	string triangulo = floatToString(ponto1.x()) + " " + floatToString(ponto1.y()) + " " + floatToString(ponto1.z()) + "\n" +
		floatToString(ponto2.x()) + " " + floatToString(ponto2.y()) + " " + floatToString(ponto2.z()) + "\n" +
		floatToString(ponto3.x()) + " " + floatToString(ponto3.y()) + " " + floatToString(ponto3.z());
	return triangulo;
}

string escreveRectangulo(Ponto ponto1, Ponto ponto2, Ponto ponto3, Ponto ponto4) {
	string rectangulo = floatToString(ponto1.x()) + " " + floatToString(ponto1.y()) + " " + floatToString(ponto1.z()) + "\n" +
		floatToString(ponto2.x()) + " " + floatToString(ponto2.y()) + " " + floatToString(ponto2.z()) + "\n" +
		floatToString(ponto3.x()) + " " + floatToString(ponto3.y()) + " " + floatToString(ponto3.z()) + "\n" +
		floatToString(ponto3.x()) + " " + floatToString(ponto3.y()) + " " + floatToString(ponto3.z()) + "\n" +
		floatToString(ponto4.x()) + " " + floatToString(ponto4.y()) + " " + floatToString(ponto4.z()) + "\n" +
		floatToString(ponto1.x()) + " " + floatToString(ponto1.y()) + " " + floatToString(ponto1.z());
	return rectangulo;
}


void plane(float lado, string ficheiro) {
	string nVertices = ("6");
	float coord = lado / 2;

	Ponto pt1(coord, 0, coord);
	Ponto pt2(coord, 0, -coord);
	Ponto pt3(-coord, 0, -coord);
	Ponto pt4(-coord, 0, coord);



	ofstream outfile(ficheiro);

	outfile << nVertices << endl;
	outfile << escreveTriangulo(pt1, pt2, pt3) << endl;
	outfile << escreveTriangulo(pt3, pt4, pt1);

	outfile.close();
}

void box(float x, float y, float z, string ficheiro) {
	string nVertices = ("36");
	float coordX = x / 2;
	float coordY = y / 2;
	float coordZ = z / 2;

	Ponto pt1(coordX, -coordY, -coordZ);
	Ponto pt2(-coordX, -coordY, -coordZ);
	Ponto pt3(-coordX, -coordY, coordZ);
	Ponto pt4(coordX, -coordY, coordZ);
	Ponto pt5(coordX, coordY, -coordZ);
	Ponto pt6(-coordX, coordY, -coordZ);
	Ponto pt7(-coordX, coordY, coordZ);
	Ponto pt8(coordX, coordY, coordZ);

	ofstream outfile(ficheiro);

	outfile << nVertices << endl;
	outfile << escreveRectangulo(pt4, pt3, pt2, pt1) << endl;
	outfile << escreveRectangulo(pt5, pt6, pt7, pt8) << endl;
	outfile << escreveRectangulo(pt1, pt5, pt8, pt4) << endl;
	outfile << escreveRectangulo(pt3, pt7, pt6, pt2) << endl;
	outfile << escreveRectangulo(pt4, pt8, pt7, pt3) << endl;
	outfile << escreveRectangulo(pt2, pt6, pt5, pt1);

	outfile.close();
}

void sphere(float radius, int slices, int stacks, string ficheiro) {

	vector<vector<Ponto> > pontos;
	float x,y,z;
	Ponto topo(0, radius, 0);

	float beta;
	float alpha;
	//Gera pontos
	for (int i = 0; i<=stacks; i++){
		beta = (i*M_PI / stacks)-M_PI/2;
		vector<Ponto> aux;
		float alpha = 0;
		for(int j = 0; j<slices; j++){
			alpha = j * 2 * M_PI / slices;
			z = radius*cos(beta)*cos(alpha);
			x = radius*cos(beta)*sin(alpha);
			y = radius*sin(beta);
			Ponto p (x,y,z);			
			aux.push_back(p);
		}
		pontos.push_back(aux);
	}

	ofstream outfile(ficheiro);
	int numVertices = stacks*slices * 6;
	outfile << numVertices << endl;

	//Escreve triangulos
	for(int stack = 0; stack<stacks; stack++){
		for(int slice = 0; slice<slices; slice++){
			int slice_direita = (slice + 1)%slices;

			outfile << pontos[stack][slice].x() << " " << pontos[stack][slice].y() << " " << pontos[stack][slice].z() << endl;
			outfile << pontos[stack+1][slice_direita].x() << " " << pontos[stack+1][slice_direita].y() << " " << pontos[stack+1][slice_direita].z() << endl;
			outfile << pontos[stack + 1][slice].x() << " " << pontos[stack + 1][slice].y() << " " << pontos[stack + 1][slice].z() << endl;

			outfile << pontos[stack][slice].x() << " " << pontos[stack][slice].y() << " " << pontos[stack][slice].z() << endl;
			outfile << pontos[stack][slice_direita].x() << " " << pontos[stack][slice_direita].y() << " " << pontos[stack][slice_direita].z() << endl;
			outfile << pontos[stack+1][slice_direita].x() << " " << pontos[stack+1][slice_direita].y() << " " << pontos[stack+1][slice_direita].z() << endl;

		}
	}
	outfile.close();

}

void cone(float bottomRadius, float height, int slices, int stacks, string ficheiro) {

	Ponto pBot(0,-height/2,0);
	vector<vector<Ponto> > pontos;
	float raio = bottomRadius;
	float x, z, y, alpha;

	//Gera pontos
	for (int i = 0; i <= stacks; i++){
		y = (i*height / stacks) - (height / 2);
		vector<Ponto> aux;
		for (int j = 0; j < slices; j++){
			alpha = j * 2 * M_PI / slices;
			z = raio*sin(alpha);
			x = raio*cos(alpha);
			Ponto p (x,y,z);			
			aux.push_back(p);
		}
		pontos.push_back(aux);
		raio-=bottomRadius/stacks;
	}

	ofstream outfile(ficheiro);
	int numVertices = slices*stacks * 6 + 3 * slices;
	outfile << numVertices << endl;

	//Escreve triangulos laterais
	for(int stack = 0; stack<stacks; stack++){
		for(int slice = 0; slice<slices; slice++){
			int slice_direita = (slice + 1)%slices;

			outfile << pontos[stack][slice].x() << " " << pontos[stack][slice].y() << " " << pontos[stack][slice].z() << endl;
			outfile << pontos[stack + 1][slice_direita].x() << " " << pontos[stack + 1][slice_direita].y() << " " << pontos[stack + 1][slice_direita].z() << endl;
			outfile << pontos[stack + 1][slice].x() << " " << pontos[stack + 1][slice].y() << " " << pontos[stack + 1][slice].z() << endl;

			outfile << pontos[stack][slice].x() << " " << pontos[stack][slice].y() << " " << pontos[stack][slice].z() << endl;
			outfile << pontos[stack][slice_direita].x() << " " << pontos[stack][slice_direita].y() << " " << pontos[stack][slice_direita].z() << endl;
			outfile << pontos[stack + 1][slice_direita].x() << " " << pontos[stack + 1][slice_direita].y() << " " << pontos[stack + 1][slice_direita].z() << endl;
		}
	}

	//Escreve triangulos da base
	for(int slice = 0; slice<slices; slice++){
		int slice_direita=(slice+1)%slices;
		outfile << pontos[0][slice].x() << " " << pontos[0][slice].y() << " " << pontos[0][slice].z() << endl;
		outfile << pBot.x() << " " << pBot.y() << " " << pBot.z() << endl;
		outfile << pontos[0][slice_direita].x() << " " << pontos[0][slice_direita].y() << " " << pontos[0][slice_direita].z() << endl;
	}

	outfile.close();

}

int main(int argc, char *argv[]) {

	if (argc <= 2) {
		cout << "Indique a primitiva (plane,box,sphere,cone) e o ficheiro de saida\n";
		return 1;
	}

	else if(hasEnding(argv[1],".patch")){
			cout << "teste\n";
			readBezier(argv[1],argv[2]);
		}

	else if (strcmp(argv[1], "plane") == 0) {
		if (argc == 4) {
			plane(atof(argv[2]), argv[3]);
		}
		else {
			cout << "Indicou mal os argumentos\n";
			return 1;
		}
	}
	else if (strcmp(argv[1], "box") == 0) {
		if (argc == 6) {
			box(atof(argv[2]), atof(argv[3]), atof(argv[4]), argv[5]);
		}
		else {
			cout << "Indicou mal os argumentos\n";
			return 1;
		}
	}
	else if (strcmp(argv[1], "sphere") == 0) {
		if (argc == 6) {
			sphere(atof(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5]);
		}
		else {
			cout << "Indicou mal os argumentos\n";
			return 1;
		}
	}
	else if (strcmp(argv[1], "cone") == 0) {
		if (argc == 7) {
			cone(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]), argv[6]);
		}
		else {
			cout << "Indicou mal os argumentos\n";
			return 1;
		}
	}
	else {
		cout << "Argumentos mal indicados\n";
		return 1;
	}
	cout << "Sucesso!\n";
	return 1;


}