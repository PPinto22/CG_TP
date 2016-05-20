#ifndef _PATCH_
#define _PATCH_

class Patch {
private: int* indices;
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

		Ponto result = resultado.getPonto(0,0);
		result.setTextXY(coluna,1.0-linha);
		return result;

	}
	Ponto* getVertices(int tesselation){
		float intervalo = 1.0/(3*tesselation);
		Ponto* vertices = (Ponto*)malloc(sizeof(Ponto)*(3*tesselation*3*tesselation*6));

		int count = 0;
		for(int i = 0;i<3*tesselation;i++){
			float u = i*(1/(3.0*tesselation));
			for(int j = 0;j<3*tesselation;j++){
				float v = j*(1/(3.0*tesselation));
				
				Ponto p = this->getPonto(u,v);
				Ponto q = this->getPonto(u+intervalo,v);
				Ponto r = this->getPonto(u,v+intervalo);
				Ponto s = this->getPonto(u+intervalo,v+intervalo);

				vertices[count] = p;
				count++;
				vertices[count] = r;
				count++;
				vertices[count] = q;
				count++;

				vertices[count] = r;
				count++;
				vertices[count] = s;
				count++;
				vertices[count] = q;
				count++;
				
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

#endif