#ifndef _TRASLACAO_
#define _TRASLACAO_

class Translacao {
public:
	int tempo; //milisegundos
	vector<Ponto> pontos;
	float up[3] = { 0.0f, 1.0f, 0.0f };

	Translacao() {
		this->tempo = 0;
		this->pontos = vector<Ponto>();
	}

	Translacao(float segundos, vector<Ponto> pontos) {
		this->tempo = (int) (segundos*1000);
		this->pontos = pontos;
	}
	
	// given  global t, returns the point in the curve
	Ponto getPoint(float gt) {

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

	Ponto getPoint(int milisegundos) {
		float gt = (float)(milisegundos % this->tempo) / this->tempo;
		float t = gt * pontos.size();
		int index = floor(t);
		t = t - index;

		int indices[4];
		indices[0] = (index + pontos.size() - 1) % pontos.size();
		indices[1] = (indices[0] + 1) % pontos.size();
		indices[2] = (indices[1] + 1) % pontos.size();
		indices[3] = (indices[2] + 1) % pontos.size();

		return getCatmullRomPoint(t, indices);
	}

	float* getRotMatrix(int milisegundos) {
		float left[3];
		float* m = (float*)malloc(16*sizeof(float));
		float gt = (float)(milisegundos % this->tempo) / this->tempo;
		float t = gt * pontos.size(); // this is the real global t
		int index = floor(t);  // which segment
		t = t - index; // where within  the segment

		int indices[4];
		indices[0] = (index + pontos.size() - 1) % pontos.size();
		indices[1] = (indices[0] + 1) % pontos.size();
		indices[2] = (indices[1] + 1) % pontos.size();
		indices[3] = (indices[2] + 1) % pontos.size();

		float* dir = getCatmullRomDirection(t, indices); normalize(dir);
		cross(up, dir, left); normalize(left);
		cross(dir, left, up); normalize(up);

		buildRotMatrix(dir, up, left, m);
		free(dir);

		return m;
	}

private:
	//Catmull-rom matrix
	float M[4][4] = { { -0.5f,  1.5f, -1.5f,  0.5f },
					  { 1.0f, -2.5f,  2.0f, -0.5f  },
					  { -0.5f,  0.0f,  0.5f,  0.0f },
					  { 0.0f,  1.0f,  0.0f,  0.0f  } };

	void buildRotMatrix(float *x, float *y, float *z, float *m) {
		m[0] = x[0];
		m[1] = x[1];
		m[2] = x[2];
		m[3] = 0.0f;
		m[4] = y[0];
		m[5] = y[1];
		m[6] = y[2];
		m[7] = 0.0f;
		m[8] = z[0];
		m[9] = z[1];
		m[10] = z[2];
		m[11] = 0.0f;
		m[12] = 0.0f;
		m[13] = 0.0f;
		m[14] = 0.0f;
		m[15] = 1.0f;
	}

	void cross(float *a, float *b, float *res) {
		res[0] = a[1] * b[2] - a[2] * b[1]; res[1] = a[2] * b[0] - a[0] * b[2]; res[2] = a[0] * b[1] - a[1] * b[0];
	}

	void normalize(float *a) {
		float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
		a[0] = a[0] / l;
		a[1] = a[1] / l;
		a[2] = a[2] / l;
	}
	
	float* getCatmullRomDirection(float t, int* indices) {
		float* dir = (float*)malloc(3 * sizeof(float));
		dir[0] = 0.0f; dir[1] = 0.0f; dir[2] = 0.0f;


		float Td[1][4] = { { 3 * pow(t,2), 2 * t, 1.0f, 0.0f } };

		float P[4][3];
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++) {
				P[i][j] = pontos[indices[i]].toArray()[j];
			}
		}

		//Calcular Td*M
		float TdM[1][4] = { { 0.0f,0.0f,0.0f,0.0f } };
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				TdM[0][j] += Td[0][k] * M[k][j];
			}
		}

		//Calcular Td*M*P
		for (int xyz = 0; xyz < 3; xyz++) {
			for (int i = 0; i < 4; i++) {
				dir[xyz] += TdM[0][i] * P[i][xyz];
			}
		}

		return dir;
	}

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

#endif