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
	//vec3 normal;	// vertex normal
	//vec3 worldPos;  // vertex world coordinates to be used for light computation
	vec4 colour;	// vertex colour
	vec2 texCoord;
} frag;

float wavelength = 10.0;
float waveAmplitude = 1.0;

float offsetwater(float x, float z){
	float radiansX = (x/wavelength) * 2.0 * 3.14;
	float radiansZ = (z/wavelength) * 2.0 * 3.14;
	return waveAmplitude * 0.5 * (sin(radiansZ) + cos(radiansX));

}	

vec3 distort(vec3 v){
	float xCoord = offsetwater(v.x, v.z);
	float yCoord = offsetwater(v.x, v.z);
	float zCoord = offsetwater(v.x, v.z);
	return v + vec3(xCoord, yCoord, zCoord);
}


void main()

{


	float waveHeight = sin((vtxPos.x * 25.0 + 0.05*time)) * 5.0; // Adjust frequency and amplitude as needed

    // Apply displacement to vertex position
    vec3 displacedPosition = vec3(vtxPos.x,vtxPos.y,vtxPos.z) + vec3(0.0, waveHeight, 0.0);

	gl_Position = projection * view * model * vec4(displacedPosition, 1.0);	// vertex transformation
	
	frag.texCoord = vtxCoord;

	frag.colour = vtxCol;
}


