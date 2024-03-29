#pragma once
#ifndef WATER_HEADER
#define WATER_HEADER

//=============================================================================
// floor.h
//
//Author: Doron Nussbaum (C) 2023 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a.  class for rendering a subobject in a hierarchical object.  In this case it is a floor in a building 
//
//
// Description:
//--------------
//  This class defines a floor in a two story building. 
//
//	The base geometry is a cube
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

#include "graphicsObject.h"

#include "graphicsObject.h"

class Water : public GraphicsObject
{
public:
    Water();
    ~Water();

    void initGeom();
    int render(Matrix4f parentWorldMat);



private:
    Vertices    m_vertices;
    Indices		m_indices;
};

#endif