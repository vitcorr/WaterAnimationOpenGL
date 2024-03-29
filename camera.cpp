
//=============================================================================
// camera.cpp 
//
//Author: Doron Nussbaum (C) 2011 All Rights Reserved.
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


#include "camera.h"


/******************************************************************/
/*
Purpose: camera constructor


Descripton:

Return:


*/

Camera::Camera(void) : position(0.0, 0.0, 0.0), lookAtVector(0.0, 0.0, 100.0), upVector(0.0, 1.0, 0.0)
, speed(0), nearPlane(0.1), farPlane(1000), fieldOfView(60), aspectRatio(1.0)
{
	viewMat = Matrix4f::cameraMatrix(position, lookAtVector, upVector);
	projMat = Matrix4f::symmetricPerspectiveProjectionMatrix(fieldOfView, aspectRatio, nearPlane, farPlane);
}

/******************************************************************/
/*
Purpose: camera destructor


Descripton:

Return:


*/
Camera::~Camera(void)
{
}


/******************************************************************/
/*
Purpose: change the orientation of the Camera (roll transformation)


Descripton:

Return:
1 - if failed
0 - if successful

*/


int Camera::roll(float angleDeg)
{
	Vector4f u;

	// create rotation matrix
	rotMat = Matrix4f::rotateVector(-lookAtVector, angleDeg, 1);

	// rotate the camera (up vector and/or looAtVector)
	u = rotMat * upVector;
	upVector = u.to3D();

	// update the view matrix
	viewMat = Matrix4f::cameraMatrix(position, position + lookAtVector, upVector);

	return 0;
}


/******************************************************************/
/*
Purpose: change the orientation of the Camera (pitch transformation)


Descripton:

Return:
1 - if failed
0 - if successful

*/

int Camera::pitch(float angleDeg)
{
	Vector3f rotVector(0.0, 0.0, 0.0);		// DN 26/10/2015
	Vector4f u;

	// get rotation axis
	rotVector = Vector3f::cross(lookAtVector, upVector);	// DN 26/10/2015

	// create rotation matrix
	rotMat = Matrix4f::rotateVector(rotVector, angleDeg, 1);
	// rotate the camera (up vector and/or looAtVector)
	u = rotMat * upVector;
	upVector = u.to3D();

	// rotate the lookAtVector
	u = rotMat * lookAtVector;
	lookAtVector = u.to3D();

	// update the view matrix
	viewMat = Matrix4f::cameraMatrix(position, position + lookAtVector, upVector);

	return 0;
}

/******************************************************************/
/*
Purpose: change the orientation of the camera (yaw transformation)


Descripton:

Return:
1 - if failed
0 - if successful

*/



int Camera::yaw(float angleDeg)
{
	Vector4f u;


	// create rotation matrix
	rotMat = Matrix4f::rotateVector(upVector, angleDeg, 1);

	// rotate the lookAtVector
	u = rotMat * lookAtVector;
	lookAtVector = u.to3D();

	// update the view matrix
	viewMat = Matrix4f::cameraMatrix(position, position + lookAtVector, upVector);

	return 0;
}

/******************************************************************/
/*
Purpose: obtains the current position of the camera


Descripton:

Return:
Vector3f - camera position

*/

Vector3f Camera::getPosition(void)
{
	Vector3f v = position;	// DN 26/10/2015
	return (v);
}


/******************************************************************/
/*
Purpose: obtains the lookAt point of the camera


Descripton:

Return:
Vector3f - camera lookAt point

*/


Vector3f Camera::getLookAtPoint(void)	// DN 26/10/2015
{
	return (position + lookAtVector);
}

/******************************************************************/
/*
Purpose: obtains the camera's up vector


Descripton:

Return:
Vector3f - Camera upVector

*/



Vector3f Camera::getUpVector(void)		// DN 26/10/2015
{
	return (upVector);
}


/******************************************************************/
/*
Purpose: changes the camera's position relative to its current position


Descripton:

Return:
0 - success

*/


int Camera::changePositionDelta(float dx, float dy, float dz)
{
	position.x += dx;
	position.y += dy;
	position.z += dz;

	// update the view matrix
	viewMat = Matrix4f::cameraMatrix(position, position + lookAtVector, upVector);


	return 0;
}


