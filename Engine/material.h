#ifndef _MATERIAL_
#define _MATERIAL_

class Material{
public:
	float diff[4];
	float spec[4];
	float emiss[4];
	float amb[4];

	bool hasTexture;
	GLuint texture;
	unsigned int image;
	int width, height;
	unsigned char* imageData;

	Material(const char* textureFile,
		float diffR, float diffG, float diffB,
		float specR, float specG, float specB,
		float emissR, float emissG, float emissB,
		float ambR, float ambG, float ambB) {

		if (strcmp(textureFile,"") != 0) {
			this->hasTexture = true;

			ilGenImages(1, &image);
			ilBindImage(image);
			ilLoadImage((ILstring)textureFile);
			ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
			width = ilGetInteger(IL_IMAGE_WIDTH);
			height = ilGetInteger(IL_IMAGE_HEIGHT);
			imageData = ilGetData();

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		}
		else this->hasTexture = false;

		this->diff[0] = diffR; this->diff[1] = diffG; this->diff[2] = diffB; this->diff[3] = 1.0f;
		this->spec[0] = specR; this->spec[1] = specG; this->spec[2] = specB; this->spec[3] = 1.0f;
		this->emiss[0] = emissR; this->emiss[1] = emissG; this->emiss[2] = emissB; this->emiss[3] = 1.0f;
		this->amb[0] = ambR; this->amb[1] = ambG; this->amb[2] = ambB; this->amb[3] = 1.0f;
	}

};

#endif