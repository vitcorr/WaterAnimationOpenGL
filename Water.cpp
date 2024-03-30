
//=============================================================================
// sphere.c
//
//Author: Doron Nussbaum (C) 2015 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. creates a sphere object. 
//
//
// Description:
//--------------
//  Creates sphere geometry.  Currently it is used only to create a geometry of a sphere. It can be converted to a stand alone function. 
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


#include "Water.h"
#include <iostream>
using namespace std;



Water::Water()
{
}


Water::~Water()
{
}






/*
// Function creates a sphere geometry

input:
numLong - number of longitudes
numLat - number of latituudes.  Note that the number of latitudes is defined only for the northern hemishphere


*/

void Water:: initGeom(Vertices& vtx, Indices& ind){
	
	m_vertices.resize(4);
	// initial each vertex with a positin and colour
	m_vertices[0] = Vertex(Vector4f(-1, -1, 0, 1), Vector4f(0, 0, 1, 1));
	m_vertices[1] = Vertex(Vector4f(1, -1, 0, 1), Vector4f(0, 1, 1, 1));
	m_vertices[2] = Vertex(Vector4f(1, 1, 0, 1), Vector4f(0, 0, 1, 1));
	m_vertices[3] = Vertex(Vector4f(-1, 1, 0, 1), Vector4f(0, 0, 0, 1));


	// set the colours


	m_indices.resize(6);
	int indices[] = {
		// front face
		0, 1, 2,
		2, 3, 0
	};


	m_indices.assign(std::begin(indices), std::end(indices));

	//transfer
	ind.assign(std::begin(indices), std::end(indices));
	vtx.assign(std::begin(m_vertices), std::end(m_vertices));

	for (int i = 0; i < vtx.size(); i++) {
		printf("x=%f, " "y=%f " "z = %f \n", vtx[i].pos.x, vtx[i].pos.y, vtx[i].pos.z);
		printf("%d \n", ind[i]);

	}

}





/*********************************************************************************/


int Water::render(Matrix4f parentWorldMat)
{
    int i, j;

    Matrix4f worldMat;	// world matrix
    Vector4f p;


    worldMat = parentWorldMat * this->worldMat * modelMat;

    glColor3f(1, 0, 0);

    glBegin(GL_TRIANGLES);
    for (i = 0; i < m_indices.size(); ++i) {
        j = m_indices[i];
        glColor3f(m_vertices[j].col.r, m_vertices[j].col.g, m_vertices[j].col.b);
        p = worldMat * m_vertices[j].pos;
        //p = roof[i];
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
    //printf("%s", "ran");

    // render any children
    for (int i = 0; i < m_children.size(); i++) {
        m_children[i]->render(parentWorldMat * this->worldMat);
    }



    return(0);
}

int Water::render(Shader shader)
{
	Matrix4f rotMat;  // rotation matrix;
	Matrix4f scaleMat; // scaling matrix;
	Matrix4f transMat;	// translation matrix
	Matrix4f modelMat;	// final model matrix

	int shaderProgId = shader.getProgId();

	// set the transformation matrix - the model transfomration
	modelMat = Matrix4f::identity();

	// set the scaling - this is model space to model space transformation
	scaleMat = Matrix4f::scale(scale.x, scale.y, scale.z);
	modelMat = scaleMat * modelMat;

	// set the rotation  - this is model space to model space transformation 
	rotMat = Matrix4f::rotateRollPitchYaw(worldRollAngle, worldPitchAngle, worldYawAngle, 0);

	// note that we always multiply the new matrix on the left
	modelMat = rotMat * modelMat;

	// set the translation - this is model space to world space transformation
	transMat = Matrix4f::translation(worldPosition.x, worldPosition.y, worldPosition.z);
	modelMat = transMat * modelMat;

	// move matrix to shader
	shader.copyMatrixToShader(modelMat, "model");

	////load materials
	//materials.ambientMaterial = Vector3f(0.2, 0.2, 0.2);
	//materials.diffuseMaterial = Vector3f(0.75, 0.75, 0.75);
	//materials.specularMaterial = Vector3f(0.8, 0.8, 0.8);
	//materials.ambientMaterial = Vector3f(0.2, 0.2, 0.2);

	// load the object materials
	//loadMaterials(shader);

	// bind the vao
	glBindVertexArray(vao);

	// draw elements using indices

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);

	// unbind the vao
	glBindVertexArray(0);

	return 0;
}