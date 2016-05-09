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

string escreveTriangulo(Ponto ponto1, Ponto ponto2, Ponto ponto3, Ponto normal) {
    string triangulo = ponto1.toString() + " " + normal.toString() + "\n" +
					   ponto2.toString() + " " + normal.toString() + "\n" +
					   ponto3.toString() + " " + normal.toString();
    return triangulo;
}

string escreveRectangulo(Ponto ponto1, Ponto ponto2, Ponto ponto3, Ponto ponto4, Ponto normal) {
    string rectangulo = ponto1.toString() + " " + normal.toString() + "\n" +
						ponto2.toString() + " " + normal.toString() + "\n" +
						ponto3.toString() + " " + normal.toString() + "\n" +
						ponto3.toString() + " " + normal.toString() + "\n" +
						ponto4.toString() + " " + normal.toString() + "\n" +
						ponto1.toString() + " " + normal.toString();
    return rectangulo;
}





#endif