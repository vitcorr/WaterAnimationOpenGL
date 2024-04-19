#include "Texture.h"
//#include "GL/soil.h"

#define STB_IMAGE_IMPLEMENTATION    

#include "stb_image.h"


/**************************************************************************************/


Texture::Texture() :texId(-1)
{
}


/**************************************************************************************/




Texture::~Texture()
{
}



/**************************************************************************************/
//  load a texture


void Texture::loadTextures(const char* imageFileName, GLuint texTarget)
{

	int width, height;
	int numChannels;
	unsigned char* image = NULL;
	GLenum pixelFormat = 0;
	// generate the texture
	glGenTextures(1, &texId);
	printf("%s\n", imageFileName);
	printf("%s , %d\n", "TEXID = ", texId);

	glBindTexture(GL_TEXTURE_2D, texId);
	//printf("%s , %d", "numChannels1 = ", numChannels);

	// load the image 
	image = stbi_load(imageFileName, &width, &height, &numChannels, 0);
	if (numChannels == 1) pixelFormat = GL_RED;
	else if (numChannels == 3) pixelFormat = GL_RGB;
	if (numChannels == 4) pixelFormat = GL_RGBA;
	//printf("%s  %d", "numChannels = ", numChannels);
	//Transfer the image data to openGL
	glTexImage2D(GL_TEXTURE_2D, 0, pixelFormat, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, image);
	stbi_image_free(image);
	image = NULL;

	// set the repeat behaviour
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set the sampling behaviour
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}


/**********************************************************************************/

void Texture::bindToTextureUnit(int textureUnit)
{
	// set the active texture (the picture frame)
	glActiveTexture(textureUnit);
	// bind the texture to it using the texId
	glBindTexture(GL_TEXTURE_2D, texId);
	// keep the texture unit internally
	this->textureUnit = textureUnit;
}


/**********************************************************************************/

GLuint Texture::getTexId()
{
	return(texId);
}
/**********************************************************************************/

// the sampler id should correspond to the textureUnit

GLuint Texture::setTextureSampler(Shader shader, const char* samplerName, GLuint samplerId)
{
	int location = 0;
	int rc = 0;
	//get the location of the sampler2D in the fragment shader
	location = glGetUniformLocation(shader.getProgId(), samplerName);
	

	if (location == -1) {
		rc = -1;
		goto err;
	}


	// transfer the samplerId to the shader program
	glUniform1i(location, samplerId);


err:
	return(rc);
}

int Texture::loadTextureImages(const char** texFileName)
{

	int i;
	int rc = 0;
	int width = 1, height = 1;
	unsigned char* image = NULL;
	int numChannels;


	if (texFileName == NULL) return(-1);
	else for (i = 0; i < 6; i++) {
		if (texFileName[i] == NULL) return(-1);
	}


	// Create texture object
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texId);
	this->texTarget = GL_TEXTURE_CUBE_MAP;


	for (i = 0; i < 6; i++) {

		image = stbi_load(texFileName[i], &width, &height, &numChannels, 0);
		if (image != NULL) {
			GLenum format;
			if (numChannels == 1) format = GL_RED;
			else if (numChannels == 3) format = GL_RGB;
			else if (numChannels == 4) format = GL_RGBA;


			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
		}
		else {
			fprintf(stderr, "error loading cube textures - %s \n", texFileName[i]);
			return(-1);
		}
	}

	// set up the behaviour
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP);

	return (0);
}

int Texture::loadColourTexture(void)
{


	int width = 1, height = 1;
	GLubyte posX[4] = { 255, 0, 0, 1 };
	GLubyte negX[4] = { 0, 255, 0, 1 };
	GLubyte posY[4] = { 0, 0, 255, 1 };
	GLubyte negY[4] = { 0, 255, 255, 1 };
	GLubyte posZ[4] = { 255, 0, 255, 1 };
	GLubyte negZ[4] = { 255, 255, 0, 1 };


	// Create texture object
	glGenTextures(1, &texId);
	//glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texId);
	this->texTarget = GL_TEXTURE_CUBE_MAP;


	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, posX);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, negX);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, posY);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, negY);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, posZ);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, negZ);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);


	return 0;
}