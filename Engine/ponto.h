#ifndef _PONTO_
#define _PONTO_

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

#endif