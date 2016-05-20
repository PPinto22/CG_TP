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

string escreveTriangulo(Ponto ponto1, Ponto ponto2, Ponto ponto3, Ponto normal, Ponto t1, Ponto t2, Ponto t3) {
    string triangulo = ponto1.toString() + " " + normal.toString() + " " + t1.toString2d() + "\n" +
					   ponto2.toString() + " " + normal.toString() + " " + t2.toString2d() + "\n" +
					   ponto3.toString() + " " + normal.toString() + " " + t3.toString2d();
    return triangulo;
}

string escreveRectangulo(Ponto ponto1, Ponto ponto2, Ponto ponto3, Ponto ponto4, Ponto normal, Ponto c1, Ponto c2, Ponto c3, Ponto c4) {
    string rectangulo = ponto1.toString() + " " + normal.toString() + " " + c1.toString2d() + "\n" +
						ponto2.toString() + " " + normal.toString() + " " + c2.toString2d() + "\n" +
						ponto3.toString() + " " + normal.toString() + " " + c3.toString2d() + "\n" +
						ponto3.toString() + " " + normal.toString() + " " + c3.toString2d() + "\n" +
						ponto4.toString() + " " + normal.toString() + " " + c4.toString2d() + "\n" +
						ponto1.toString() + " " + normal.toString() + " " + c1.toString2d();
    return rectangulo;
}





#endif