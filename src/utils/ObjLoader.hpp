/* ObjLoader.hpp
 *
 * Copyright (C) 2016 Cody Balos
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#pragma once

#include <tiny_obj_loader.h>
#include <unordered_map>
#include <glm.hpp>

class ObjLoader
{
public: 
	ObjLoader();
	ObjLoader& LoadObj(std::string filepath, bool verbose = true);
	ObjLoader& Map(std::vector<glm::vec3> *vertices, std::vector<glm::vec2>* texcoords = nullptr, std::vector<glm::vec3>* normals = nullptr);

private:
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
};
