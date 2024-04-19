

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
	winid1 = glutCreateWindow("Water Pool Animation");
	checkGLError();

	//	glutSetWindow(winid1);
	glClearColor(0.0, 0.0, 0.0, 1.0);
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
	glutMouseFunc(Solution::mouseCB);
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
	factor = 1;

	Vertices floorvtx;
	Indices floorind;

	Vertices spherevtx;
	Indices sphereind;

	Vertices polevtx;
	Indices poleind;
	
	const char* sbTextureNameSunnyDay[6] = {
		"TropicalSunnyDayLeft2048.png",
		"TropicalSunnyDayRight2048.png",
		"TropicalSunnyDayUp2048.png",
		"grasstext.png",
		"TropicalSunnyDayFront2048.png",
		"grasstext.png" };

	waterfloor.createSurface(50, 50, 0, 1, 0, 1, vtx, ind);
	floor.createSurface(10, 10, 0, 5, 0, 10, floorvtx, floorind);
	Water::createSphere(10, 10, spherevtx, sphereind);
	pole.init3dgeom(polevtx, poleind);
	//model to model
	

	// create the water shader object
	rc = waterShader.createShaderProgram("waterShader.vs", "waterShader.fs");
	if (rc != 0) {
		fprintf(stderr, "Error in generating shader (solution)\n");
		rc = -1;
		goto err;
	}
	checkGLError();

	waterShaderProgId = waterShader.getProgId();

	// create the water VAO  using geometryObject createVAO(shaderProgId,...)
	waterfloor.createVAO(waterShader, vtx, ind);
	waterfloor.setModelScale(150, 1, 150);
	waterfloor.setWorldRotations(0, 90, 45);
	waterfloor.setWorldPosition(Vector3f(0, 0, 5));//model to world

	// create the FLOOR shader object
	rc = floorShader.createShaderProgram("floorShader.vs", "floorShader.fs");
	if (rc != 0) {
		fprintf(stderr, "Error in generating shader (solution)\n");
		rc = -1;
		goto err;
	}
	checkGLError();
	floorShaderProgId = floorShader.getProgId();

	// create the floor VAO  using geometryObject createVAO(shaderProgId,...)
	floor.createVAO(floorShader, floorvtx, floorind);
	floor.setModelScale(200, 200, 200);
	floor.setWorldRotations(0, 90, 45);
	//model to world
	floor.setWorldPosition(Vector3f(0, 0, 0));

	// create the SPHERE shader object
	rc = sphereShader.createShaderProgram("sphereShader.vs", "sphereShader.fs");
	if (rc != 0) {
		fprintf(stderr, "Error in generating shader (solution)\n");
		rc = -1;
		goto err;
	}
	checkGLError();
	sphereShaderProgId = sphereShader.getProgId();
	//sphere setup
	sphere.createVAO(sphereShader, spherevtx, sphereind);
	sphere.setModelScale(20, 20, 20);
	sphere.setWorldRotations(1, 1, 1);
	sphere.setModelPosition(Vector3f(-200, 0, -17));

	// create the POLE shader object
	rc = poleShader.createShaderProgram("poleShader.vs", "poleShader.fs");
	if (rc != 0) {
		fprintf(stderr, "Error in generating shader (solution)\n");
		rc = -1;
		goto err;
	}
	checkGLError();
	poleShaderProgId = poleShader.getProgId();

	// create the floor VAO  using geometryObject createVAO(shaderProgId,...)
	pole.createVAO(poleShader, polevtx, poleind);
	pole.setModelScale(5, 5, 5);
	pole.setWorldRotations(0, 90, 45);
	//model to world
	pole.setModelPosition(Vector3f(0, 0, -200));
	checkGLError(); 
	// set the camera initial position
	//cam.setCamera(Vector3f(15, 15, 70), Vector3f(15, 0, 0), Vector3f(0, 1, 0));
	cam.setCamera(Vector3f(0, 500, 1), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

	//load textures
	watertexture.loadTextures("water_pic.jpg", GL_TEXTURE_2D);
	//watertexture.loadTextures("water2.jpg", GL_TEXTURE_2D);
	//watertexture.loadTextures("clear-ocean-water-texture.jpg", GL_TEXTURE_2D);
	//floortexture.loadTextures("grasstext.png", GL_TEXTURE_2D);
	floortexture.loadTextures("floor_tile.jpg", GL_TEXTURE_2D);

	spheretexture.loadTextures("BeachBallTexture2.jpg", GL_TEXTURE_2D);


	//skybox inits
	
	/*skybox.init("skybox.vs", "skybox.fs");
	skybox.loadColourTexture();
	skybox.loadTextureImages(sbTextureNameSunnyDay);*/
	//floortexture.bindToTextureUnit(GL_TEXTURE0);
	//setthe point light using the pointLight structure
	pointLight.ambientIntensity = Vector3f(0.5, 0.5, 0.5);
	pointLight.lightIntensity = Vector3f(2, 2, 2);
	pointLight.specularPower = 40;
	pointLight.worldPos = Vector3f(0, 0, -100);


	// load the flags for showing hte components
	includeAmbient = 1;
	includeDiffuse = 1;
	includeSpecular = 1;
	includeObjColour = 1;

	lightType = POINT_LIGHT;

	factor = 1;		// motion and specular factor

	includeSpecular = includeObjColour = 1;
	plotWireFrame = 0;

err:
	return 0;
}


