#ifndef _FUNCAUX_
#define _FUNCAUX_

using namespace std;

string floatToString(float number) {
	ostringstream buff;
	buff << number;
	return buff.str();
}

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

#endif