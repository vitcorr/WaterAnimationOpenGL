#version 330 

//structure of the fragment datga from the vertex shader
in fragData{
	//vec3 normal;
	//vec3 worldPos; 
	vec4 colour;
} frag;




void main(void)
{

	// final color
	gl_FragColor = frag.colour;
}
