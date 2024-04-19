//=============================================================================
// skybox.cpp
//
//Author: Doron Nussbaum (C) 2024 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. demonstarte the usage of a skybox
//
//
// Description:
//--------------
// A simple application that demonstrates how to crete and use a skybox 
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes.
// Usage of code for other purposes is not allowed without a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

//=============================================================================





#include "SkyBox.h"

#include "stdio.h"
#include "stdlib.h"

#include "GL/nuss_math.h"



/*****************************************************************/
// helper function - move to a new file gl_util

static int checkError()
{
	int rc = glGetError();
	switch (rc) {
	case GL_NO_ERROR:
		rc = 0;
		break;
	case GL_INVALID_ENUM:
		printf("gl error - not a legal enum \n");
		rc = 1;
		break;
	case GL_INVALID_VALUE:
		printf("gl error - not a legal value \n");
		rc = 2;
		break;

	case GL_INVALID_OPERATION:
		printf("gl error - not a legal operation \n");
		rc = 3;
		break;


	case GL_INVALID_FRAMEBUFFER_OPERATION:
		printf("gl error - stack underflow \n");
		rc = 5;
		break;

	case GL_OUT_OF_MEMORY:
		printf("gl error - out of memory \n");
		rc = 6;
		break;


	default:
		printf("gl error - default \n");
		rc = 9;
		break;
	}
	return(rc);
}

/****************************************************************************************/

SkyBox::SkyBox(void) : tex(), s()
{

}


SkyBox::~SkyBox(void)
{
}


/**************************************************************************************/

/*loads the skybox images

order of images is
GL_TEXTURE_CUBE_MAP_POSITIVE_X
GL_TEXTURE_CUBE_MAP_NEGATIVE_X
GL_TEXTURE_CUBE_MAP_POSITIVE_Y
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
GL_TEXTURE_CUBE_MAP_POSITIVE_Z
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z

*/


int SkyBox::loadTextureImages(const char** texFileName)
{

	int rc;
	int i;

	if (texFileName == NULL) return(-1);
	else for (i = 0; i < 6; i++) {
		if (texFileName[i] == NULL) return(-1);
	}

	rc = tex.loadTextureImages(texFileName);

	return (rc);
}



/********************************************************************************************/

/**********************************************************************************************/

void SkyBox::render(Camera cam)
{

	int t = 1;  // remove
	Matrix4f viewMat, projMat, modelMat, m;


	glDisable(GL_DEPTH_TEST);    // need depth test to correctly draw 3D objects 

	// set up the mode to wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//set up the mode to fill
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// load the program to the shader
	s.useProgram(1);


	// get the transformation matrix 
	modelMat = Matrix4f::translation(cam.getPosition());
	// move matrix to shader
	s.copyMatrixToShader(modelMat, "modelMat");


	// setting up the viewpoint transformation
	viewMat = cam.getViewMatrix(NULL);	// get the camera view transformation
	// move matrix to shader
	s.copyMatrixToShader(viewMat, "viewMat");

	// set the projection matrix
	projMat = cam.getProjectionMatrix(NULL);
	// move matrix to shader
	s.copyMatrixToShader(projMat, "projMat");


	// bind the buffers to the shaders
	GLuint positionLoc = glGetAttribLocation(s.getProgId(), "vertex_position");

	glEnableVertexAttribArray(positionLoc);
	glBindBuffer(GL_ARRAY_BUFFER, vboCube);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboCube);

	// Tells OpenGL how to walk through the two VBOs
	glVertexAttribPointer(positionLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	// draw the triangles

	GLuint bufferId;

	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, (GLint*)&bufferId);



	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (char*)NULL + 0);

	glEnable(GL_DEPTH_TEST);    // need depth test to correctly draw 3D objects 

	return;

}

GLuint SkyBox::getTexHandle(void)
{
	return (tex.getTexId());
}

/**************************************************************************************/
/* this function creates a cube aound (0,0,0).

Ouptut:
vboCube - the vertex buffer offer of the cube
iboCube - the index buffer offer of the cube
Return:
the function returns 0 is successful.


*/

int SkyBox::createCube(void)
{
	float vtx1[8][4] = {
			{-1.0,  1.0,  1.0, 1.0},
			{-1.0, -1.0,  1.0, 1.0},
			{1.0, -1.0,  1.0, 1.0},
			{1.0,  1.0,  1.0, 1.0},
			{-1.0,  1.0, -1.0, 1.0},
			{-1.0, -1.0, -1.0, 1.0},
			{1.0, -1.0, -1.0, 1.0},
			{1.0,  1.0, -1.0, 1.0} };

	float vtx[8][4] = {
			{-10.0,  10.0,  10.0, 1.0},
			{-10.0, -10.0,  10.0, 1.0},
			{10.0, -10.0,  10.0, 1.0},
			{10.0,  10.0,  10.0, 1.0},
			{-10.0,  10.0, -10.0, 1.0},
			{-10.0, -10.0, -10.0, 1.0},
			{10.0, -10.0, -10.0, 1.0},
			{10.0,  10.0, -10.0, 1.0} };

	glGenBuffers(1, &vboCube);
	printf("sizeof vtx=%d \n", sizeof(vtx));
	glBindBuffer(GL_ARRAY_BUFFER, vboCube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx), vtx, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint ind[12][3] = {
		{0, 1, 2},
		{2, 3, 0},
		{3, 2, 6},
		{6, 7, 3},
		{7, 6, 5},
		{5, 4, 7},
		{4, 5, 1},
		{1, 0, 4},
		{0, 3, 7},
		{7, 4, 0},
		{1, 2, 6},
		{6, 5, 1},
	};

	printf("sizeof ind=%d size of gluint*36=%d \n", sizeof(ind), sizeof(GLuint) * 36);
	glGenBuffers(1, &iboCube);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboCube);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);

	return 0;
}

/*******************************************************************************/

int SkyBox::init(const char* vsFileName, const char* fsFileName)
{

	int rc;
	rc = s.createShaderProgram(vsFileName, fsFileName);
	if (rc != 0) {
		printf(" error after generating skybox shaders \n");
		getchar();
		exit(1);
	}

	createCube();

	return 0;
}

/*******************************************************************************************/
// loads a colour textures
// front (posZ) is purple (magenta), 
// back (negZ) is yellow, 
// left (negX) is green
// right (posX) is red
// top (posY) is blue)
// bottom (negY) is cyan



int SkyBox::loadColourTexture(void)
{

	tex.loadColourTexture();

	return 0;
}
