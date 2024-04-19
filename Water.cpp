
//=============================================================================
// Surface.c
//
//Author: Doron Nussbaum (C) 2015 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. creates a Surface object. 
//
//
// Description:
//--------------
//  Creates Surface geometry.  Currently it is used only to create a geometry of a Surface. It can be converted to a stand alone function. 
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



/*********************************************************************************/
// INCLUDE FILES

#include "Water.h"
#include <iostream>
using namespace std;


/*********************************************************************************/
// DEFINES
#define MIN_X_RANGE -1.0
#define MAX_X_RANGE 1.0

#define MIN_Z_RANGE -1.0
#define MAX_Z_RANGE 1.0

/*********************************************************************************/


/*********************************************************************************/

Water::Water()
{
}


Water::~Water()
{
}





/***************************************************************************************************************/
/*
// Function creates a Surface geometry

input:
numRows - number of rows in the surface
numCols - number of columns in the surface


*/

int Water::createSurface(int numRows, int numCols, Vertices& vtx, Indices& ind)

{
	int i, j, k;
	int numVtx;
	int numTriangles;
	Vector4f pos;
	Vector4f colour = Vector4f(0, 0, 0, 1);
	float x;
	float z;
	float deltaX;
	float deltaZ;


	numVtx = (numRows + 1) * (numCols + 1);		// include the addtional vertices along the boundary of the surface
	//vtx.resize(numVtx);
	vtx.resize(0);
	cout << "   the vector's size is: " << vtx.size() << endl;
	cout << "   the vector's capacity is: " << vtx.capacity() << endl;
	cout << "   the vector's maximum size is: " << vtx.max_size() << endl;


	numTriangles = numRows * numCols * 2;
	//ind.resize(numTriangles * 3);
	ind.resize(0);

	// Fill the vertex buffer with positions
	k = 0;
	deltaX = (float)(MAX_X_RANGE - MIN_X_RANGE) / numCols; // increment along the x-axis
	deltaZ = (float)(MAX_Z_RANGE - MIN_Z_RANGE) / numRows; // increment along the y-axis


	for (i = 0, z = MIN_Z_RANGE; i <= numRows; i++, z += deltaZ) {
		for (j = 0, x = MIN_X_RANGE; j <= numCols; j++, x += deltaX) {
			pos.x = x;
			pos.y = 0;
			pos.z = z;
			pos.w = 1.0;

			// add colour 
			/*colour.x = j * deltaX / 2.0f;
			colour.z = i * deltaZ / 2.0f;*/
			colour = Vector4f(0, 0, 1, 1);

			vtx.push_back(Vertex(pos, colour));
			//		vtx[k] = Vertex(pos, colour);
			k++;


		}
	}
	// fill the index buffer

	for (i = 0; i < numRows; i++) {
		for (j = 0; j < numCols; j++) {
			createQuad(numRows, numCols, i, j, ind);
		}
	}
#if 0
	k = 0;
	for (i = 0; i < numRows; i++) {
		for (j = 0; j < numCols; j++) {
			// fill indices for the quad
			// change by making a quad function
			ind[k++] = i * (numCols + 1) + j;
			ind[k++] = i * (numCols + 1) + j + 1;
			ind[k++] = (i + 1) * (numCols + 1) + j + 1;

			ind[k++] = i * (numCols + 1) + j;
			ind[k++] = (i + 1) * (numCols + 1) + j + 1;
			ind[k++] = (i + 1) * (numCols + 1) + j;
		}
	}

#endif
	return(0);

}




/***************************************************************************************************************/
/*
// Function creates a Surface geometry

input:
numRows - number of rows in the surface
numCols - number of columns in the surface
u0, u1 - range of texture u coordinates
v0, v1 - range of texture v coordinates


*/

int Water::createSurface(int numRows, int numCols, float u0, float u1, float v0, float v1, Vertices& vtx, Indices& ind)

