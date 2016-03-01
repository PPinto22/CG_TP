#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>

const double PI = 3.141592653589793238463;

using namespace std;

string doubleToString(double number) {
	ostringstream buff;
	buff << number;
	return buff.str();
}

class Ponto {
private: double xval, yval, zval;

public:
	Ponto(double x, double y, double z) {
		xval = x;
		yval = y;
		zval = z;
	}
	double x() { return xval; }
	double y() { return yval; }
	double z() { return zval; }

};

string escreveTriangulo(Ponto ponto1, Ponto ponto2, Ponto ponto3) {
	string triangulo = doubleToString(ponto1.x()) + " " + doubleToString(ponto1.y()) + " " + doubleToString(ponto1.z()) + "\n" +
		doubleToString(ponto2.x()) + " " + doubleToString(ponto2.y()) + " " + doubleToString(ponto2.z()) + "\n" +
		doubleToString(ponto3.x()) + " " + doubleToString(ponto3.y()) + " " + doubleToString(ponto3.z());
	return triangulo;
}

string escreveRectangulo(Ponto ponto1, Ponto ponto2, Ponto ponto3, Ponto ponto4) {
	string rectangulo = doubleToString(ponto1.x()) + " " + doubleToString(ponto1.y()) + " " + doubleToString(ponto1.z()) + "\n" +
		doubleToString(ponto2.x()) + " " + doubleToString(ponto2.y()) + " " + doubleToString(ponto2.z()) + "\n" +
		doubleToString(ponto3.x()) + " " + doubleToString(ponto3.y()) + " " + doubleToString(ponto3.z()) + "\n" +
		doubleToString(ponto3.x()) + " " + doubleToString(ponto3.y()) + " " + doubleToString(ponto3.z()) + "\n" +
		doubleToString(ponto4.x()) + " " + doubleToString(ponto4.y()) + " " + doubleToString(ponto4.z()) + "\n" +
		doubleToString(ponto1.x()) + " " + doubleToString(ponto1.y()) + " " + doubleToString(ponto1.z());
	return rectangulo;
}


void plane(double lado, string ficheiro) {
	string nVertices = ("6");
	double coord = lado / 2;

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

void box(double x, double y, double z, string ficheiro) {
	string nVertices = ("36");
	double coordX = x / 2;
	double coordY = y / 2;
	double coordZ = z / 2;

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

void sphere(double radius, int slices, int stacks, string ficheiro) {

	Ponto bot(0,-radius,0);
	Ponto top(0,radius,0);
	vector<vector<Ponto> > pontos;
	double x,y,z;

	for (double beta = -PI/2; beta<=PI/2; beta +=PI/stacks){
		vector<Ponto> aux;
		for(double alpha = 0; alpha<=2*PI; alpha +=2*PI/slices){
			z = radius*cos(beta)*cos(alpha);
			x = radius*cos(beta)*sin(alpha);
			y = radius*sin(beta);
			Ponto p (x,y,z);			
			aux.push_back(p);
		}
		pontos.push_back(aux);
	}

	ofstream outfile(ficheiro);
	int numVertices = slices*stacks*6; //Cada vertice pertence a 6 triangulos
	outfile << numVertices << endl;
	for(int stack = 0; stack<stacks; stack++){
		for(int slice = 0; slice<slices; slice++){
			int slice_esquerda;
			if (slice == 0) slice_esquerda = slices - 1;
			else slice_esquerda = slice - 1;
			int slice_direita = (slice + 1)%slices;

			outfile << pontos[stack][slice].x() << " " << pontos[stack][slice].y() << " " << pontos[stack][slice].z() << endl;
			outfile << pontos[stack + 1][slice].x() << " " << pontos[stack + 1][slice].y() << " " << pontos[stack + 1][slice].z() << endl;
			outfile << pontos[stack + 1][slice_esquerda].x() << " " << pontos[stack + 1][slice_esquerda].y() << " " << pontos[stack + 1][slice_esquerda].z() << endl;

			outfile << pontos[stack][slice].x() << " " << pontos[stack][slice].y() << " " << pontos[stack][slice].z() << endl;
			outfile << pontos[stack][slice_direita].x() << " " << pontos[stack][slice_direita].y() << " " << pontos[stack][slice_direita].z() << endl;
			outfile << pontos[stack+1][slice].x() << " " << pontos[stack+1][slice].y() << " " << pontos[stack+1][slice].z() << endl;
		}
	}
	outfile.close();

}

void cone(double bottomRadius, double height, int slices, int stacks, string ficheiro) {
	ofstream outfile(ficheiro);
	outfile << "Cone!" << endl;
	outfile.close();
}


int main(int argc, char *argv[]) {

	if (argc <= 2) {
		cout << "Indique a primitiva (plane,box,sphere,cone) e o ficheiro de sa�da\n";
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