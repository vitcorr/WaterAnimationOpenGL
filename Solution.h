#pragma once
#ifndef SOLUTION_HEADER
#define SOLUTION_HEADER



//=============================================================================
// solution.h
//
//Author: Doron Nussbaum (C) 2023 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. solution framework for assignments
//
//
// Description:
//--------------
// A simple framework for drawing objecs 
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




#include "GL/glew.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include "GL/glut.h"
#endif

#include "Shader.h"
#include "Water.h"
#include "camera.h"
#include "GL/nuss_math.h"


/*****************************************************************************************/
// DEFINES

#define UPDATE_RENDERRED_OBJECTS 1000
#define FRAME_TIME 30


/************************************************************************************************/
// STRUCTURES


// Point light properties
struct pointLight {
	Vector3f lightIntensity;		// light intensity
	Vector3f ambientIntensity;		// intesity of ambient colour
	Vector3f worldPos;
	float specularPower;		// specular light power factor
};

// directional light properties
struct directionalLight {
	Vector3f lightIntensity;		// light intensity
	Vector3f ambientIntensity;		// intesity of ambient colour
	Vector3f lightDirection;		// direciton of light
	float specularPower;		// specular light power factor
};

// light attenuation  
// attenuation = 1/(a + b * d + c * d * d)
struct attenuation {
	float a;
	float b;
	float c;
};



// spot light properties
struct spotLight {
	Vector3f lightIntensity;		// light intensity
	Vector3f ambientIntensity;		// intesity of ambient colour
	Vector3f worldPos;				// location of spot light
	Vector3f lightDirection;		// direction of spot light
	float spotdDistLimit;			// distance limit of the spot light
	float cosSpotAlpha;				// cos of max opening of spot light (angle alpha)
	float specularPower;			// specular light power factor
};





class Solution
{
public:
	Solution();
	~Solution();
	// initializing the opengl functions and windows
	int initOpenGL(int argc, char** argv, int posX, int posY, int winWidth, int winHeight);


	// static callback function 
	static void renderCB();
	static void passiveMouseCB(int x, int y);
	static void keyboardCB(unsigned char key, int x, int y);
	static void specialKeyboardCB(int key, int x, int y);
	static void winResizeCB(int width, int height);
	static void timerCB(int operation);
	static void menuFunCB(int num);

	static void setSolution(Solution* _sol);

	// generaL shader that will be used by all objects
	// initialization of the solution
	int initSolution();




private:
	static Solution* sol;
	Shader waterShader;
	int waterShaderProgId;
	Water waterfloor;

	

	Camera cam;
	int factor;
	unsigned int numFrames;	// number of frames so far 

	int winWidth, winHeight;

	//Blinn-Phong
	int blinn = 0;
	int lightType;		// type of light 
	int includeAmbient;		// flag for rendering ambient  colour
	int includeDiffuse;	// flag for rendering diffuse  colour
	int includeSpecular;	// flag for rendering specular colour
	int includeObjColour;	// flag for rendering object colour
	int plotWireFrame;		// plot in wireframe of filled polyogns == 1
	int plotCorrect;	// plot correctly with normal matrix

	int setLights(Vector3f pointLightPos, Vector3f spotLightPos, Vector3f spotLightLookAtPoint);
	int render();
	void keyboard(unsigned char key, int x, int y);

	void passiveMouse(int x, int y);

	void specialKeyboard(int key, int x, int y);
	void winResize(int width, int height);
	int timer(int operation);

	void createMenu(void);
	int updateObjects(unsigned int numFrames);

	// lights
	struct pointLight pointLight;
	struct spotLight spotLight;
	struct directionalLight directionalLight;

	int loadLights(Shader shader);
	//int loadMaterials(Shader shader);
	int printOpenGLError(int errorCode);
	int clearGLError();
	int checkGLError();


};



#endif