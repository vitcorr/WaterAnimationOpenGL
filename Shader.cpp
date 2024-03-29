
//=============================================================================
// shader.cpp
//
//Author: Doron Nussbaum (C) 2015 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. wrap around opengl shaders. 
//
//
// Description:
//--------------
//  The wrap around helps in checking the code for errors since this is a simple repetetive task.

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



#include "stdio.h"
#include "stdlib.h"
#include "Shader.h"


#define NUSS_DEBUG 0
/******************************************************************************/
// Constructor

Shader::Shader(void) : shaderProgramid(-1), vertShaderid(-1), fragShaderid(-1)
{

}


/******************************************************************************/
/* destructor */

Shader::~Shader(void)
{
}


/******************************************************************/
char* Shader::readCode(char* fileName)
{
	char* shaderCode = NULL;
	int codeLength = 0;
	FILE* fp = NULL;

	// check for error in file name

	fp = fopen(fileName, "rb");	// open file and check for errors
	if (fp == NULL) { return NULL; }

	// fine the length of code
	fseek(fp, 0L, SEEK_END);
	codeLength = ftell(fp);
	rewind(fp);		// could use fseek(fp, 0L, SEEK_SET)

	if (codeLength > 0) {
		// allocated space for code and check for errors
		shaderCode = (char*)malloc(codeLength + 1);	// add a space for the '\0'
		if (shaderCode == NULL) return(NULL);
		memset((void*)shaderCode, 0, codeLength + 1);
		fread((void*)shaderCode, sizeof(char), codeLength, fp);
		shaderCode[codeLength] = 0;
	}

	if (fp != NULL) fclose(fp);

	return shaderCode;
}



/******************************************************************************/
/* create the shaders

input:
fileName - file name of shader.  Path is assume to be the project.
Otherwise, pass in the directory as well

shaderType - can either be vertex shader or fragment shader.  The type
must correspond to the OpenGL shader types.

Output:
shaderid - a handle to the shader.

return:
0 - if successfull
-1 if error

*/

int Shader::createShaderObj(char* fileName, int shaderType, GLuint* shaderid)
{
	char* code = NULL;
	int rc = 0;


	// create a shader handle
	*shaderid = glCreateShader(shaderType);

	// read the code
	code = readCode(fileName);
	if (code == NULL) return(-1);

	// attach the source to the shaders
	glShaderSource(*shaderid, 1, &code, NULL);

	// compile the code
	glCompileShader(*shaderid);;

	// check for errors
	glGetShaderiv(*shaderid, GL_COMPILE_STATUS, &rc);
	if (rc != GL_TRUE) {
		GLsizei length;
		GLsizei bufSize = 0;
		char* error = NULL;
		fprintf(stderr, "Error when creating a shader file=%s \n", fileName);
		//fputs("Error when creating a shader \n",stderr);
		glGetShaderiv(*shaderid, GL_INFO_LOG_LENGTH, &bufSize);
		error = (char*)malloc(bufSize * sizeof(char) + 1);
		if (error != NULL) {
			glGetShaderInfoLog(*shaderid, bufSize, &length, error);
			fprintf(stderr, "%s \n", error);
		}
		if (error != NULL) free(error);
		rc = -1;
	}
	else rc = 0;

	if (code != NULL) free(code);	// free the source code of the shader

	return(rc);

}



/***************************************************************************************************************/

/* functions creates a shader program.  The two shader programs (vertex and fragment) were already compiled.
input:
vertShaderid - a handle to the vertex shader

fragShaderid - a handle to the fragment shader

Output:
shhderProgId - a handle to the shader program.

return:
0 - if successfull
-1 if error
*/

