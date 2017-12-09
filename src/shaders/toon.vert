//This vertex shader is based on Examples 7.8 and 7.20 in the OpenGL Programming Guide (8th edition)
//It really does not do anything specific to toon shading.

#version 420 core

layout(location = 0) in vec4  vertexPosition;
layout(location = 1) in vec4  vertexColor;
layout(location = 2) in vec3  vertexNormal;

out vec4 position; //position of the vertex in "eye" coordinates
out vec4 color;
out vec3 normal; //orientation of the normal in "eye" coordinates

layout(binding = 35,std140) uniform Matrices
{
	mat4 model_matrix;
	mat4 view_matrix;
	mat4 projection_matrix;
};

void main()
{
	position = view_matrix * model_matrix * vertexPosition;
	color = vertexColor;
	vec4 n = view_matrix * model_matrix * vec4(vertexNormal, 0.0); //Assumes only isometric scaling
	normal = normalize(vec3(n.x, n.y, n.z));

	gl_Position = projection_matrix * position;
}