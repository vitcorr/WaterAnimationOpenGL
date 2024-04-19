#version 330

//
//Author: Doron Nussbaum (C) 2014 All Rights Reserved.
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

in vec3 rVector;

uniform samplerCube texCube;

out vec4 colour;

void main() 
{ 
    gl_FragColor = texture(texCube, rVector);
 } 
