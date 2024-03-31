#pragma once

//=============================================================================
/*
Surface.h

Author: Doron Nussbaum (C) 2015 All Rights Reserved.
-----------------------------------------------------

 Purpose:
--------------
 a. creates a Surface object.


 Description:
--------------
  Creates Surface geometry.  Currently it is used only to create a geometry of a Surface. It can be converted to a stand alone function.
  It creates a surface around the (0,0,0) along the x,z plane.  Range is (-1,0,-1) to (1,0,1).

  The user provices number of rows and number of columns, which results in number of rows X number of columns quads.


 License
--------------

 Code can be used for instructional and educational purposes.
 Usage of code for other purposes is not allowed without a given permission by the author.


 Disclaimer
--------------

 The code is provided as is without any warranty

 */
 //=============================================================================



#ifndef SURFACE_HEADER
#define SURFACE_HEADER 1


#include "graphicsObject.h"


class Surface : public GraphicsObject
{
public:
	Surface();
	~Surface();
	static int createSurface(int numRows, int numCols, Vertices& vtx, Indices& ind);
	static int createSurface(int numRows, int numCols, float u0, float u1, float v0, float v1, Vertices& vtx, Indices& ind);
	int render(Shader shader);

private:
	static int createQuad(int NumRows, int numCols, int row, int col, Indices& ind);

};


#endif

