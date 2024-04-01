

//=============================================================================
// graphicsObject.c
//
//Author: Doron Nussbaum (C) 2015 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. generic class for graphics objects. 
//
//
// Description:
//--------------
//  sub classes of objects can be inherited.  The class does the main funcionality of setting and rendering objects.

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




#include "GraphicsObject.h"

/***************************************************************************/

GraphicsObject::GraphicsObject() :
	rollAngle(0),
	pitchAngle(0),
	yawAngle(0),
	position(Vector3f(0, 0, 0)),
	scale(1, 1, 1),
	worldRollAngle(0),
	worldPitchAngle(0),
	worldYawAngle(0),
	worldPosition(Vector3f(0, 0, 0)),
	worldScale(1, 1, 1),
	modelMat(Matrix4f::identity()),
	worldMat(Matrix4f::identity())
{
	m_children.resize(0);
}

/***************************************************************************/



GraphicsObject::~GraphicsObject()
{
}



/***************************************************************************/



int GraphicsObject::createVAO(Shader shader)
{
	return 0;
}


/***************************************************************************/


int GraphicsObject::createVAO(Shader shader, Vertices vtx, Indices ind)
{
	int rc = 0;
	Vertex v;


	GLint location = 0;		// location of the attributes in the shader;

	int shaderProgId = shader.getProgId();

	//create vertex array object 
	// get an object (VAO)
	glGenVertexArrays(1, &vao);
	// bind the vertex array
	glBindVertexArray(vao);

	//create vertex buffer object
	// get an object (VBO)
	glGenBuffers(1, &vtxVBO);
	// bind the buffer object
	glBindBuffer(GL_ARRAY_BUFFER, vtxVBO);
	//transfer the data
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vtx.size(), (void*)vtx.data(), GL_STATIC_DRAW);

	//set the vertex position information
	// get the location for the vertex position - match with the shader
	location = glGetAttribLocation(shaderProgId, "vtxPos");

	if (location == -1) {
		rc = -1;
		goto err;
	}

	// enable the location
	glEnableVertexAttribArray(location);
	// inform openfl abouit the attribute address
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

	/*
	//set the vertex color - repeat for vertex colour 
	location = glGetAttribLocation(shaderProgId, "vtxCol");
	printf("col-lo: %d\n", location);

	if (location == -1) {
		rc = -2;
		goto err;		
	}

	// enable the location
	glEnableVertexAttribArray(location);
	// inform openfl abouit the attribute address
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col)); */

	/*//set the vertex normal
	location = glGetAttribLocation(shaderProgId, "vtxNormal");

	if (location == -1) {
		rc = -3;
		//goto err; ------THIS BREAKS THE CODE BC THERES NOT NORMAL YET COME BACK TO REVISE!!!!!!!!!!!!!!!!!!
	}

	// enable the location
	glEnableVertexAttribArray(location);
	// inform openfl abouit the attribute address
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));*/

	//set the vertex tex coordinates
	location = glGetAttribLocation(shaderProgId, "vtxCoord");
	printf("lo: %d\n", location);

	if (location == -1) {
		rc = -3;
		//DN	goto err;
	}
	else {
		glEnableVertexAttribArray(location);
		int relAddress = (char*)v.texCoord - (char*)&v;
		printf("reladres: %d\n", relAddress);
		glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)relAddress);

		//glEnableVertexAttribArray(location);
		//glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	}

	//create index buffer
	// get a handle
	glGenBuffers(1, &indVBO);

	// bind the buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indVBO);

	// transfer the data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * ind.size(), (void*)ind.data(), GL_STATIC_DRAW);

	// store the number of indices
	numIndices = ind.size();

	//end creation unbind the vertex array buffer
	glBindVertexArray(0);

err:
	return(rc);
}




/*********************************************************************************/



int GraphicsObject::loadMaterials(Shader shader)

