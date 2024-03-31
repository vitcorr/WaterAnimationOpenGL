#version 330 

uniform sampler2D texSampler1;

//structure of the fragment datga from the vertex shader
in fragData{
	//vec3 normal;
	//vec3 worldPos; 
	vec4 colour;
	vec2 texCoord;
} frag;

out vec4 color;


void main(void)
{

	// final color
	color = texture2D(texSampler1, frag.texCoord);
	//color = frag.colour;

}
