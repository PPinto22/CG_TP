using namespace std;

#ifndef _PONTO_
#define _PONTO_


class Ponto {
private: float xval, yval, zval, textXx, textYy;

public:
	Ponto(){
		this->xval = 0.0;
		this->yval = 0.0;
		this->zval = 0.0;
	}
	Ponto(float x, float y, float z) {
		this->xval = x;
		this->yval = y;
		this->zval = z;
	}
	void setX(float x){
		this->xval = x;
	}
	void setY(float y){
		this->yval = y;
	}
	void setZ(float z){
		this->zval = z;
	}
	void setTextXY(float x, float y){
		this->textXx = x;
		this->textYy = y;
	}
	float x() { return xval; }
	float y() { return yval; }
	float z() { return zval; }
	float textX() {return textXx; }
	float textY() {return textYy; }
	
	Ponto subtrai(Ponto pt){
		Ponto newP = Ponto(this->xval-pt.x(),this->yval-pt.y(),this->zval-pt.z());
		return newP;
	}
	void multiplica(Ponto pt){
		this->xval *= pt.x();
		this->yval *= pt.y();
		this->zval *= pt.z();
	}
	void soma(Ponto pt){
		this->xval += pt.x();
		this->yval += pt.y();
		this->zval += pt.z();
	}
	Ponto cross(Ponto b) {
		Ponto res = Ponto();
		res.setX(yval * b.z() - zval * b.y());
		res.setY(zval * b.x() - xval * b.z());
		res.setZ(xval * b.y() - yval * b.x());
		return res;
	}
	void normalize() {
		float l = sqrt(xval * xval + yval * yval + zval * zval);
		if(l>0){
			xval = xval / l;
			yval = yval / l;
			zval = zval / l;
		}
	}
	string toString(){
		string sponto = to_string(xval) + " " + to_string(yval) + " " + to_string(zval);
		return sponto;
	}
	string toString2d(){
		string sponto = to_string(xval) + " " + to_string(yval);
		return sponto;
	}

};

#endif