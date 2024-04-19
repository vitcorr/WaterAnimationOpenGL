#version 330 core

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection;
uniform int time;// Time variable for animation


in vec4 vtxPos;
in vec4 vtxCol;
//in vec3 vtxNormal;
in vec2 vtxCoord;


// strcutre to be passed to the fragment shader
out fragData{
	vec3 normal;	// vertex normal
	vec3 worldPos;  // vertex world coordinates to be used for light computation
	vec4 colour;	// vertex colour
	vec2 texCoord;
} frag;

float wavelength = 10.0;
float waveAmplitude = 1.0;




void main()

{
	mat4 m;		// matrix to be used for the normal transformation
	m = transpose(inverse(model));
	vec3 n = normalize(vec3(0.0,1.0,0.0));

	gl_Position = model * vtxPos;
 	frag.worldPos = gl_Position.xyz;
	frag.normal = normalize((m * vec4(n,0)).xyz);
	
	float waveHeighty = sin((vtxPos.x * 25.0 + 0.05*time)) * 8.0; // Adjust frequency and amplitude as needed
	float waveHeight = (sin(vtxPos.x * 15.0 + time*0.05) + cos(vtxPos.z * 15.0 + 0.05*time)) * 0.05;
    // Apply displacement to vertex position
    vec3 displacedPosition = vec3(vtxPos.x,vtxPos.y,vtxPos.z) + vec3(waveHeight, -5, waveHeight);

	gl_Position = projection * view * model * vec4(displacedPosition, 1.0);	// vertex transformation
	
	frag.texCoord = vtxCoord;

	frag.colour = vtxCol;
}


