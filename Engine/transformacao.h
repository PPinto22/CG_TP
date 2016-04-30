#ifndef _TRANSFORMACAO_
#define _TRANSFORMACAO_

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

#endif