{
	int i, j;
	int numVtx;
	int numTriangles;
	Vector4f pos;
	Vector4f colour = Vector4f(0, 0, 0, 1);
	Vector3f normal;
	float x;
	float z;
	float deltaX;
	float deltaZ;
	float deltaU, deltaV;
	float u, v;		// texture coordinates
	Vector2f texCoord;


	numVtx = (numRows + 1) * (numCols + 1);		// include the addtional vertices along the boundary of the surface
	//vtx.resize(numVtx);
	vtx.resize(0);
	cout << "   the vector's size is: " << vtx.size() << endl;
	cout << "   the vector's capacity is: " << vtx.capacity() << endl;
	cout << "   the vector's maximum size is: " << vtx.max_size() << endl;


	numTriangles = numRows * numCols * 2;
	//ind.resize(numTriangles * 3);
	ind.resize(0);

	// Fill the vertex buffer with positions
	deltaX = (float)(MAX_X_RANGE - MIN_X_RANGE) / numCols; // increment along the x-axis
	deltaZ = (float)(MAX_Z_RANGE - MIN_Z_RANGE) / numRows; // increment along the y-axis

	// texture coordinates deltas
	deltaU = (u1 - u0) / numCols;
	deltaV = (v1 - v0) / numRows;

	normal = Vector3f(0, 1, 0);
	for (i = 0, z = MIN_Z_RANGE, v = v0; i <= numRows; i++, z += deltaZ, v += deltaV) {
		for (j = 0, x = MIN_X_RANGE, u = u0; j <= numCols; j++, x += deltaX, u += deltaU) {
			pos.x = x;
			pos.y = 0;
			pos.z = z;
			pos.w = 1.0;

			texCoord.x = u;
			texCoord.y = v;


			// add colour 
			colour.x = j * deltaX / 2.0f;
			colour.z = i * deltaZ / 2.0f;
			colour = Vector4f(0.5f, 0.3f, 0.2f, 1.0f);

			normal.y = 1;

			vtx.push_back(Vertex(pos, normal, colour, texCoord));


		}
	}

	// fill the index buffer
	for (i = 0; i < numRows; i++) {
		for (j = 0; j < numCols; j++) {
			createQuad(numRows, numCols, i, j, ind);
		}
	}

	return(0);

}

/*******************************************************************************/
// creates two triangoles indices of quad at position row, col

int Water::createQuad(int NumRows, int numCols, int row, int col, Indices& ind)
{
	// fill indices for the quad
	// change by making a quad function


	ind.push_back(row * (numCols + 1) + col);
	ind.push_back((row + 1) * (numCols + 1) + col + 1);
	ind.push_back(row * (numCols + 1) + col + 1);

	ind.push_back(row * (numCols + 1) + col);
	ind.push_back((row + 1) * (numCols + 1) + col);
	ind.push_back((row + 1) * (numCols + 1) + col + 1);


	return 0;
}





/*********************************************************************************/


int Water::render(Shader shader)
{
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
	rotMat = Matrix4f::rotateRollPitchYaw(worldRollAngle, worldPitchAngle, worldYawAngle, 0);
	// note that we always multiply the new matrix on the left
	modelMat = rotMat * modelMat;


	// set the translation - this is model space to world space transformation
	transMat = Matrix4f::translation(position.x, position.y, position.z);
	modelMat = transMat * modelMat;

	// move matrix to shader
	shader.copyMatrixToShader(modelMat, "model");

	
	// bind the texture to it using the texId
	//glBindTexture(GL_TEXTURE_2D, texId);
	// keep the texture unit internally
	/*if (shader.getProgId()==4)
	{*/
		materials.ambientMaterial = Vector3f(0.2, 0.2, 0.2);
		materials.diffuseMaterial = Vector3f(0.75, 0.75, 0.75);
		materials.specularMaterial = Vector3f(1, 1, 1);
		loadMaterials(shader);

	//}
	

	// bind the vao
	glBindVertexArray(vao);

	// draw using indices
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);

	// unbind the vao
	glBindVertexArray(0);

	return 0;
}

int Water::createSphere(int numLong, int numLat, Vertices& vtx, Indices& ind)

