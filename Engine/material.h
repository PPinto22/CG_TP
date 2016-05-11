#ifndef _MATERIAL_
#define _MATERIAL_

class Material{
public:
	float diff[4];
	float spec[4];
	float emiss[4];
	float amb[4];
	GLuint texture;
	bool hasTexture;

	Material(const char* texture,
		float diffR, float diffG, float diffB,
		float specR, float specG, float specB,
		float emissR, float emissG, float emissB,
		float ambR, float ambG, float ambB) {

		if (texture) {
			//Ler com glut e gravar no VBO
			this->hasTexture = true;
		}
		else this->hasTexture = false;

		this->diff[0] = diffR; this->diff[1] = diffG; this->diff[2] = diffB; this->diff[3] = 1.0f;
		this->spec[0] = specR; this->spec[1] = specG; this->spec[2] = specB; this->spec[3] = 1.0f;
		this->emiss[0] = emissR; this->emiss[1] = emissG; this->emiss[2] = emissB; this->emiss[3] = 1.0f;
		this->amb[0] = ambR; this->amb[1] = ambG; this->amb[2] = ambB; this->amb[3] = 1.0f;
	}

};

#endif