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
	Ponto(){
		this->xval = 0;
		this->yval = 0;
		this->zval = 0;
	}
	Ponto(float x, float y, float z) {
		this->xval = x;
		this->yval = y;
		this->zval = z;
	}
	void setX(float x){
		this->xval = x;
	}
	void setY(float y){
		this->yval = y;
	}
	void setZ(float z){
		this->zval = z;
	}
	float x() { return xval; }
	float y() { return yval; }
	float z() { return zval; }
	void multiplica(Ponto pt){
		this->xval *= pt.x();
		this->yval *= pt.y();
		this->zval *= pt.z();
	}
	void soma(Ponto pt){
		this->xval += pt.x();
		this->yval += pt.y();
		this->zval += pt.z();
	}
	void toString(){
		printf("X = %f, Y = %f, Z = %f;\n",this->xval,this->yval,this->zval);
	}

};

class MatrizPontos {
private: Ponto** matriz;
		 int linhas;
		 int colunas;

public:
	MatrizPontos(){
		this->matriz = NULL;
	}
	MatrizPontos(int linhas, int colunas){
		this->matriz = (Ponto**)malloc(sizeof(Ponto*)*linhas);
		for(int i = 0;i<linhas;i++){
			this->matriz[i]=(Ponto*)malloc(sizeof(Ponto)*colunas);
		}
		this->linhas = linhas;
		this->colunas = colunas;
	}
	MatrizPontos(Ponto** m, int linhas, int colunas){
		this->matriz = m;
		this->linhas = linhas;
		this->colunas = colunas;
	}
	int getLinhas(){
		return this->linhas;
	}
	int getColunas(){
		return this->colunas;
	}
	Ponto getPonto(int linha,int coluna){
		Ponto p = this->matriz[linha][coluna];
		return p;
	}
	Ponto** getMatriz(){
		return this->matriz;
	}

	void add(Ponto p,int linha,int coluna){
		this->matriz[linha][coluna] = p;
	}
	

	MatrizPontos multiplica(MatrizPontos m){
		int l2 = m.getLinhas();
		int c2 = m.getColunas();
		Ponto value = Ponto();

		if(this->colunas==l2){
			MatrizPontos resultado = MatrizPontos(this->linhas,c2);

			for(int i=0; i<this->linhas; ++i)
	    	for(int j=0; j<c2; ++j){
	    		value = Ponto();
	    		for(int k=0; k<this->colunas; ++k){
	    			Ponto aux = this->getPonto(i,k);
	    			aux.multiplica(m.getPonto(k,j));
	    			value.soma(aux);

	    		}
	    		resultado.add(value,i,j);
	    	}
	    	return resultado;
		}
		else return MatrizPontos();
	}
	
};

class Patch {
public: int* indices;
		 MatrizPontos P;
		 MatrizPontos M;

