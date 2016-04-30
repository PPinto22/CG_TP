#ifndef _ROTACAO_
#define _ROTACAO__

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

#endif