GLint Shader::ceateShaderProgram(GLint vertShaderid, GLint fragShaderid, GLuint* shaderProgId)
{

	int rc = 0;
	// get a handle to the shader program
	shaderProgramid = glCreateProgram();
	// connect the shaders subprogram to the "main" program
	glAttachShader(shaderProgramid, vertShaderid);
	rc = glGetError();
	if (rc != GL_NO_ERROR) {
		fprintf(stderr, "error in attach shaders \n");
		rc = -1;
		goto err;
	}

	glAttachShader(shaderProgramid, fragShaderid);
	rc = glGetError();
	if (rc != GL_NO_ERROR) {
		fprintf(stderr, "error in attach shaders \n");
		rc = -1;
		goto err;
	}

	glLinkProgram(shaderProgramid);

	// check for errors
	glGetProgramiv(shaderProgramid, GL_LINK_STATUS, &rc);

	if (rc != GL_TRUE) {
		rc = -1;
		GLsizei length;
		GLsizei bufSize = 0;
		char* error = NULL;
		fprintf(stderr, "Error when creating a shader program \n");
		//fputs("Error when creating a shader \n",stderr);
		glGetProgramiv(shaderProgramid, GL_INFO_LOG_LENGTH, &bufSize);
		error = (char*)malloc(bufSize * sizeof(char) + 1);
		if (error != NULL) {
			glGetProgramInfoLog(shaderProgramid, bufSize, &length, error);
			fprintf(stderr, "%s \n", error);
		}
		if (error != NULL) free(error);
	}
	else rc = 0;

	printShaderStatus();  // may want to remove it.

	if (shaderProgId != NULL) *shaderProgId = shaderProgramid;
err:
	return (rc);
}


/***************************************************************************************************************/

/* creates a shader program from files vsFileName and fsFileName

input:
vsFileName - name of vertex shader

fsFileName - name of fragment shader

output:
none


return:
0 if succssfull
!=0 - if error

*/


int Shader::createShaderProgram(char* vsFileName, char* fsFileName)
{

	int rc = 0;

	rc = createShaderObj(vsFileName, GL_VERTEX_SHADER, &vertShaderid);
	if (rc == 0) {
		rc = createShaderObj(fsFileName, GL_FRAGMENT_SHADER, &fragShaderid);
	}
	if (rc == 0) {
		rc = ceateShaderProgram(vertShaderid, fragShaderid, &shaderProgramid);
	}

	return(rc);
}




/***************************************************************************************************************/

/* creates a shader program from files vsFileName and fsFileName

input:
vsFileName - name of vertex shader

fsFileName - name of fragment shader

output:
shaderProgramId - a handle to the shader program.  Note that it is also stored
internally.

return:
0 if succssfull
!=0 - if error

*/


int Shader::createShaderProgram(char* vsFileName, char* fsFileName, GLuint* shaderProgramid)
{

	int rc = 0;

	rc = createShaderObj(vsFileName, GL_VERTEX_SHADER, &vertShaderid);
	if (rc == 0) {
		rc = createShaderObj(fsFileName, GL_FRAGMENT_SHADER, &fragShaderid);
	}
	if (rc == 0) {
		rc = ceateShaderProgram(vertShaderid, fragShaderid, shaderProgramid);
	}

	return(rc);
}




/*********************************************************************/
// print the status and some information about the linked shader program

