
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

void Water:: initGeom(){
	
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

