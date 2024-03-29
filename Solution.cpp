

//=============================================================================
// solution.c
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

// INCLUDES

#include "Solution.h"

/***************************************************************************/

// DEFINES 

// MENU defines
#define EXIT_PROGRAM 0
#define CLEAR_SCREEN 1
#define RENDER_HOUSE_1 10
#define RENDER_HOUSE_2 20
#define RENDER_WIREFRAME 2
#define RENDER_FILLED	3

#define NORMAL_SPEED 0.5
#define MAX_SPEED 2.0

#define NO_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT  2
#define DIRECTIONAL_LIGHT 3

/************************************************************/
//	GLOBALS

Solution* Solution::sol = NULL;


/****************************************************************************/

Solution::Solution() : numFrames(0)
{

	includeAmbient = includeDiffuse = includeSpecular = includeObjColour = 0;

}

/*************************************************************************/


Solution::~Solution()
{
	printf("\n exiting the progam gracefully\n");

}

/******************************************************************************/


// initializing the opengl functions and windows
int Solution::initOpenGL(int argc, char** argv, int posX, int posY, int winWidth, int winHeight)
{
	int winid1;
	//initialize glut
	glutInit(&argc, argv);

	//initialize OpenGLwindow
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(posX, posY);
	glutInitWindowSize(winWidth, winHeight);
	winid1 = glutCreateWindow("Drawing Basic Objects");
	checkGLError();

	//	glutSetWindow(winid1);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	glutDisplayFunc(Solution::renderCB);
	glutReshapeFunc(Solution::winResizeCB);
	glutKeyboardFunc(Solution::keyboardCB);
	glutSpecialFunc(Solution::specialKeyboardCB);
	glutPassiveMotionFunc(Solution::passiveMouseCB);
	glutTimerFunc(FRAME_TIME, Solution::timerCB, UPDATE_RENDERRED_OBJECTS);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// create a menu
	createMenu();

	checkGLError();


	GLenum res = glewInit();
	if (res != GLEW_OK) {
		printf("Error - %s \n", glewGetErrorString(res));
		return (-1);
	}


	return 0;
}

/************************************************************/

// render callback function.  This is a static funcion


void Solution::renderCB()
{

	sol->render();

}


/************************************************************/

// keyboard callback function.  This is a static funcion


void Solution::passiveMouseCB(int x, int y)
{
	sol->passiveMouse(x, y);
}


/************************************************************/

// keyboard callback function.  This is a static funcion


void Solution::keyboardCB(unsigned char key, int x, int y)
{
	sol->keyboard(key, x, y);
}


/************************************************************/

// special keyboard callback function.  This is a static funcion



void Solution::specialKeyboardCB(int key, int x, int y)
{
	sol->specialKeyboard(key, x, y);
}


/************************************************************/

// window resize callback function.  This is a static funcion



void Solution::winResizeCB(int width, int height)
{
	sol->winResize(width, height);
}

/************************************************************/

// timer  callback function.  This is a static funcion


void Solution::timerCB(int operation)
{

	glutTimerFunc(FRAME_TIME, Solution::timerCB, UPDATE_RENDERRED_OBJECTS);
	sol->timer(operation);

}


/************************************************************/

// timrt  function.  


int Solution::timer(int operation)
{

	switch (operation) {
	case UPDATE_RENDERRED_OBJECTS:
		updateObjects(numFrames);
		break;
	default:
		break;
	}
	return(0);
}




/******************************************************************************/
// initialization of the solution
int Solution::initSolution()
{
	int rc;
	Vertices vtx;
	Indices ind;


	// Add Code
	// initialize the geomegtry and transformation of the floor object
	// world position and scale
	//init the geometry
	waterfloor.initGeom();
	//model to model
	waterfloor.setModelScale(15, 15, 5);
	waterfloor.setModelRotations(0, -45, 0);
	//model to world
	waterfloor.setWorldPosition(Vector3f(10, 20, 0));

	// create the house by invoking the house create function
	//waterfloor.create();


	// set the camera initial position
	cam.setCamera(Vector3f(15, 15, 70), Vector3f(15, 0, 0), Vector3f(0, 1, 0));

	return 0;
}


/**********************************************************************/

void Solution::setSolution(Solution* _sol)
{
	Solution::sol = _sol;
}



/************************************************************/

// render function.  


void Solution::render()
{

	Vector3f viewerPosition;
	Vector3f lookAtPoint;
	Vector3f upVector;;
	Matrix4f viewMat, projMat;



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glDisable(GL_CULL_FACE);

	glViewport(0, 0, (GLsizei)winWidth, (GLsizei)winHeight);

	// set the view model transformation
	glMatrixMode(GL_MODELVIEW);
	viewMat = cam.getViewMatrix(NULL);	// get the camera view transformation

	// pass it to opengl before draw
	viewMat = Matrix4f::transpose(viewMat);
	//viewMat = Matrix4f::identity();
	glLoadMatrixf((GLfloat*)viewMat.data());


	// set the projection matrix
	projMat = cam.getProjectionMatrix(NULL);

	glMatrixMode(GL_PROJECTION);
	// pass it to opengl - Note that OpenGL accepts the matrix in column major
	projMat = Matrix4f::transpose(projMat);
	glLoadMatrixf((GLfloat*)projMat.data());



	// render the objects
	waterfloor.render(Matrix4f::identity());
	//house.render(Matrix4f::identity());
	glutSwapBuffers();
	/*return 0;*/
}