int Shader::printShaderStatus(void)
{
	int rc;

	glGetProgramiv(shaderProgramid, GL_VALIDATE_STATUS, &rc);
	if (rc == 0) {
		//fprintf(stderr, "shader is valid status = %d \n", rc);
		printf("shader is valid status = %d \n", rc);
	}
	else {
		//fprintf(stderr, "shader is not valid status = %d \n", rc);
		printf("shader is not valid status = %d \n", rc);
	}

	rc = 0;
	glGetProgramiv(shaderProgramid, GL_INFO_LOG_LENGTH, &rc);
	if (rc == 0) {
		//fprintf(stderr, "shader program info log length= %d \n", rc);
		printf("shader program info log length= %d \n", rc);
	}
	else {
		char s[1024];
		int length = 0;
		s[0] = 0;
		glGetProgramInfoLog(shaderProgramid, 1024, &length, s);
		//fprintf(stderr, "shader info log = %s \n", s);
		printf("shader info log = %s \n", s);
	}

	glGetProgramiv(shaderProgramid, GL_ATTACHED_SHADERS, &rc);
	fprintf(stderr, "Number of attached shaders = %d \n", rc);

	glGetProgramiv(shaderProgramid, GL_ACTIVE_ATTRIBUTES, &rc);
	fprintf(stderr, "Number of active attributes = %d \n", rc);

	glGetProgramiv(shaderProgramid, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &rc);
	fprintf(stderr, "Max name length (active attriute) = %d \n", rc);

	glGetProgramiv(shaderProgramid, GL_ACTIVE_UNIFORMS, &rc);
	fprintf(stderr, "Number of active uniforms = %d \n", rc);

	glGetProgramiv(shaderProgramid, GL_ACTIVE_UNIFORM_MAX_LENGTH, &rc);
	fprintf(stderr, "Max name length (active uniform) = %d \n", rc);

	return 0;
}


/*********************************************************************/
/* Enable or disable shader program to be current program

input:
useProg ==  0 disable the progrdam
useProg != 0 set the shader to be used

*/



void Shader::useProgram(int useProg) {
	if (useProg != 0) {
		glUseProgram(shaderProgramid);
	}
	else {
		glUseProgram(0);
	}
}


/*********************************************************************/
/*
Copy a matrix to the shader
This is a helper function since this operation is used quite often


*/
int Shader::copyMatrixToShader(Matrix4f matrix, const char* name) {
	int location;

	// get the location of the matrix
	location = glGetUniformLocation(getProgId(), name);

	assert(location != -1);
	if (location == -1) return (-1);

	// transfer the matrix to the shader program
	glUniformMatrix4fv(location, 1, true, matrix.data());
	return(0);
}


/*********************************************************************/
/*
Copy an array of floats to the shader
This is a helper function since this operation is used quite often
input
v - the array
vectorSize - number of elements in the array
numElementFloats - number of floats in an element
name - name of variable in the shader


*/
int Shader::copyFloatVectorToShader(float* v, int vectorSize, int numElementFloats, const char* name)
{
	int location;
	int i;
	int rc = 0;
	float loaded[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };


	location = glGetUniformLocation(getProgId(), name);
	//assert(location != -1);  DN
	if (location == -1) return (-1);
	switch (numElementFloats) {
	case 1:
		glUniform1fv(location, vectorSize, v);
		break;
	case 2:
		glUniform2fv(location, vectorSize, v);
		break;
	case 3:
		glUniform3fv(location, vectorSize, v);
		break;
	case 4:
		glUniform4fv(location, vectorSize, v);
		break;
	}
	for (int i = 0; i < 20; i++) {
		rc = glGetError();
		if (rc != GL_NO_ERROR) {
			switch (rc) {
			case GL_INVALID_VALUE:
				printf("GL_INVALID_VALUE is generated if program is not a value generated by OpenGL.\n");
				break;
			case GL_INVALID_OPERATION:
				printf("GL_INVALID_OPERATION is generated if program is not a program object. or \n");
				printf("GL_INVALID_OPERATION is generated if program has not been successfully linked. or \n");
				printf("GL_INVALID_OPERATION is generated if location does not correspond to a valid uniform variable location for the specified program object.");
				break;
			default:
				printf("openGL unknown error rc = \n", rc);
			}
			rc = -1;
		}
		else break;
	}


	return(rc);
}

