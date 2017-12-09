/////////////////////////////////////////////////////////////////////////
////  Author: Cody Balos										     ////
////  Email:  cjbalos@gmail.com									     ////
////  Contributors: See Credits.txt					                 ////
////  This software may be modified and distributed under the terms  ////
////  of the MIT license. See the LICENSE file for details.		     ////
/////////////////////////////////////////////////////////////////////////

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include <iostream>
#include <string>
#include <algorithm>
#include <gtc/matrix_transform.inl>
#include "SkyBox.h"
#include "../utils/Texture.h"
#include "../utils/ObjLoader.hpp"
#include "../ViewModel.h"


SkyBox::SkyBox()
	: texID{0}, cube_texture{0}, texture_loaded{false}
{
}

void SkyBox::DefineVertices()
{
	if (vertices.size() != 0)
		return;

	ObjLoader obj_loader;
	obj_loader
		.LoadObj(RES_PATH + "/skybox.obj")
		.Map(&vertices, &texcoords, &normals);
}

bool SkyBox::Init(std::shared_ptr<ShaderProgram>& shader, std::string filepaths[6])
{
	if (initialized)
	{
		std::cout << "Attempted to reinitialize Skybox." << endl;
		return true;
	}

	this->shader = shader;

	// check for required shader attributes/uniforms
	try
	{
		vertex_position = shader->attribute("vertexPosition");
		cube_texture = shader->uniform("CubeTexture");
	}
	catch (std::runtime_error)
	{
		std::cout << "ERROR: Cannot Init the SkyBox object because required shader attributes were missing." << std::endl;
		return false;
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	DefineVertices();

	glGenBuffers(1, &VBOs.vertices);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs.vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(vertex_position);

	// Cube map texture stuff
	glGenTextures(1, &texID);
	glActiveTexture(GL_TEXTURE0);

	// load textures
	LoadTexture(RES_PATH + filepaths[0], GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
	LoadTexture(RES_PATH + filepaths[1], GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	LoadTexture(RES_PATH + filepaths[2], GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	LoadTexture(RES_PATH + filepaths[3], GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	LoadTexture(RES_PATH + filepaths[4], GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	LoadTexture(RES_PATH + filepaths[5], GL_TEXTURE_CUBE_MAP_POSITIVE_X);

	// format cube map texture
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	initialized = true;
	return true;  //Everything got initialized
}

bool SkyBox::Init(std::shared_ptr<ShaderProgram>& shader)
{
	std::string filepaths[] = {
		"/stars_bk.bmp", "/stars_fr.bmp",  "/stars_dn.bmp",
		"/stars_up.bmp", "/stars_rt.bmp", "/stars_lf.bmp"
	};

	return Init(shader, filepaths);
}

SkyBox& SkyBox::LoadTexture(string texFileName, GLuint side)
{
	Texture t(texFileName, true); cout << endl;

	
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
	glTexImage2D(side, 0, GL_RGB8, t.width(), t.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, t.bits());
	//glTexStorage2D(side, 1, GL_RGB8, t.width(), t.height());
	//glTexSubImage2D(side, 0, 0, 0, t.width(), t.height(), GL_RGB, GL_UNSIGNED_BYTE, t.bits());

	texture_loaded = true;

	return *this;
}

void SkyBox::Draw(glm::vec4 color)
{
	if (initialized == false)
	{
		std::cout << "ERROR: Cannot render a SkyBox object before it has been initialized." << std::endl;
		return;
	}

	glDepthMask(GL_FALSE);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID); //Bind the texture with the ID of texID to this VAO
										       //NOTE that we need to make sure that the corect texture is bound for the appropriate object before every Draw.
										       //Othwerise, the last texture loaded will be applied to every object in the scene 
	
	
	// remove translation from sky box so it appears huge
	// but make sure to put the translate matrix back to how it was
	glm::mat4 temp = ViewModel::transform.view_matrix;
	ViewModel::transform.view_matrix = glm::rotate(ViewModel::transform.view_matrix, radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ViewModel::transform.view_matrix = glm::mat4(glm::mat3(ViewModel::transform.view_matrix));
	ViewModel::UpdateTransform();

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	// always reset any modified transforms
	ViewModel::transform.view_matrix = temp;
	ViewModel::UpdateTransform();

	glDepthMask(GL_TRUE);
}