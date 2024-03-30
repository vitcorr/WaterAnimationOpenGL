#version 330 core

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection;

in vec4 vtxPos;
in vec4 vtxCol;
//in vec3 vtxNormal;

// strcutre to be passed to the fragment shader
out fragData{
	//vec3 normal;	// vertex normal
	//vec3 worldPos;  // vertex world coordinates to be used for light computation
	vec4 colour;	// vertex colour
} frag;


	

void main()

{
	// transform the vertex position
	gl_Position = model * vtxPos;


	gl_Position = projection * view * gl_Position;	// vertex transformation



	
	frag.colour = vtxCol;
}


