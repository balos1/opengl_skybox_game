/////////////////////////////////////////////////////////////////////////
////  Author: Cody Balos											 ////
////  Email:  cjbalos@gmail.com									     ////
////  Contributors: See Credits.txt					                 ////
////  This software may be modified and distributed under the terms  ////
////  of the MIT license. See the LICENSE file for details.		     ////
/////////////////////////////////////////////////////////////////////////

#pragma once

#include <glm.hpp>
#include "CubeMesh.hpp"

class SphereMesh : public CubeMesh
{
public:
	SphereMesh();
	void DefineVertices() override;
	void CopyVertices(SphereMesh &model);
	void DefineColors() override;
};
