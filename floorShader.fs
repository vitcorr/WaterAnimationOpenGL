#version 330 




struct material {
	vec3 ambientMaterial;		// ambient material properties
	vec3 diffuseMaterial;		// diffuse material properties
	vec3 specularMaterial;		// specular material properties
	vec3 internalRadiation;		// internal radiation of colour;
};

////////////////////////
 
// Point light properties
// light properties
struct lightProperties {
	vec3 lightIntensity;			// light intensity
	vec3 ambientIntensity;			// intesity of ambient colour
};


/////////////////////////


struct pointLight {		
	vec3 lightIntensity;		// light intensity
	vec3 ambientIntensity;		// intesity of ambient colour
	vec3 worldPos;				// position of light
	float specularPower;		// specular light power factor
} ;

///////////////////////

// directional light properties
struct directionalLight {
	vec3 lightIntensity;		// light intensity
	vec3 ambientIntensity;		// intesity of ambient colour
	vec3 lightDirection;		// direciton of light
	float specularPower;		// specular light power factor
};

///////////////////////////

// light attenuation  
// attenuation = 1/(a + b * d + c * d * d)
struct attenuation {
	float a; 
	float b; 
	float c;
};  
 
///////////////////
  
// spot light properties
struct spotLight {
	vec3 lightIntensity;		// light intensity
	vec3 ambientIntensity;		// intesity of ambient colour
	vec3 worldPos;				// location of spot light
	vec3 lightDirection;			// direction of spot light
	float spotDistLimit;		// distance limit of the spot light
	float cosSpotAlpha;			// cos of max opening of spot light (angle alpha)
	float specularPower;		// specular light power factor
}; 


uniform vec3 gEyeWorldPos;		// user location
uniform material gMaterial;		// object material
uniform pointLight gPointLight;	// point light attributes
uniform directionalLight gDirectionalLight;	// directional light attributes
uniform spotLight gSpotLight;		// spot light attributes
 
 uniform int includeAmbient;
 uniform int includeDiffuse;
 uniform int includeSpecular;
 uniform int includeObjColour;
 uniform int blinn;


uniform int gLightType;	// == 1 is a point light; == 2 spot light;  == 3 directional light;  


  

////////////////////////////////////////////////////////////////////
// Point light calculation
//structure of the fragment datga from the vertex shader
in fragData{
	vec3 normal;
	vec3 worldPos; 
	vec4 colour;
	vec2 texCoord;
} frag;

vec3 calcPointLight()  
{ 
	vec3 v1, v2;
	vec3 lightVec;
	vec3 colourAmbient = vec3(0,0,0);
	vec3 colourDiffuse = vec3(0,0,0);
	vec3 colourSpecular = vec3(0,0,0);
	vec3 fragEyeVec;
	vec3 reflectVec;
	float factor;
	vec3 colour;

	// ambient light
	colourAmbient = gPointLight.ambientIntensity * gMaterial.ambientMaterial; 
	//colourAmbient = gPointLight.ambientIntensity; 

	lightVec = normalize(frag.worldPos - gPointLight.worldPos);
	factor = max(dot(frag.normal, -lightVec),0); 
	factor = clamp(dot(frag.normal, -lightVec), 0, 1);

	colourDiffuse = gPointLight.lightIntensity * factor * gMaterial.diffuseMaterial; 
	//colourDiffuse = gPointLight.lightIntensity * factor ; 
	
	// get the eye vector 
	fragEyeVec = normalize(gEyeWorldPos - frag.worldPos);

	// get the reflection vector
	reflectVec = normalize(reflect(lightVec, frag.normal));
	factor = max(dot(reflectVec, fragEyeVec),0); 

	factor = pow(factor, gPointLight.specularPower);
	colourSpecular = gPointLight.lightIntensity * gMaterial.specularMaterial * factor; 
	//colourSpecular = gPointLight.lightIntensity*factor; 
	//colourSpecular = factor * vec3(1,0,0);
	
	colour = colourAmbient*includeAmbient + colourDiffuse*includeDiffuse + colourSpecular*includeSpecular; 
	//colour = colourAmbient;
	// if  (includeObjColour >0)  colour = colour * frag.colour.xyz;
	// if  (includeObjColour >0 && includeAmbient == 0 &&  includeDiffuse ==0 && includeSpecular == 0)  {
	// 	colour = frag.colour.xyz;
	// }
	//colour = colour * frag.colour.xyz;
	return(colour);
} 
uniform sampler2D texSampler1;



out vec4 color;


void main(void)
{
	//vec3 lightVec = normalize(frag.worldPos - gPointLight.worldPos);

	// final color
	//color = texture2D(texSampler1, frag.texCoord) * vec4(gPointLight.ambientIntensity,1.0);

	color = texture2D(texSampler1, frag.texCoord) * vec4(calcPointLight(),1.0);
	//color = vec4(0,1,1,0.5);

}
