/////////////////////////////////////////////////////////////////////////
////  Author: Cody Balos, Daniel Cliburn						     ////
////  Email:  cjbalos@gmail.com									     ////
////  Contributors: See Credits.txt					                 ////
////  This software may be modified and distributed under the terms  ////
////  of the MIT license. See the LICENSE file for details.		     ////
/////////////////////////////////////////////////////////////////////////

#pragma once

#include <glew.h>  //glew.h is supposed to be included before gl.h. To be safe, you can just include glew.h instead
#include "../meshes/QuadMesh.hpp"

class TexModel : public QuadMesh
{

public:
	TexModel();
	explicit TexModel(std::string texFileName);
	virtual ~TexModel() {}
	virtual TexModel& BuildMipMaps();
	virtual TexModel& Define();
	bool Init(std::shared_ptr<ShaderProgram>& shader) override;  //initializes the model and must take the file name of the texture
	TexModel& LoadTexture(std::string texFileName);
	void Draw(glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)) override;

protected:
	GLuint texID;

	// additional shader requirments
	GLuint vertex_shininess;
	GLuint in_texcoord;

	// properties
	std::vector<glm::vec2> texcoords;
	bool texture_loaded;
};