/******************************************************************/
/*
Purpose: changes the camera's position relative to its current position


Descripton:

Return:
0 - success

*/


int Camera::changePositionDelta(Vector3f* dv)
{
	position += *dv;
	// update the view matrix
	viewMat = Matrix4f::cameraMatrix(position, position + lookAtVector, upVector);

	return 0;
}

/******************************************************************/
/*
Purpose: changes the camera's position to a new position


Descripton:

Return:
0 - success

*/


int Camera::changeAbsPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;

	// update the view matrix
	viewMat = Matrix4f::cameraMatrix(position, position + lookAtVector, upVector);

	return 0;
}

/******************************************************************/
/*
Purpose: changes the camera's position to a new position


Descripton:

Return:
0 - success

*/

int Camera::changeAbsPosition(Vector3f* v)
{
	position = *v;	// DN 26/10/2015

	// update the view matrix
	viewMat = Matrix4f::cameraMatrix(position, lookAtVector, upVector);

	return 0;
}

/******************************************************************/
/*
Purpose: changes the camera's position along the LootAt vector


Descripton: move the camera forward by the numUnits along the looAtVector

Return:
the new position

*/


Vector3f Camera::moveForward(float numUnits)
{
	position += lookAtVector * numUnits;

	// update the view matrix
	viewMat = Matrix4f::cameraMatrix(position, position + lookAtVector, upVector);

	return (position);	// DN 26/10/2015
}



/******************************************************************/
/*
Purpose: changes the camera's position along the LootAt vector


Descripton: move the camera forward by the numUnits along the looAtVector

Return:
the new position

*/


Vector3f Camera::moveBackward(float numUnits)
{
	return(moveForward(-numUnits));

}


/******************************************************************/
/*
Purpose: update the up vector of the camera


Descripton:

Return:


*/


int Camera::updateOrientation(Vector3f rotVector, float angleRad)
{

	Vector3f xaxis(0.0, 0.0, 0.0);
	Vector4f u;

	// create rotation matrix
	rotMat = Matrix4f::rotateVector(rotVector, angleRad, 0);
	// rotate the camera (up vector and/or looAtVector)
	u = rotMat * upVector;
	upVector = u.to3D();
	u = rotMat * lookAtVector;
	lookAtVector = u.to3D();	// DN 26/10/2015

	// update the upVector
	xaxis = Vector3f::cross(upVector, lookAtVector);
	upVector = Vector3f::cross(lookAtVector, xaxis);
	upVector.normalize();
	lookAtVector.normalize();
	return 0;
}

/******************************************************************/
/*
Purpose: obtains the view transformation matrix


Descripton:

Return:
the camera transformation matrix

*/



Matrix4f Camera::getViewMatrix(Matrix4f* viewMatrix)
{
	Vector3f lookAtPoint;
	Matrix4f m;

	lookAtPoint = position + lookAtVector;
	m = Matrix4f::cameraMatrix(position, lookAtPoint, upVector);
	if (viewMatrix != NULL) *viewMatrix = m;

	return(m);
}



/*********************************************************************************/

/*
Purpose: set the camera parameters


input:
position - the camera position
lookAtPoint - the point that the camera is looking at
upVector - the up vector of the camera

Return:
none

*/


void Camera::setCamera(Vector3f position, Vector3f lookAtPoint, Vector3f upVector)
{
	this->position = position;
	this->lookAtVector = lookAtPoint - position;
	this->upVector = upVector;
	this->upVector.normalize();
	this->lookAtVector.normalize();
	// updatre the view matrix
	viewMat = Matrix4f::cameraMatrix(position, position + lookAtVector, upVector);

}


/*********************************************************************************/


/*
Purpose: updates the speed of the camera motion (i.e., number of units that the
camera would move at every time tick

input:
speed - the number of units by which the camera would move at every time tick

output:
none

return:
none
*/
int Camera::updateSpeed(float speed)
{
	this->speed += speed;
	return 0;
}


/*********************************************************************************/


/*
Purpose: returns the camera speed

input:
none

output
none

return:
speed - the speed of the camera


*/
float Camera::getSpeed(void)
{
	return(speed);
}


/*********************************************************************************/

