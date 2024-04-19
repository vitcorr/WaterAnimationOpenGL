#version 330 core

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection;
uniform int time;// Time variable for animation


in vec4 vtxPos;
in vec4 vtxCol;
in vec3 vtxNormal;
in vec2 vtxCoord;


// strcutre to be passed to the fragment shader
out fragData{
	vec3 normal;	// vertex normal
	vec3 worldPos;  // vertex world coordinates to be used for light computation
	vec4 colour;	// vertex colour
	vec2 texCoord;
} frag;



void main()

{
	frag.normal = vec3(1,1,1);
	gl_Position = model * vtxPos;
	
	frag.texCoord = vtxCoord;

	gl_Position = projection * view * gl_Position;	// vertex transformation
	//frag.colour = vtxCol;
}


