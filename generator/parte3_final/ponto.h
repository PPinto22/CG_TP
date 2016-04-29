#ifndef _PONTO_
#define _PONTO_

class Ponto {
private: float xval, yval, zval;

public:
	Ponto(){
		this->xval = 0;
		this->yval = 0;
		this->zval = 0;
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
	float x() { return xval; }
	float y() { return yval; }
	float z() { return zval; }
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
	void toString(){
		printf("X = %f, Y = %f, Z = %f;\n",this->xval,this->yval,this->zval);
	}

};

#endif