/**********************************************************************/

void Solution::setSolution(Solution* _sol)
{
	Solution::sol = _sol;
}



/************************************************************/

// render function.  


int Solution::render()
{
	//shader use code
	time++;		// increament the time

	Vector3f viewerPosition;
	Vector3f lookAtPoint;
	Vector3f upVector;;
	Matrix4f viewMat, projMat;
	int location = 0;

	/*
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTexHandle());*/
	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (!plotWireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/*skybox.render(cam);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTexHandle());*/
	// ------------------------RENDER THE WATER--------------------
	glUseProgram(waterShaderProgId);

	
	// set the view matrix
	viewMat = cam.getViewMatrix(NULL);	// get the camera view transformation
	waterShader.copyMatrixToShader(viewMat, "view");
	assert(location != -1);
	if (location == -1) return (-1);


	// set the projection matrix
	projMat = cam.getProjectionMatrix(NULL);
	waterShader.copyMatrixToShader(projMat, "projection");
	assert(location != -1);
	if (location == -1) return (-1);


	waterShader.copyIntVectorToShader(&time, 1, 1, "time");

	//  load the light data
	loadLights(waterShader);
	viewerPosition = cam.getPosition();
	// get the location of viewerPosition
	location = glGetUniformLocation(waterShaderProgId, "gEyeWorldPos");
	if (location == -1) return (-1);
	glUniform3fv(location, 1, (float*)&viewerPosition);
	Solution::clearGLError();
	// render the objects

	//floortexture.bindToTextureUnit(GL_TEXTURE0);
	watertexture.bindToTextureUnit(GL_TEXTURE1);
	watertexture.setTextureSampler(waterShader, "texSampler1", 1);
	waterfloor.render(waterShader);
	

	

	// ------------------------RENDER THE SPHERE--------------------
	glUseProgram(sphereShaderProgId);


	// set the view matrix
	viewMat = cam.getViewMatrix(NULL);	// get the camera view transformation
	sphereShader.copyMatrixToShader(viewMat, "view");
	assert(location != -1);
	if (location == -1) return (-1);


	// set the projection matrix
	projMat = cam.getProjectionMatrix(NULL);
	sphereShader.copyMatrixToShader(projMat, "projection");
	assert(location != -1);
	if (location == -1) return (-1);


	sphereShader.copyIntVectorToShader(&time, 1, 1, "time");
	spheretexture.bindToTextureUnit(GL_TEXTURE3);
	spheretexture.setTextureSampler(sphereShader, "texSampler1", 3);
	sphere.render(sphereShader);


	// ------------------------RENDER THE POLE--------------------
	glUseProgram(poleShaderProgId);


	// set the view matrix
	viewMat = cam.getViewMatrix(NULL);	// get the camera view transformation
	poleShader.copyMatrixToShader(viewMat, "view");
	assert(location != -1);
	if (location == -1) return (-1);


	// set the projection matrix
	projMat = cam.getProjectionMatrix(NULL);
	poleShader.copyMatrixToShader(projMat, "projection");
	assert(location != -1);
	if (location == -1) return (-1);


	poleShader.copyIntVectorToShader(&time, 1, 1, "time");


	// render the objects
	//  load the light data
	// load the ambient light
	location = glGetUniformLocation(poleShaderProgId, "gPointLight.ambientIntensity");
	if (location == -1) return (-1);
	glUniform3fv(location, 1, (float*)&pointLight.ambientIntensity);

	Solution::clearGLError();
	// load the light intensity using "gPointLight.lightIntensity" as the name
	location = glGetUniformLocation(poleShaderProgId, "gPointLight.lightIntensity");
	if (location == -1) return (-1);
	glUniform3fv(location, 1, (float*)&pointLight.lightIntensity);

	Solution::clearGLError();


	// load the specular light
	location = glGetUniformLocation(poleShaderProgId, "gPointLight.specularPower");
	if (location == -1) return (-1);
	glUniform1fv(location, 1, &pointLight.specularPower);
	Solution::clearGLError();


	// load the light position 
	location = glGetUniformLocation(poleShaderProgId, "gPointLight.worldPos");
	if (location == -1) return (-1);
	glUniform3fv(location, 1, (float*)&pointLight.worldPos);
	Solution::clearGLError();


	// load the viewer position
	// get the viewerposition from the camera
	viewerPosition = cam.getPosition();

	// get the location of viewerPosition
	location = glGetUniformLocation(poleShaderProgId, "gEyeWorldPos");
	if (location == -1) return (-1);
	glUniform3fv(location, 1, (float*)&viewerPosition);
	// // transfer the viewer position to the sh ader program
	// 
	Solution::clearGLError();



	// load the flags for showing hte components
	includeAmbient = 1;
	includeDiffuse = 1;
	includeSpecular = 1;
	includeObjColour = 1;
	poleShader.copyIntVectorToShader(&includeAmbient, 1, 1, "includeAmbient");
	poleShader.copyIntVectorToShader(&includeDiffuse, 1, 1, "includeDiffuse");
	poleShader.copyIntVectorToShader(&includeSpecular, 1, 1, "includeSpecular");
	poleShader.copyIntVectorToShader(&includeObjColour, 1, 1, "includeObjColour");
	
	pole.render(poleShader);

	// ------------------------RENDER THE FLOOR--------------------
	glUseProgram(floorShaderProgId);


	// set the view matrix
	viewMat = cam.getViewMatrix(NULL);	// get the camera view transformation
	floorShader.copyMatrixToShader(viewMat, "view");
	assert(location != -1);
	if (location == -1) return (-1);


	// set the projection matrix
	projMat = cam.getProjectionMatrix(NULL);
	floorShader.copyMatrixToShader(projMat, "projection");
	assert(location != -1);
	if (location == -1) return (-1);


	floorShader.copyIntVectorToShader(&time, 1, 1, "time");

	// render the objects

	//floortexture.bindToTextureUnit(GL_TEXTURE0);
	floortexture.bindToTextureUnit(GL_TEXTURE2);
	floortexture.setTextureSampler(floorShader, "texSampler1", 2);
	floortexture.setTextureSampler(floorShader, "texSampler2", 4);

	//  load the light data
	loadLights(floorShader);
	viewerPosition = cam.getPosition();
	// get the location of viewerPosition
	location = glGetUniformLocation(floorShaderProgId, "gEyeWorldPos");
	if (location == -1) return (-1);
	glUniform3fv(location, 1, (float*)&viewerPosition);
	Solution::clearGLError();
	floor.render(floorShader);
	glutSwapBuffers();
	return 0; 
}


/************************************************************/

// passive motion function. 

int mouse = 0;
int prev = 0;
int prevy = 0;
int mouseR = 0;


void Solution::mouseCB(int button, int state, int mousex, int mousey) {
	sol->mouseclicks(button,  state,  mousex,  mousey);

}

void Solution::mouseclicks(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (mouse == 1) {
			mouse = 0;
		}
		else if(mouse==0){
			mouse = 1;
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (mouseR == 1) {
			mouseR = 0;
		}
		else if (mouseR == 0) {
			mouseR = 1;
		}
	}

	//printf("x =%d, y = %d \n", x, y);
}

void Solution::passiveMouse(int x, int y)
{
	int winid = glutGetWindow();
	//std::cout << "Passive Mouse winId=" << winid << "(" << x << " , " << y << ")" << std::endl;
	if (mouse == 1) {
		if (prev > x) {
			waterfloor.incrementWorldRotations(0, 0, 0.01);
			floor.incrementWorldRotations(0, 0, 0.01);
			sphere.incrementWorldRotations(0, 0, 0.01);
			//pole.incrementWorldRotations(0, 0, 0.01);


		}
		if(prev < x) {
			waterfloor.incrementWorldRotations(0, 0, -0.01);
			floor.incrementWorldRotations(0, 0, -0.01);
			sphere.incrementWorldRotations(0, 0, -0.01);
			//pole.incrementWorldRotations(0, 0, -0.01);

		}
		if (prevy > y) {
			waterfloor.incrementWorldRotations(0, -0.01, 0);
			floor.incrementWorldRotations(0, -0.01, 0);
			sphere.incrementWorldRotations(0, -0.01, 0);
			//pole.incrementWorldRotations(0, -0.01, 0);

		}
		if (prevy < y) {
			waterfloor.incrementWorldRotations(0, 0.01, 0);
			floor.incrementWorldRotations(0, 0.01, 0);
			sphere.incrementWorldRotations(0, 0.01, 0);
			//pole.incrementWorldRotations(0, 0.01, 0);

		}
		prev=x;
		prevy = y;
		printf("x =%d, y = %d \n", x, y);

	}

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
	case '.':
		mouse = 1;
		break;
	case ',':
		mouse = 0;
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
		//cam.roll(1);
		pole.incrementModelPosition(Vector3f(-5, 0, 0));
		pointLight.worldPos = pointLight.worldPos + Vector3f(-5,0,0);
		break;
	case GLUT_KEY_UP:
		pole.incrementModelPosition(Vector3f(0, -5, 0));
		pointLight.worldPos = pointLight.worldPos + Vector3f(0, -5, 0);
		//cam.pitch(1.0);
		break;
	case GLUT_KEY_RIGHT:
		//cam.roll(-1);
		pole.incrementModelPosition(Vector3f(5, 0, 0));
		pointLight.worldPos = pointLight.worldPos + Vector3f(5, 0, 0);

		break;
	case GLUT_KEY_DOWN:
		pole.incrementModelPosition(Vector3f(0, 5, 0));
		pointLight.worldPos = pointLight.worldPos + Vector3f(0, 5, 0);

		//cam.pitch(-1);
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


	shader.copyIntVectorToShader(&includeAmbient, 1, 1, "includeAmbient");
	shader.copyIntVectorToShader(&includeDiffuse, 1, 1, "includeDiffuse");
	shader.copyIntVectorToShader(&includeSpecular, 1, 1, "includeSpecular");
	shader.copyIntVectorToShader(&includeObjColour, 1, 1, "includeObjColour");


	return 0;
}


/*************************************************************************************************************/

int Solution::printOpenGLError(int errorCode)
{
	switch (errorCode) {
	case GL_INVALID_VALUE:
		printf("GL_INVALID_VALUE is generated if program is not a value generated by OpenGL3.\n");
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
				printf("GL_INVALID_VALUE is generated if program is not a value generated by OpenGL2.\n");
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
