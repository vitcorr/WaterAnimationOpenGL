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
	mat4 m;		// matrix to be used for the normal transformation
	m = transpose(inverse(model));
	vec3 n = normalize(vec3(0.0,1.0,0.0));

	gl_Position = model * vtxPos;
	//frag.normal = vec3(0.0,1.0,0.0);
	frag.normal = normalize((m * vec4(n,0)).xyz);

	frag.texCoord = vtxCoord;
	frag.worldPos = gl_Position.xyz;

	gl_Position = projection * view * gl_Position;	// vertex transformation
	frag.colour = vec4(0.0,1.0,0.0,1.0);
}