/************************************************************/

// passive motion function. 


void Solution::passiveMouse(int x, int y)

{
	int winid = glutGetWindow();
	//std::cout << "Passive Mouse winId=" << winid << "(" << x << " , " << y << ")" << std::endl;
}

/************************************************************/

// keyboard handling function. 

void Solution::keyboard(unsigned char key, int x, int y)
{
	static int nc = 0;
	nc++;
	switch (key) {
	case 033:
	case 'Q':
	case 'q':
		this->~Solution();
		exit(1);
		break;
	case 'm':
		includeAmbient = includeAmbient ? 0 : 1;
		break;
	case 'n':
		includeDiffuse = includeDiffuse ? 0 : 1;
		break;
	case 'b':	// 
		includeSpecular = includeSpecular ? 0 : 1;
		break;
	case 'v':	// 
		includeObjColour = includeObjColour ? 0 : 1;
		break;
	case 'w':
		cam.moveForward(NORMAL_SPEED * factor);
		break;
	case 's':
		cam.moveBackward(NORMAL_SPEED * factor);
		break;
	case 'd':
		cam.moveRight(NORMAL_SPEED * factor);
		break;
	case 'a':
		cam.moveLeft(NORMAL_SPEED * factor);
		break;
	case 'x':
		cam.yaw(1 * factor);
		break;
	case 'c':
		cam.yaw(-1 * factor);
		break;
	case 'z':
		cam.zoomIn();
		break;
	case 'Z':
		cam.zoomOut();
		break;
	case 't':
		plotWireFrame = plotWireFrame ? 0 : 1;
		break;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		factor = key - '0';
		pointLight.specularPower = factor * 3;
		break;
	case 'e':
		//phongSphere.incrementModelScale(0, factor, 0);
		//gouraudSphere.incrementModelScale(0, factor, 0);
		break;
	case 'E':
		//phongSphere.incrementModelScale(0, -factor, 0);
		//gouraudSphere.incrementModelScale(0, -factor, 0);
		break;
	case 'r':
		plotCorrect = plotCorrect ? 0 : 1;
		break;
	case 'H':
		blinn = 1;
		break;
	case 'h':
		blinn = 0;
		break;
	}


}

/************************************************************/

// special keyboard handling  function.  


void Solution::specialKeyboard(int key, int x, int y)
{
	switch (key) {
	case 033:
	case 'Q':
	case 'q':
		exit(1);
		break;
	case GLUT_KEY_LEFT:
		cam.roll(1);
		break;
	case GLUT_KEY_UP:
		cam.pitch(1.0);
		break;
	case GLUT_KEY_RIGHT:
		cam.roll(-1);
		break;
	case GLUT_KEY_DOWN:
		cam.pitch(-1);
		break;
	}
}


/************************************************************/

// window resize handling function.  



void Solution::winResize(int width, int height)
{
	glViewport(0, 0, width, height);
	winWidth = width;
	winHeight = height;

}

/***********************************************************/
// update the state of the objects

int Solution::updateObjects(unsigned int numFrames)
{
	numFrames++;
	// recall that this will be carried out in the model space
	//phongSphere.incrementModelRotations(0, 0, 0.5);
	//gouraudSphere.incrementModelRotations(0, 0, 0.5);

	glutPostRedisplay();
	return 0;
}




/******************************************************************************/
/* purpose: Creates a menu and a sub menuassigns it to the right mouse button*/

