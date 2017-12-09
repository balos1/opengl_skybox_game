//FileName:		ViewModel.h
//Programmer:	Dan Cliburn, Cody Balos
//Date:			4/20/2015
//Purpose:		This file defines the header for the WorldModel class.
//This ViewModel class differs from ones in previous labs in that it is responsible for loading the shaders and 
//rendering all of the objects in their proper places. Basically, OpenGL and GLEW stuff goes in this class.
//SDL goes in the Viewcontroller. The Viewcontroller then passes the view_matrix to the Draw() method of this class.

#pragma once

#include <glew.h> //glew.h is supposed to be included before gl.h. To be safe, you can just include glew.h instead
#include <memory>
#include "utils/ShaderProgram.hpp"
#include "Renderer.h"
#include "meshes/CubeMesh.hpp"
#include "models/StageModel.hpp"
#include "models/SkyBox.h"
#include "game_objects/EnergizerModel.hpp"
#include "Tiles.h"
#include <detail/type_mat.hpp>
#include "game_utils/ScoreManager.hpp"

using namespace glm;

class ViewModel
{
public:
	ViewModel();

	void SetUpLights() const;
	static void SetViewMatrix(const mat4 &view_matrixIN) { transform.view_matrix = view_matrixIN; }
	bool OnTick();
	void DoMovement(vec3 displacement); //TODO: offload to camera/glman class
	static bool InitGLEW();
	bool SetupShaders();
	bool Init(game::Levels level);
	void Draw();
	static void UpdateTransform();

	std::unique_ptr<ScoreManager> score_manager;

	// static properties used by many things in program ... eeek
	static Transformation transform;
	static GLuint transform_id;
	static GLuint transform_binding;
	static GLuint lights_binding;

	

private:
	// state
	bool initialized;
	game::Levels level;

	// Shader programs used
	std::shared_ptr<ShaderProgram> default_shader;
	std::shared_ptr<ShaderProgram> energizer_shader;
	std::shared_ptr<ShaderProgram> skybox_shader;

	// Game models that belong to the view
	Renderer<SkyBox> skybox;
	Renderer<StageModel> stage;
	std::unique_ptr<Tiles> tiles;

	// TODO: Offload the camera/glman to its own class
	glm::vec3 displacement;
	// The axis-aligned bounding box for the camera/glman/player
	AABB glman_aabb;
	
	// debug stuff
	Renderer<SphereMesh> origin;
	Renderer<BoundingBox> glman_bounding_box_renderable;
};