{
	shader.copyFloatVectorToShader((float*)&materials.ambientMaterial, 1, 3, "gMaterial.ambientMaterial");
	shader.copyFloatVectorToShader((float*)&materials.diffuseMaterial, 1, 3, "gMaterial.diffuseMaterial");
	shader.copyFloatVectorToShader((float*)&materials.specularMaterial, 1, 3, "gMaterial.specularMaterial");
	shader.copyFloatVectorToShader((float*)&materials.interalRadiation, 1, 3, "gMaterial.interalRadiation");

	return(0);
}

/*********************************************************************************/


int GraphicsObject::render()
{
#if 0
	Matrix4f rotMat;  // rotation matrix;
	Matrix4f scaleMat; // scaling matrix;
	Matrix4f transMat;	// translation matrix
	Matrix4f modelMat;	// final model matrix

	// set the transformation matrix - the model transfomration
	modelMat = Matrix4f::identity(); // = Matrix4f::rotateRollPitchYaw(rollAngle, pitchAngle, yawAngle, 1);

	// set the scaling - this is model space to model space transformation
	scaleMat = Matrix4f::scale(scale.x, scale.y, scale.z);
	modelMat = scaleMat * modelMat;

	// set the rotation  - this is model space to model space transformation 
	rotMat = Matrix4f::rotateRollPitchYaw(rollAngle, pitchAngle, yawAngle, 1);
	// note that we always multiply the new matrix on the left
	modelMat = rotMat * modelMat;


	// set the translation - this is model space to world space transformation
	transMat = Matrix4f::translation(position);
	modelMat = transMat * modelMat;


	// draw using indices
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);


#endif
	return 0;
}

/*********************************************************************************/


int GraphicsObject::render(Matrix4f worldMat)
{
#if 0
	Matrix4f rotMat;  // rotation matrix;
	Matrix4f scaleMat; // scaling matrix;
	Matrix4f transMat;	// translation matrix
	Matrix4f modelMat;	// final model matrix

	// set the transformation matrix - the model transfomration
	modelMat = Matrix4f::identity(); // = Matrix4f::rotateRollPitchYaw(rollAngle, pitchAngle, yawAngle, 1);

	// set the scaling - this is model space to model space transformation
	scaleMat = Matrix4f::scale(scale.x, scale.y, scale.z);
	modelMat = scaleMat * modelMat;

	// set the rotation  - this is model space to model space transformation 
	rotMat = Matrix4f::rotateRollPitchYaw(rollAngle, pitchAngle, yawAngle, 1);
	// note that we always multiply the new matrix on the left
	modelMat = rotMat * modelMat;


	// set the translation - this is model space to world space transformation
	transMat = Matrix4f::translation(position);
	modelMat = transMat * modelMat;


	// draw using indices
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);


#endif
	return 0;
}

/*************************************************************/
// sets the initial orientation

void GraphicsObject::setModelRotations(float rollAngle, float pitchAngle, float yawAngle)
{
	this->rollAngle = rollAngle;
	this->pitchAngle = pitchAngle;
	this->yawAngle = yawAngle;

	computeModelMat();
}

/*************************************************************************/

// set the initial position

void GraphicsObject::setModelPosition(Vector3f position)
{
	this->position = position;
	computeModelMat();
}

/*************************************************************/
// sets the initial orientation

void GraphicsObject::setWorldRotations(float rollAngle, float pitchAngle, float yawAngle)
{
	this->worldRollAngle = rollAngle;
	this->worldPitchAngle = pitchAngle;
	this->worldYawAngle = yawAngle;

	computeWorldMat();
}

/*************************************************************************/

// set the initial position

void GraphicsObject::setWorldPosition(Vector3f position)
{
	this->worldPosition = position;
	computeWorldMat();
}



/*************************************************************/
// increment the courrent rotation by the given amounts

void GraphicsObject::incrementWorldRotations(float rollAngle, float pitchAngle, float yawAngle)
{
	this->worldRollAngle += rollAngle;
	this->worldPitchAngle += pitchAngle;
	this->worldYawAngle += yawAngle;

	computeWorldMat();
}


