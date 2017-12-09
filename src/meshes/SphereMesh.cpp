/////////////////////////////////////////////////////////////////////////
////  Author: Cody Balos											 ////
////  Email:  cjbalos@gmail.com									     ////
////  Contributors: See Credits.txt					                 ////
////  This software may be modified and distributed under the terms  ////
////  of the MIT license. See the LICENSE file for details.		     ////
/////////////////////////////////////////////////////////////////////////

#include "../config.inl"
#include "SphereMesh.hpp"
#include "../utils/ObjLoader.hpp"

SphereMesh::SphereMesh()
{
}

void SphereMesh::DefineVertices()
{
	if (vertices.size() != 0)
		return;

	ObjLoader obj_loader;
	obj_loader
		.LoadObj(RES_PATH + "/hexsphere.obj")
		.Map(&vertices, &texcoords, &normals);
}

void SphereMesh::CopyVertices(SphereMesh& model)
{
	vertices.insert(vertices.begin(), model.vertices.begin(), model.vertices.end());
	normals.insert(normals.begin(), model.normals.begin(), model.normals.end());
}

void SphereMesh::DefineColors()
{
}