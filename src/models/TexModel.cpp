/////////////////////////////////////////////////////////////////////////
////  Author: Cody Balos, Daniel Cliburn						     ////
////  Email:  cjbalos@gmail.com									     ////
////  Contributors: See Credits.txt					                 ////
////  This software may be modified and distributed under the terms  ////
////  of the MIT license. See the LICENSE file for details.		     ////
/////////////////////////////////////////////////////////////////////////

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include <iostream>
#include <string>
#include "TexModel.h"
#include "../utils/Texture.h"

TexModel::TexModel()
	: texID{0}, vertex_shininess{0}, in_texcoord{0}, texture_loaded{false}
{
}

TexModel::TexModel(std::string texFileName)
	: texID{0}, vertex_shininess{0}, in_texcoord{0}, texture_loaded{false}
{
	LoadTexture(texFileName);
}

TexModel& TexModel::BuildMipMaps()
{
	if (initialized)
	{
		glBindTexture(GL_TEXTURE_2D, texID);
		glEnable(GL_TEXTURE_2D);  //This Web page (http://www.opengl.org/wiki/Common_Mistakes) reports that some
		//ATI drivers require that texturing be enabled before calling glGenerateMipMap
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		cout << "TexModel objects must be initialized before calling BuildMipMaps." << endl;
	}

	return *this;
}

TexModel& TexModel::Define()
{
	//Texture Coordinates
	texcoords.push_back(glm::vec2(0.0f, 0.0998f)); // upper-left
	texcoords.push_back(glm::vec2(0.0f, 0.0f));    // lower-left
	texcoords.push_back(glm::vec2(1.0f, 0.998f));  // upper-right
	texcoords.push_back(glm::vec2(1.0f, 0.0f));    // lower-right

	//NOTE: The reason I am using .998 instead of 1 is that on some graphics cards I was getting a strange 
	//artifact that seemed to be caused by trying to go right up to the boundary of the texture. Using .998
	//instead of 1 fixed the problem.

	return *this;
}

bool TexModel::Init(std::shared_ptr<ShaderProgram>& shader)
{
	if (!texture_loaded)
		return false;

	if (!QuadMesh::Init(shader))
		return false;

	// check for additional (ones not required by QuadMesh) required shader attributes
	try
	{
		vertex_shininess = shader->attribute("vertexShininess");
		in_texcoord = shader->attribute("inTexCoord");
	}
	catch (std::runtime_error)
	{
		// cannot Init because shader attributes are missing
		return false;
	}

	Define();

	glGenBuffers(1, &VBOs.texcoords);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs.texcoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*texcoords.size(), &texcoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(in_texcoord, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(in_texcoord);
	
	DefineVerticeColor(color); //Call the method inherited from Quad to set up the vertices' color

	initialized = true;
	return true;  //Everything got initialized
}

TexModel& TexModel::LoadTexture(string texFileName)
{
	Texture t(texFileName, true); cout << endl;

	glGenTextures(1, &texID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, t.width(), t.height());
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, t.width(), t.height(), GL_RGB, GL_UNSIGNED_BYTE, t.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	texture_loaded = true;

	return *this;
}

void TexModel::Draw(glm::vec4 color)
{
	if (initialized == false)
	{
		std::cout << "ERROR: Cannot render a TexModel object before it has been initialized." << std::endl;
		return;
	}

	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID); //Bind the texture with the ID of texID to this VAO
	//NOTE that we need to make sure that the corect texture is bound for the appropriate object before every Draw.
	//Othwerise, the last texture loaded will be applied to every object in the scene 

	glVertexAttrib4f(vertex_color, color.r, color.g, color.b, color.a);  //color is attribute location 1
	glVertexAttrib3f(vertex_normal, 0.0, 0.0, 1.0);  //normal is attribute location 2
	glVertexAttrib1f(vertex_shininess, 2.0); //shininess is attribute location 3
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
}