		void initM(){
			Ponto** pts = (Ponto**)malloc(sizeof(Ponto*)*4);
			for(int i = 0;i<4;i++){
				pts[i] = (Ponto*)malloc(sizeof(Ponto)*4);
			}

			pts[0][0] = Ponto(-1,-1,-1);
			pts[0][1] = Ponto(3,3,3);
			pts[0][2] = Ponto(-3,-3,-3);
			pts[0][3] = Ponto(1,1,1);
			pts[1][0] = Ponto(3,3,3);
			pts[1][1] = Ponto(-6,-6,-6);
			pts[1][2] = Ponto(3,3,3);
			pts[1][3] = Ponto(0,0,0);
			pts[2][0] = Ponto(-3,-3,-3);
			pts[2][1] = Ponto(3,3,3);
			pts[2][2] = Ponto(0,0,0);
			pts[2][3] = Ponto(0,0,0);
			pts[3][0] = Ponto(1,1,1);
			pts[3][1] = Ponto(0,0,0);
			pts[3][2] = Ponto(0,0,0);
			pts[3][3] = Ponto(0,0,0);

			this->M = MatrizPontos(pts,4,4);
		}
		MatrizPontos getU(float u){
			Ponto** pts = (Ponto**)malloc(sizeof(Ponto*)*1);
			pts[0] = (Ponto*)malloc(sizeof(Ponto)*4);

			pts[0][0] = Ponto(pow(u,3),pow(u,3),pow(u,3));
			pts[0][1] = Ponto(pow(u,2),pow(u,2),pow(u,2));
			pts[0][2] = Ponto(u,u,u);
			pts[0][3] = Ponto(1,1,1);
			
			return MatrizPontos(pts,1,4);
		}
		MatrizPontos getV(float v){
			Ponto** pts = (Ponto**)malloc(sizeof(Ponto*)*4);
			for(int i = 0;i<4;i++){
				pts[i] = (Ponto*)malloc(sizeof(Ponto)*1);
			}
			pts[0][0] = Ponto(pow(v,3),pow(v,3),pow(v,3));
			pts[1][0] = Ponto(pow(v,2),pow(v,2),pow(v,2));
			pts[2][0] = Ponto(v,v,v);
			pts[3][0] = Ponto(1,1,1);
			
			return MatrizPontos(pts,4,1);
		}


public:
	Patch(){
		this->indices = (int*)malloc(sizeof(int)*16);
		this->P = MatrizPontos(4,4);
		initM();
	}
	Patch(int* ind){
		this->indices = ind;
		this->P = MatrizPontos(4,4);
		initM();
	}
	int* getIndices(){
		return this->indices;
	}
	void preenche(vector<Ponto> pts){
		for (int i = 0;i<4;i++){
			for (int j = 0;j<4;j++){
				Ponto p = pts[this->indices[i*4+j]];
				this->P.add(p,i,j);
			}
		}
	}
	void setIndices(int ind[]){
		this->indices = ind;
	}
	Ponto getPonto(float linha,float coluna){
		MatrizPontos U = getU(linha);
		MatrizPontos V = getV(coluna);
		MatrizPontos resultado = U.multiplica(this->M).multiplica(this->P).multiplica(this->M).multiplica(V);


		return resultado.getPonto(0,0);

	}
	Ponto* getVertices(int tesselation){
		float intervalo = 1.0/tesselation;

		Ponto* vertices = (Ponto*)malloc(sizeof(Ponto)*(3*tesselation*3*tesselation*6));
		initM();
		int count = 0;
		for(int i = 0;i<3*tesselation;i++){
			float u = i*intervalo;
			for(int j = 0;j<3*tesselation;j++){
				float v = j*intervalo;
				Ponto p = this->getPonto(u,v);
				Ponto q = this->getPonto(u+intervalo,v);
				Ponto r = this->getPonto(u,v+intervalo);
				Ponto s = this->getPonto(u+intervalo,v+intervalo);

				vertices[count++] = p;
				vertices[count++] = q;
				vertices[count++] = r;

				vertices[count++] = r;
				vertices[count++] = q;
				vertices[count++] = s;

				/*
				p_______r
				|		|
				|		|
			   q|_______|s

				*/ 
			}
		}
		return vertices;
	}
	
};

class PatchSet {
private: vector<Patch> patches;
		 vector<Ponto> pontos;
public:
	PatchSet(){
		patches = vector<Patch>();
		pontos = vector<Ponto>();
	}
	PatchSet(vector<Patch> p,vector<Ponto> pts){
		this->patches = p;
		this->pontos = pts;
	}
	vector<Patch> getPatches(){
		return this->patches;
	}
	vector<Ponto> getPontos(){
		return this->pontos;
	}
	void setPatches(vector<Patch> p){
		this->patches = p;
	}
	void setPontos(vector<Ponto> p){
		this->pontos = p;
	}
	void addPatch(Patch p){
		this->patches.push_back(p);
	}
	void addPonto(Ponto p){
		this->pontos.push_back(p);
	}
	void preenche(){
		for(int i = 0;patches.size();i++){
			patches[i].preenche(pontos);
		}
	}
	void toString(){
		for(int i = 0;i<this->pontos.size(); i++){
			pontos[i].toString();
		}
	}
	int nrPatches(){
		return this->patches.size();
	}
	int nrPontos(){
		return this->pontos.size();
	}
	

};



bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
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


string removeVirgulas(string s){
	for (string::iterator it = s.begin(); it != s.end(); ++it) {
    if (*it == ',') {
        s.erase(it);
    }
    else continue;
}
return s;
}

