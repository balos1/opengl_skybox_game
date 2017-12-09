#pragma once

//IMPORTANT - If you change this structure in any way you need to change it in all the shaders as well!!!
struct Transformation
{
	glm::mat4 model_matrix;
	glm::mat4 view_matrix;
	glm::mat4 projection_matrix;
};
