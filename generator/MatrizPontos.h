#ifndef _MATRIZPONTOS_
#define _MATRIZPONTOS_

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

#endif