void Solution::createMenu(void) {

	// create a submenu
	int submenu_id = glutCreateMenu(menuFunCB);
	glutAddMenuEntry("Move house 1 by 10 along hte x-axis", RENDER_HOUSE_1);
	glutAddMenuEntry("Rotate house 2 by 3 more degreees", RENDER_HOUSE_2);
	glutAddMenuEntry("Render filled ", RENDER_FILLED);
	glutAddMenuEntry("Render wireframe", RENDER_WIREFRAME);

	// create the main menu
	int menu_id = glutCreateMenu(menuFunCB);
	//glutAddMenuEntry("Clear Screen", CLEAR_SCREEN);
	glutAddSubMenu("Render Objects", submenu_id);
	glutAddMenuEntry("Quit", EXIT_PROGRAM);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/******************************************************************************/
/* purpose: Processes the response from the GLUT menu entry

Note that the function asigns to a global variable the optin that the user wanted

*/

void Solution::menuFunCB(int num)
{
	int winId;
	switch (num) {
	case EXIT_PROGRAM:
		winId = glutGetWindow();
		glutDestroyWindow(winId);
		exit(0);
		break;
	case RENDER_HOUSE_1:
		break;
	case RENDER_HOUSE_2:
		break;
	case RENDER_FILLED:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case RENDER_WIREFRAME:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;

	default:
		break;
	}
	glutPostRedisplay();

}

/***********************************************************/
// update the state of the objects



int Solution::setLights(Vector3f pointLightPos, Vector3f spotLightPos, Vector3f spotLightLookAtPoint)
{

	pointLight.ambientIntensity = Vector3f(0.3, 0.2, 0.2);
	pointLight.lightIntensity = Vector3f(1, 1, 1);
	pointLight.specularPower = 40;
	pointLight.worldPos = pointLightPos;

	spotLight.ambientIntensity = Vector3f(0.2, 0.2, 0.2);
	spotLight.lightIntensity = Vector3f(1, 1, 1);
	spotLight.cosSpotAlpha = cos(30 * M_PI / 180.0);
	spotLight.lightDirection = spotLightLookAtPoint - spotLightPos;
	spotLight.specularPower = 4;
	spotLight.spotdDistLimit = 10000;
	spotLight.worldPos = spotLightPos;

	directionalLight.ambientIntensity = Vector3f(0.2, 0.2, 0.2);
	directionalLight.lightIntensity = Vector3f(1, 1, 1);
	directionalLight.specularPower = 4;
	directionalLight.lightDirection = spotLightLookAtPoint - spotLightPos;

	return 0;
}

/*********************************************************************************/

// loads the lights to the shader

int Solution::loadLights(Shader shader)
{

	// load point light
	shader.copyFloatVectorToShader((float*)&pointLight.ambientIntensity, 1, 3, "gPointLight.ambientIntensity");
	shader.copyFloatVectorToShader((float*)&pointLight.lightIntensity, 1, 3, "gPointLight.lightIntensity");
	shader.copyFloatVectorToShader((float*)&pointLight.worldPos, 1, 3, "gPointLight.worldPos");
	shader.copyFloatVectorToShader((float*)&pointLight.specularPower, 1, 1, "gPointLight.specularPower");



	// load spot light
	shader.copyFloatVectorToShader((float*)&spotLight.ambientIntensity, 1, 3, "gSpotLight.ambientIntensity");
	shader.copyFloatVectorToShader((float*)&spotLight.lightIntensity, 1, 3, "gSpotLight.lightIntensity");
	shader.copyFloatVectorToShader((float*)&spotLight.worldPos, 1, 3, "gSpotLight.worldPos");
	shader.copyFloatVectorToShader((float*)&spotLight.lightDirection, 1, 3, "gSpotLight.lightDirection");
	shader.copyFloatVectorToShader((float*)&spotLight.specularPower, 1, 1, "gSpotLight.specularPower");
	//shader.copyFloatVectorToShader((float *)&spotLight.spotdDistLimit, 1, 1, "gSpotLight.spotDistLimit");
	shader.copyFloatVectorToShader((float*)&spotLight.cosSpotAlpha, 1, 1, "gSpotLight.cosSpotAlpha");

	// load directional light
	shader.copyFloatVectorToShader((float*)&directionalLight.ambientIntensity, 1, 3, "gDirectionalLight.ambientIntensity");
	shader.copyFloatVectorToShader((float*)&directionalLight.lightIntensity, 1, 3, "gDirectionalLight.lightIntensity");
	shader.copyFloatVectorToShader((float*)&directionalLight.lightDirection, 1, 3, "gDirectionalLight.lightDirection");
	shader.copyFloatVectorToShader((float*)&directionalLight.specularPower, 1, 1, "gDirectionalLight.specularPower");


	return 0;
}


/*************************************************************************************************************/

int Solution::printOpenGLError(int errorCode)
{
	switch (errorCode) {
	case GL_INVALID_VALUE:
		printf("GL_INVALID_VALUE is generated if program is not a value generated by OpenGL.\n");
		break;
	case GL_INVALID_OPERATION:
		printf("GL_INVALID_OPERATION is generated if program is not a program object. or \n");
		printf("GL_INVALID_OPERATION is generated if program has not been successfully linked. or \n");
		printf("GL_INVALID_OPERATION is generated if location does not correspond to a valid uniform variable location for the specified program object.\n");
		break;
	default:
		printf("openGL unknown error \n");
	}

	clearGLError();

	return 0;
}

/*************************************************************************************************************/

int Solution::clearGLError()
{
	for (int i = 0; i < 20; i++) {
		int rc = glGetError();
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
				printf("openGL unknown error \n");
			}
		}
		else break;
	}

	return 0;
}

/*************************************************************************************************************/

int Solution::checkGLError()
{
	int rc = glGetError();
	if (rc != GL_NO_ERROR) {
		printOpenGLError(rc);
		clearGLError();
	}

	return 0;
}