/* ObjLoader.cpp
 * 
 * Copyright (C) 2016 Cody Balos
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#define TINYOBJLOADER_IMPLEMENTATION

#include <iostream>
#include <stdexcept>
#include <glm.hpp>
#include "ObjLoader.hpp"

ObjLoader::ObjLoader()
{
}

ObjLoader& ObjLoader::LoadObj(std::string filepath, bool verbose)
{
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filepath.c_str()))
		throw std::runtime_error(err);

	if (verbose)
	{
		std::cout << "Loaded .obj file " + filepath << std::endl;
		std::cout << "	vertex count = " + std::to_string(attrib.vertices.size()) << std::endl;
		std::cout << "	texcoord count = " + std::to_string(attrib.texcoords.size()) << std::endl;
		std::cout << "	normals count = " + std::to_string(attrib.normals.size()) << std::endl;
	}

	return *this;
}

ObjLoader& ObjLoader::Map(std::vector<glm::vec3>* vertices, std::vector<glm::vec2>* texcoords, std::vector<glm::vec3>* normals)
{
	// https://vulkan-tutorial.com/Loading_models
	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			if (attrib.vertices.size() > 0)
			{
				try
				{
					glm::vec3 vertex = {
						attrib.vertices.at(3 * index.vertex_index + 0),
						attrib.vertices.at(3 * index.vertex_index + 1),
						attrib.vertices.at(3 * index.vertex_index + 2)
					};
					vertices->push_back(vertex);
				}
				catch (const std::out_of_range& err)
				{
					// TODO: Decide what to do in this case
				}
			}
			
			if (attrib.normals.size() > 0 && normals != nullptr)
			{
				try
				{
					glm::vec3 normal = {
						attrib.normals.at(3 * index.normal_index + 0),
						attrib.normals.at(3 * index.normal_index + 1),
						attrib.normals.at(3 * index.normal_index + 2)
					};
					normals->push_back(normal);
				}
				catch (const std::out_of_range& err)
				{
					// TODO: Decide what to do in this case
				}
			}
			if (attrib.texcoords.size() > 0 && texcoords != nullptr)
			{
				try
				{
					glm::vec2 texCoord = {
						attrib.texcoords.at(2 * index.texcoord_index + 0),
						attrib.texcoords.at(2 * index.texcoord_index + 1),
					};
					texcoords->push_back(texCoord);
				}
				catch (const std::out_of_range& err)
				{
					// TODO: Decide what to do in this case
				}
			}
		}
	}

	return *this;
}