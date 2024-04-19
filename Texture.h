#pragma once

#include "GL/glew.h"
#include "GL/glut.h"
#include "Shader.h"



class Texture
{


	GLuint texId;		// the texture handle
	int textureUnit;	// the texture unit which will be used for the sampler
	GLuint texTarget;

public:
	Texture();
	~Texture();

	void loadTextures(const char* imageFileName, GLuint texTarget);
	void bindToTextureUnit(int textureUnit);
	int loadColourTexture(void);					// loads a cube colour textures
	GLuint setTextureSampler(Shader shader, const char* sampleName, GLuint samplerId);
	int loadTextureImages(const char** texFileName);		// loads a cube texture images

	GLuint getTexId();
};

