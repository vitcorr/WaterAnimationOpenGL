//=============================================================================
// camera.h 
//
//Author: Doron Nussbaum (C) 2014 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. demonstarte the usage of a camera
//
//
// Description:
//--------------
// A simple application that demonstrates how to crete and use a camera 
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes and for assignments in the gaming courses at 
// the School of Compuer Science at Carleton University.
// Usage of code for other purposes is not allowed without a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

// Revsions:
// 1. Created - 10/2012 Doron Nussbaum
// 2. October 26, 2015 Doron Nussbaum - modifed geometry of the camera from 4D to 3D coordinates. 


//=============================================================================

#pragma once
#include "GL/nuss_matrix.h"

class Camera
{
public:
	Camera(void);
	~Camera(void);
	int roll(float angleDeg);
	int pitch(float angleDeg);
	int yaw(float angleDeg);
	Vector3f getPosition(void);		// DN 26/10/2015
	Vector3f getLookAtPoint(void);	// DN 26/10/2015
	Vector3f getUpVector(void);		// DN 26/10/2015
	float getSpeed(void);


	int changePositionDelta(float dx, float dy, float dz);	// change position by relative amount
	int changePositionDelta(Vector3f* dv);				// change position by relative amount
	int changeAbsPosition(float x, float y, float z);		// change to a new position in space
	int changeAbsPosition(Vector3f* v);					// change to a new position in space
	int changeAbsPosition(Vector3f v);					// change to a new position in space
	void setCamera(Vector3f position, Vector3f lookAtPoint, Vector3f upVector);
	Vector3f moveForward(float numUnits);	// moves the camera forward by the numUnits units along the lookAtVector
	Vector3f moveBackward(float numUnits);  // moves the camera backward numUnits units along the lookAtVector
	Vector3f moveRight(float numUnits);
	Vector3f moveLeft(float numUnits);
	//void setCamera(Vector4f position, Vector4f lookAtPoint, Vector4f upVector); // DN 26/10/2015
	int updateSpeed(float speed);
	int changeLookAtVector(float x, float y, float z);
	int zoomIn(void);
	int zoomIn(float zoom);
	int zoomOut(void);
	int zoomOut(float zoom);
	Matrix4f getViewMatrix(Matrix4f* viewMatrix);
	Matrix4f getProjectionMatrix(Matrix4f* projMatrix);

	int setPerspectiveView(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);

private:
	int updateOrientation(Vector3f rotVector, float angleRad); // update the camera's orientation in space // DN 26/10/2015



private:
	Matrix4f viewMat;
	Matrix4f projMat;
	Matrix4f rotMat;
	Vector3f position;		// DN 26/10/2015
	Vector3f upVector;		// DN 26/10/2015
	Vector3f lookAtVector;	// DN 26/10/2015
	float speed;

	// perspective projection fields
	float fieldOfView;
	float aspectRatio;
	float nearPlane;
	float farPlane;
};
