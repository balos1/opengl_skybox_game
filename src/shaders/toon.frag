//The toon shader part of this code is adapted from: www.lighthouse3d.com/tutorials/glsl-tutorial/toon-shading-version-iii/
//The rest of the code is based on Example 7.8 in the OpenGL Programming Guide (8th edition)
//ACKNOWLEDGEMENT: I got some help from: http://www.gamedev.net/topic/658486-glsl-send-array-receive-struct/ on how 
//to pass the array of structs with the light info to the shader.

#version 420 core

struct LightProperties
{
	vec4 color;			//color of light
	vec4 position;		//location of light if w = 1,
						//otherwise the direction towards the light
	
	//spotlight attributes (can only use if light is local, i.e. position.w = 1)
	vec4 spotLightValues;	//if spotLightValues.x > 0 then it is a spotLight
							//spotLightValues.y is spotCosCuttoff
							//spotLightValues.z is spotExponent
	vec4 spotConeDirection;  //direction that the spotlight is shinining

	//local light attenuation coefficients (position.w must be 1 to use these)
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;

	uint isEnabled;
};

layout(binding = 0,std140) uniform Lights
{
	LightProperties lights[4];
	vec3 globalAmbientLight;
	int totalLights; //Number of lights in this application
};

layout(binding = 35,std140) uniform Matrices
{
	mat4 model_matrix;
	mat4 view_matrix;
	mat4 projection_matrix;
};

in vec4 position; //position of the fragment in "eye" coordinates
in vec4 color;
in vec3 normal; //orientation of the normal in "eye" coordinates
in float shininess; //not used

out vec4 FragColor;

void main()
{
	vec3 colorSum = vec3(0.0, 0.0, 0.0);
	vec3 n = normalize(normal);
	vec3 L; //this will be the vector towards the light

	//Loop over all of the lights
	for (int lightNum = 0; lightNum < totalLights; lightNum++)
	{
		if (lights[lightNum].isEnabled == 1)  //Make sure the light is on
		{
			vec3 I = lights[lightNum].color.rgb;  //Intensity (color) of the light
			vec4 lightPos_eyeCoords = view_matrix*lights[lightNum].position; //put light position in "eye" coordinates
			if (lights[lightNum].position.w > 0.1)  //a positional light
			{
				vec4 LTemp = lightPos_eyeCoords - position;  //Calculate the L vector
				L = normalize(vec3(LTemp.x, LTemp.y, LTemp.z)); //remove the w value from the L vector and normalize
			}
			else //directional light
			{
				L = normalize(vec3(lightPos_eyeCoords.x, lightPos_eyeCoords.y, lightPos_eyeCoords.z)); //remove w value to form the L vector and normalize
				//NOTE: You could just disable effects from directional lights with:
				//L = vec3(0.0, 0.0, 0.0);
			}

			float intensity = dot(L,n);

			if (intensity > 0.95)
				colorSum += color.rgb * I * 1.0;
			else if (intensity > .75)
				colorSum += color.rgb * I * 0.75;
			else if (intensity > .5)
				colorSum += color.rgb * I * 0.5;
			else if (intensity > 0.25)
				colorSum += color.rgb * I * 0.25;
			else
				colorSum += color.rgb * I * 0.05;
		}
	}
	colorSum = colorSum + globalAmbientLight*color.rgb;  //compute the final color of the fragment
	FragColor = vec4(colorSum.r, colorSum.g, colorSum.b, color.a);  //use the fragment's original alpha
}