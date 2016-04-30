#define _USE_MATH_DEFINES
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <vector>
#include "ponto.h"
#include "funcAux.h"
#include "MatrizPontos.h"
#include "patch.h"
#include "patchSet.h"


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


PatchSet readBezier(string ficheiro){
	Patch p;
	PatchSet ps = PatchSet();

	char c;
	int count = 0,numPatches,numPontos,n;
	string line,line1,line2,s;
	ifstream myReadFile;
	
	myReadFile.open(ficheiro);
	if(myReadFile.is_open())
    {
    	getline(myReadFile,line1);
    	numPatches = atoi(line1.c_str());
    	
    	for(int i = 0;i<numPatches;i++){
            getline(myReadFile,line);
            s = removeVirgulas(line);
           	int* patch = (int*)malloc(sizeof(int)*16);
            stringstream stream(s);
			while(stream>>n){
				stream.clear();
				if(stream){
				patch[count] = n;
				count++;
				}
				
			}
			count = 0;
			
			p = Patch(patch);
			ps.addPatch(p);
		}
		
		getline(myReadFile,line2);
		numPontos = atoi(line2.c_str());

		for(int i = 0;i<numPontos;i++){
            getline(myReadFile,line);
            s = removeVirgulas(line);

           	stringstream stream(s);

           	Ponto pt = Ponto();
			float coord;
			count = 0;
			while(stream>>coord){
				if (count == 0){
					pt.setX(coord);
				}
				if (count == 1){
					pt.setY(coord);
				}
				if (count == 2){
					pt.setZ(coord);
				}
				count++;
			}
			count = 0;
			ps.addPonto(pt);
		}

    }
    myReadFile.close();

    return ps;
}

void bezier(string input,int tesselation, string output){
	ofstream outfile(output);
	PatchSet ps = readBezier(input);

	vector<Ponto> pontos = ps.getPontos();
	
	vector<Patch> patches = ps.getPatches();
	

	outfile << 3*tesselation*3*tesselation*6*ps.nrPatches() << endl;
	for(int i = 0;i<ps.nrPatches();i++){

		patches[i].preenche(pontos);

		Ponto* pts = patches[i].getVertices(tesselation);
		
		for(int j = 0;j<(3*tesselation)*(3*tesselation)*6;j+=3){
			outfile << pts[j].x() << " " << pts[j].y() << " " << pts[j].z() << endl;
			outfile << pts[j+1].x() << " " << pts[j+1].y() << " " << pts[j+1].z() << endl;
			outfile << pts[j+2].x() << " " << pts[j+2].y() << " " << pts[j+2].z() << endl;
		}
	}
}



int main(int argc, char *argv[]) {

	if (argc <= 2) {
		cout << "Indique a primitiva (plane,box,sphere,cone), ou ficheiro .patch com o numero de tesselation desejado e o nome do ficheiro de saída.\n";
		return 1;
	}

	else if(hasEnding(argv[1],".patch")){
		if (argc == 4){
			bezier(argv[1],atoi(argv[2]),argv[3]);
		}
		else{
			cout << "Indicou mal os argumentos\n";
			return 1;
		}
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