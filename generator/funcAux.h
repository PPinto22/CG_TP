#ifndef _FUNCAUX_
#define _FUNCAUX_

using namespace std;

bool hasEnding (string const &fullString, string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
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

string escreveTriangulo(Ponto ponto1, Ponto ponto2, Ponto ponto3) {
    string triangulo = ponto1.toString() + "\n" +
					   ponto2.toString() + "\n" +
					   ponto3.toString();
    return triangulo;
}

string escreveRectangulo(Ponto ponto1, Ponto ponto2, Ponto ponto3, Ponto ponto4) {
    string rectangulo = ponto1.toString() + "\n" +
						ponto2.toString() + "\n" +
						ponto3.toString() + "\n" +
						ponto3.toString() + "\n" +
						ponto4.toString() + "\n" +
						ponto1.toString();
    return rectangulo;
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

#endif