PatchSet readBezier(string ficheiro){
	//int* patch = (int*)malloc(sizeof(int)*16);
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

	//Check
	vector<Ponto> pontos = ps.getPontos();
	//Check
	vector<Patch> patches = ps.getPatches();

	
	for(int i = 0;i<ps.nrPatches();i++){
		//check
		patches[i].preenche(pontos);

			/*printf("Matriz P do patch %d no ponto 0,0, X = %f, Y = %f, Z = %f\n",i,patches[i].P.getPonto(0,0).x(),patches[i].P.getPonto(0,0).y(),patches[i].P.getPonto(0,0).z());
			printf("Matriz P do patch %d no ponto 0,1, X = %f, Y = %f, Z = %f\n",i,patches[i].P.getPonto(0,1).x(),patches[i].P.getPonto(0,1).y(),patches[i].P.getPonto(0,1).z());
			printf("Matriz P no ponto = %f\n",patches[i].P.getPonto(0,2).x());
			printf("Matriz P no ponto = %f\n",patches[i].P.getPonto(0,3).x());

			printf("Matriz P no ponto = %f\n",patches[i].P.getPonto(1,0).x());
			printf("Matriz P no ponto = %f\n",patches[i].P.getPonto(1,1).x());
			printf("Matriz P no ponto = %f\n",patches[i].P.getPonto(1,2).x());
			printf("Matriz P no ponto = %f\n",patches[i].P.getPonto(1,3).x());

			printf("Matriz P no ponto = %f\n",patches[i].P.getPonto(2,0).x());
			printf("Matriz P no ponto = %f\n",patches[i].P.getPonto(2,1).x());
			printf("Matriz P no ponto = %f\n",patches[i].P.getPonto(2,2).x());
			printf("Matriz P no ponto = %f\n",patches[i].P.getPonto(2,3).x());

			printf("Matriz P no ponto = %f\n",patches[i].P.getPonto(3,0).x());
			printf("Matriz P no ponto = %f\n",patches[i].P.getPonto(3,1).x());
			printf("Matriz P no ponto = %f\n",patches[i].P.getPonto(3,2).x());
			printf("Matriz P no ponto = %f\n",patches[i].P.getPonto(3,3).x());*/

		Ponto* pts = patches[i].getVertices(tesselation);
		
		for(int j = 0;j<(4*tesselation)*(4*tesselation);j+=3){
			outfile << pts[j].x() << " " << pts[j].y() << " " << pts[j].z() << endl;
			outfile << pts[j+1].x() << " " << pts[j+1].y() << " " << pts[j+1].z() << endl;
			outfile << pts[j+2].x() << " " << pts[j+2].y() << " " << pts[j+2].z() << endl;
		}
	}

}



int main(int argc, char *argv[]) {

	/*MatrizPontos m = MatrizPontos(2,4);
	Ponto p1 = Ponto(4,2,3);
	Ponto p2 = Ponto(4,2,3);
	Ponto p3 = Ponto(4,2,3);
	Ponto p4 = Ponto(4,2,3);
	Ponto p5 = Ponto(1,2,3);
	Ponto p6 = Ponto(1,2,3);
	Ponto p7 = Ponto(1,2,3);
	Ponto p8 = Ponto(1,2,3);
	m.add(p1,0,0);
	m.add(p2,0,1);
	m.add(p3,0,2);
	m.add(p4,0,3);
	m.add(p5,1,0);
	m.add(p6,1,1);
	m.add(p7,1,2);
	m.add(p8,1,36);

	MatrizPontos m2 = MatrizPontos(4,2);
	Ponto q1 = Ponto(1,2,3);
	Ponto q2 = Ponto(1,2,3);
	Ponto q3 = Ponto(1,2,3);
	Ponto q4 = Ponto(1,2,3);
	Ponto q5 = Ponto(1,2,3);
	Ponto q6 = Ponto(1,2,3);
	Ponto q7 = Ponto(1,2,3);
	Ponto q8 = Ponto(1,2,3);
	m2.add(q1,0,0);
	m2.add(q2,0,1);
	m2.add(q3,1,0);
	m2.add(q4,1,1);
	m2.add(q5,2,0);
	m2.add(q6,2,1);
	m2.add(q7,3,0);
	m2.add(q8,3,1);
	printf("teste\n");
	MatrizPontos resultado = m.multiplica(m2);
	printf("teste\n");
	printf("Resultado = %f\n",resultado.getPonto(0,0).x());*/

	if (argc <= 2) {
		cout << "Indique a primitiva (plane,box,sphere,cone), ou ficheiro .patch e o ficheiro de saida\n";
		return 1;
	}

	else if(hasEnding(argv[1],".patch")){
			cout << "teste\n";
			bezier("teapot.patch",2,"pinto.txt");
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