/*********************************************************************/
/*
Copy an array of integers to the shader
this is a helper function since this operation is used quite often

input
v- the array
vectorSize - number of elements in the array
numElementInts - number of integers in an element
name - name of variable in the shader

*/
int Shader::copyIntVectorToShader(int* v, int vectorSize, int numElementInts, const char* name) {
	int location;
	int i;
	int rc;
	int loaded[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };


#if NUSS_DEBUG
	printf("\n");
	printf("loading %s size=%d", name, vectorSize);
	for (i = 0; i < elementSize; i++) {
		printf("[%d]= %d,", i, v[i]);
	}
	printf("\n");
#endif
	location = glGetUniformLocation(getProgId(), name);
	// assert(location != -1);  DN
	if (location == -1) return (-1);
	switch (numElementInts) {
	case 1:
		glUniform1iv(location, vectorSize, v);
		break;
	case 2:
		glUniform2iv(location, vectorSize, v);
		break;
	case 3:
		glUniform3iv(location, vectorSize, v);
		break;
	case 4:
		glUniform4iv(location, vectorSize, v);
		break;
	}

	while (1) {
		rc = glGetError();
		if (rc == GL_NO_ERROR) break;
		switch (rc) {
		case GL_INVALID_VALUE:
			printf("GL_INVALID_VALUE is generated if program is not a value generated by OpenGL.\n");
			break;
		case GL_INVALID_OPERATION:
			printf("GL_INVALID_OPERATION is generated if program is not a program object. or \n");
			printf("GL_INVALID_OPERATION is generated if program has not been successfully linked. or \n");
			printf("GL_INVALID_OPERATION is generated if location does not correspond to a valid uniform variable location for the specified program object.");
			break;
		default:
			printf("openGL unknown error \n");
		}		rc = -1;
	}
#if NUSS_DEBUG
	glGetUniformiv(getProgId(), location, loaded);
	printf("       loaded   ");
	for (i = 0; i < elementSize; i++) {
		printf("[%d]= %d ,", i, loaded[i]);
	}
	printf("\n");
#endif
	return(0);
}



/*****************************************************************************************/

/*
Purpose: create a shader program from a single vertex file
and a single fragment file

Input:
vertFiles -  an arrray of file names that when combined make the shader code.
this is a null terminated array where the last element in the array
must be NULL (the sentinel is a NULL)
fragFilse -  an arrray of file names that when combined make the shader code.
this is a null terminated array where the last element in the array
must be NULL (the sentinel is a NULL)

Return:
0 - is success
1 - if not successful

*/


int Shader::createFromFiles(char** vertFiles, char** fragFiles)

{
	int rc = 0;
	char tempVertFile[128];
	char tempFragFile[128];
	int i;
	std::string sysCmd;


	// check if all files exist
	rc = filesExist(vertFiles);
	if (rc != 0) {
		rc = 1;
		goto err;
	}
	rc = filesExist(fragFiles);
	if (rc != 0) {
		rc = 2;
		goto err;
	}

	// concatenate the files to a single file
	tmpnam_s(tempVertFile, 128);
	sysCmd = "type ";
	for (i = 0; vertFiles[i] != NULL; i++) {
		sysCmd += vertFiles[i];
		sysCmd += "  ";
	}
	sysCmd += " > ";
	sysCmd += &tempVertFile[1];

	// execute the command
	rc = system(sysCmd.c_str());

	tmpnam_s(tempFragFile, 128);
	sysCmd.clear();
	sysCmd = "type ";
	for (i = 0; fragFiles[i] != NULL; i++) {
		sysCmd += fragFiles[i];
		sysCmd += "  ";
	}
	sysCmd += " > ";
	sysCmd += &tempFragFile[1];

	// execute the command
	system(sysCmd.c_str());




	// create the shader program from a single file
	rc = createShaderProgram(&tempVertFile[1], &tempFragFile[1]);
	rc = remove(&tempVertFile[1]);
	rc = remove(&tempFragFile[1]);
err:
	return rc;
}

/***********************************************************************************/
/*

purpose: check if all files in the array exist

input
files - an array of files names.  The last element in the array is NULL
*/

int Shader::filesExist(char** fileNames)

{
	int rc = 0;
	char** p;
	FILE* fid;

	for (p = fileNames; *p != NULL; p++) {
		fid = fopen(*p, "rb");
		if (fid == NULL) {
			rc = 1;
			break;
		}
		fclose(fid);
	}


	return(rc);

}

