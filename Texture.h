#pragma once

#include "GL/glew.h"
#include "GL/glut.h"
#include "Shader.h"



class Texture
{


	GLuint texId;		// the texture handle
	int textureUnit;	// the texture unit which will be used for the sampler


public:
	Texture();
	~Texture();

	void loadTextures(char* imageFileName, GLuint texTarget);
	void bindToTextureUnit(int textureUnit);
	GLuint setTextureSampler(Shader shader, char* sampleName, GLuint samplerId);

	GLuint getTexId();
};

