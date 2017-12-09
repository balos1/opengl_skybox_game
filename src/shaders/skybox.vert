#version 420

in vec3 vertexPosition;

//Uniform buffer object for the matrices
layout(binding = 35,std140) uniform Matrices
{
	mat4 model_matrix;
	mat4 view_matrix;
	mat4 projection_matrix;
};

out vec3 texcoords;

void main() {
  texcoords = vertexPosition;
  gl_Position = projection_matrix * view_matrix * vec4(vertexPosition, 1.0);
}