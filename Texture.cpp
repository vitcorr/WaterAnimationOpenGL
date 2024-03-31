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
	//glBindTexture(GL_TEXTURE_2D, texId);
	//printf("%s , %d", "numChannels1 = ", numChannels);

	// load the image 
	image = stbi_load(imageFileName, &width, &height, &numChannels, 0);
	if (numChannels == 1) pixelFormat = GL_RED;
	else if (numChannels == 3) pixelFormat = GL_RGB;
	if (numChannels == 4) pixelFormat = GL_RGBA;
	printf("%s  %d", "numChannels = ", numChannels);
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
	glBindTexture(textureUnit, texId);
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

GLuint Texture::setTextureSampler(Shader shader, char* samplerName, GLuint samplerId)
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