{
	int i, j, k;
	int numRows;
	int numCols;
	int numVtx;
	int numTriangles;
	Vector4f pos;
	Vector4f col;
	float alpha;
	float beta;
	float deltaAlpha;
	float deltaBeta;
	Vector2f texCoord;

	numRows = numLat * 2;  // number of horizonal slabs
	numCols = numLong;	// number of vertical slabs

	numVtx = (numRows + 1) * (numCols + 1);
	vtx.resize(0);
	cout << "   the vector's size is: " << vtx.size() << endl;
	cout << "   the vector's capacity is: " << vtx.capacity() << endl;
	cout << "   the vector's maximum size is: " << vtx.max_size() << endl;


	numTriangles = numRows * numCols * 2;
	ind.resize(0);

	// Fill the vertex buffer with positions
	k = 0;
	alpha = 0.0f;  // angle of latitude starting from the "south pole"
	deltaAlpha = (float)90.0 / numLat; // increment of alpha
	beta = 0;   // angle of the longtidute 
	deltaBeta = (float)360.0 / (numLong);	// increment of beta

	for (i = 0, alpha = -90; i <= numRows; i++, alpha += deltaAlpha) {
		for (j = 0, beta = 0; j <= numCols; j++, beta += deltaBeta) {
			pos.x = cos(DegreeToRadians(alpha)) * cos(DegreeToRadians(beta));
			pos.z = cos(DegreeToRadians(alpha)) * sin(DegreeToRadians(beta));
			pos.y = sin(DegreeToRadians(alpha));
			pos.w = 1.0;

			col = pos;
			col.x = fabs(col.x); col.y = fabs(col.y); col.z = fabs(col.z);
			col.x = 1; col.y = 0.0; col.z = 0;
			//vtx[k] = Vertex(pos, pos);

			// Calculate texture coordinates
			texCoord.x = (float)j / numCols; // Horizontal texture coordinate
			texCoord.y = (float)i / numRows; // Vertical texture coordinate

			vtx.push_back(Vertex(pos, Vector3f(pos.x, pos.y, pos.z), col, texCoord));
			//vtx.push_back(Vertex(pos, Vector3f(pos.x, pos.y, pos.z), col));

		}
	}




	// fill the index buffer

	k = 0;
	for (i = 0; i < numRows; i++) {
		for (j = 0; j < numCols; j++) {
			// fill indices for the quad
			// change by making a quad function
			ind.push_back(i * (numCols + 1) + j);
			ind.push_back(i * (numCols + 1) + j + 1);
			ind.push_back((i + 1) * (numCols + 1) + j + 1);

			ind.push_back(i * (numCols + 1) + j);
			ind.push_back((i + 1) * (numCols + 1) + j + 1);
			ind.push_back((i + 1) * (numCols + 1) + j);
		}
	}


	return(0);

}

int Water::init3dgeom(Vertices& vtx, Indices& ind) {
	vtx.resize(8);
	// initial each vertex with a positin and colour
	vtx[0] = Vertex(Vector4f(-1, -1, 1, 1), Vector3f(1, 0, 0), Vector4f(1, 0, 0, 1));
	vtx[1] = Vertex(Vector4f(1, -1, 1, 1), Vector3f(1, 0, 0), Vector4f(0, 1, 0, 1));
	vtx[2] = Vertex(Vector4f(1, 1, 1, 1), Vector3f(1, 0, 0), Vector4f(0, 0, 1, 1));
	vtx[3] = Vertex(Vector4f(-1, 1, 1, 1), Vector3f(1, 0, 0), Vector4f(1, 1, 0, 1));

	vtx[4] = Vertex(Vector4f(-1, -1, -1, 1), Vector3f(1, 0, 0), Vector4f(1, 0, 1, 1));
	vtx[5] = Vertex(Vector4f(1, -1, -1, 1), Vector3f(1, 0, 0), Vector4f(0, 1, 1, 1));
	vtx[6] = Vertex(Vector4f(1, 1, -1, 1), Vector3f(1, 0, 0), Vector4f(0.5, 0.5, 0, 1));
	vtx[7] = Vertex(Vector4f(-1, 1, -1, 1), Vector3f(1, 0, 0), Vector4f(0.5, 0, 0.5, 1));

	// set the colours


	ind.resize(36);
	int indices[] = {
		// front face
		0, 1, 2,
		2, 3, 0,
		// top face
		3, 2, 6,
		6, 7, 3,
		// back face
		7, 6, 5,
		5, 4, 7,
		// bottom face
		4, 5, 1,
		1, 0, 4,
		// left face
		4, 0, 3,
		3, 7, 4,
		// right face
		1, 5, 6,
		6, 2, 1
	};


	ind.assign(std::begin(indices), std::end(indices));
	return 0;
}

