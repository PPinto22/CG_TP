#ifndef _LUZ_
#define _LUZ_

class Luz {
public:

	float pos[4];
	float amb[4];
	float diff[4];

	Luz(float type, float posX, float posY, float posZ,
		float ambR, float ambG, float ambB,
		float diffR, float diffG, float diffB) {

		this->pos[0] = posX;
		this->pos[1] = posY;
		this->pos[2] = posZ;
		this->pos[3] = type;

		this->amb[0] = ambR; this->amb[1] = ambG; this->amb[2] = ambB; this->amb[3] = 1.0;
		this->diff[0] = diffR; this->diff[1] = diffG; this->diff[2] = diffB; this->diff[3] = 1.0f;
	}

};

#endif