/*************************************************************************/

// set the initial position

void GraphicsObject::setModelPosition(float x, float y, float z)
{
	this->position = Vector3f(x, y, z);
	computeModelMat();
}

/*************************************************************/
// increment the courrent rotation by the given amounts

void GraphicsObject::incrementModelRotations(float rollAngle, float pitchAngle, float yawAngle)
{
	this->rollAngle += rollAngle;
	this->pitchAngle += pitchAngle;
	this->yawAngle += yawAngle;

	computeModelMat();
}

/*************************************************************************/

// increment positin by delta position

void GraphicsObject::incrementModelPosition(Vector3f deltaPosition)
{
	this->position += deltaPosition;
	computeModelMat();
}

/*************************************************************************/

// increment position by delta

void GraphicsObject::incrementModelPosition(float deltaX, float deltaY, float deltaZ)
{
	this->position += Vector3f(deltaX, deltaY, deltaZ);
	computeModelMat();
}


/*************************************************************************/

// set the initial position

void GraphicsObject::setModelScale(Vector3f scale)
{
	this->scale = scale;
	computeModelMat();
}

/*************************************************************************/

// set the initial position

void GraphicsObject::setModelScale(float scaleX, float scaleY, float scaleZ)
{
	this->scale = Vector3f(scaleX, scaleY, scaleZ);
	computeModelMat();
}

/*************************************************************************/

// set the initial position

void GraphicsObject::incrementModelScale(float scaleX, float scaleY, float scaleZ)
{
	this->scale.x += scaleX;
	this->scale.y += scaleY;
	this->scale.z += scaleZ;
	computeModelMat();
}


/*************************************************************************/

// Computer the model transformation matrix

void GraphicsObject::computeModelMat()
{
	Matrix4f rotMat;  // rotation matrix;
	Matrix4f scaleMat; // scaling matrix;
	Matrix4f transMat;	// translation matrix

	// set the transformation matrix - the model transfomration
	modelMat = Matrix4f::identity(); // = Matrix4f::rotateRollPitchYaw(rollAngle, pitchAngle, yawAngle, 1);

	// set the scaling - this is model space to model space transformation
	scaleMat = Matrix4f::scale(scale.x, scale.y, scale.z);
	modelMat = scaleMat * modelMat;

	// set the rotation  - this is model space to model space transformation 
	rotMat = Matrix4f::rotateRollPitchYaw(rollAngle, pitchAngle, yawAngle, 1);
	// note that we always multiply the new matrix on the left
	modelMat = rotMat * modelMat;


	// set the translation - this is model space to world space transformation
	transMat = Matrix4f::translation(position);
	modelMat = transMat * modelMat;
}


/*************************************************************************/

// set the world transformation matrix

void GraphicsObject::computeWorldMat()
{
	Matrix4f rotMat;  // rotation matrix;
	Matrix4f scaleMat; // scaling matrix;
	Matrix4f transMat;	// translation matrix

	// set the transformation matrix - the model transfomration
	worldMat = Matrix4f::identity(); // = Matrix4f::rotateRollPitchYaw(rollAngle, pitchAngle, yawAngle, 1);

	// set the scaling - this is model space to model space transformation
	scaleMat = Matrix4f::scale(worldScale.x, worldScale.y, worldScale.z);
	worldMat = scaleMat * worldMat;

	// set the rotation  - this is model space to model space transformation 
	rotMat = Matrix4f::rotateRollPitchYaw(worldRollAngle, worldPitchAngle, worldYawAngle, 1);
	// note that we always multiply the new matrix on the left
	worldMat = rotMat * worldMat;


	// set the translation - this is model space to world space transformation
	transMat = Matrix4f::translation(worldPosition);
	worldMat = transMat * worldMat;
}





/*********************************************************************************/
// adds a child to the list of chilren

void GraphicsObject::addChild(GraphicsObject* obj)
{

	m_children.push_back(obj);
}