/*
Purpose: sets the look at at vector

input :
x,y,z - the components of the lookAtVector along the x, y and z axis

Output :
none

Return :
0 -

Restrictions
The camera is limited by how much it can change the field of view

*/

int Camera::changeLookAtVector(float x, float y, float z)
{
	lookAtVector.x = x;
	lookAtVector.y = y;
	lookAtVector.z = z;
	// update the view matrix
	viewMat = Matrix4f::cameraMatrix(position, position + lookAtVector, upVector);

	return 0;
}



/*********************************************************************************/


/*

Purpose: Moving the camera sideways to the right by numUnits and updating the camera matrix

input:
numUnits - the number of units by which the camera will be moved to the right

Output:
none

Return:
The new position of the camera

*/
Vector3f Camera::moveRight(float numUnits)
{
	// compute the the moving direction
	Vector3f moveVector;
	// moveVector = Vector3f::cross(upVector, lookAtVector);
	moveVector = Vector3f::cross(lookAtVector, upVector);
	position += moveVector * numUnits;
	// update the view matrix
	viewMat = Matrix4f::cameraMatrix(position, position + lookAtVector, upVector);
	return (position);	// DN 26/10/2015
}


/*********************************************************************************/


/*
Purpose: Moving the camera sideways to the left by numUnits and updating the camera matrix

input:
numUnits - the number of units by which the camera will be moved to the left

Output:
none

Return:
The new position of the camera

*/
Vector3f Camera::moveLeft(float numUnits)
{
	return(this->moveRight(-numUnits));
}


/****************************************************************************************/
// zoom functions

/**************************************************************************************************/
/*
Purpose: zoom in by changing the field of view by 1 unit and updating the camera matrix

input :
numUnits - the number of units by which the camera will be moved to the left

Output :
none

Return :
0

*/
int Camera::zoomIn(void)
{
	zoomIn(1);
	return 0;
}


/**************************************************************************************************/

/*
Purpose: zoom in by changing the field of view and updating the camera matrix

input :
numUnits - the number of units by which the camera will hange he field of view

Output :
none

Return :
0 -

Restrictions
The camera is limited by how much it can change the field of view

*/
int Camera::zoomIn(float zoom)
{
	fieldOfView -= zoom;
	if (fieldOfView < 10) fieldOfView = 10;
	projMat = Matrix4f::symmetricPerspectiveProjectionMatrix(fieldOfView, aspectRatio, nearPlane, farPlane);
	return 0;
}


/**************************************************************************************************/
/*
Purpose: zoom out by changing the field of view by 1 unit and updating the camera matrix

input :
numUnits - the number of units by which the camera will be moved to the left

Output :
none

Return :
0

*/
int Camera::zoomOut(void)
{
	zoomOut(1);
	return 0;
}

/**************************************************************************************************/

/*
Purpose: zoom out by changing the field of view and updating the camera matrix

input :
numUnits - the number of units by which the camera will hange he field of view

Output :
none

Return :
0 -

Restrictions
The camera is limited by how much it can change the field of view

*/

int Camera::zoomOut(float zoom)
{
	fieldOfView += zoom;
	if (fieldOfView > 150) fieldOfView = 150;
	projMat = Matrix4f::symmetricPerspectiveProjectionMatrix(fieldOfView, aspectRatio, nearPlane, farPlane);
	return 0;
}


/**************************************************************************************************/
/**************************************************************************************************/

/*
Purpose: sets the perspective projection matrix of the camera

input :
numUnits - the number of units by which the camera will hange he field of view

Output :
none

Return :
0 -

Restrictions


*/

int Camera::setPerspectiveView(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	this->fieldOfView = fieldOfView;
	this->aspectRatio = aspectRatio;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	projMat = Matrix4f::symmetricPerspectiveProjectionMatrix(fieldOfView, aspectRatio, nearPlane, farPlane);

	return 0;
}

/**************************************************************************************************/
/******************************************************************/
/*
Purpose: obtains the projection transformation matrix


Descripton:

Return:
the transformation matrix

*/



Matrix4f Camera::getProjectionMatrix(Matrix4f* projMatrix)
{
	Matrix4f m;

	m = Matrix4f::symmetricPerspectiveProjectionMatrix(fieldOfView, aspectRatio, nearPlane, farPlane);
	if (projMatrix != NULL) *projMatrix = m;

	return(m);
}

