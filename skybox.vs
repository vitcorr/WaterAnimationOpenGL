#version 330

//
//Author: Doron Nussbaum (C) 2024 All Rights Reserved.
//-----------------------------------------------------
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


in vec4 vertex_position;		// the vertex position (in the local space) from VBO

out vec3 rVector;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;


void main()
{ 
    rVector = normalize((viewMat* modelMat * vertex_position).xyz);
    gl_Position = modelMat * vertex_position;

	gl_Position = projMat* viewMat * gl_Position;	// vertex transformation 
}
