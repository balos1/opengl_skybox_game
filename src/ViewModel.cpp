//FileName:		ViewModel.cpp
//Programmer:	Dan Cliburn, Cody Balos
//Date:			11/15/2015
//Purpose:		Define the methods for the World ViewModel class.
//The Init() method needs to set up OpenGL and GLEW and prepare all objects (and their shaders) to be rendered.
//The Draw() method positions and renders all objects in the scene and activates the appropriate shader(s).

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include <iostream>
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <detail/type_mat.hpp>
#include "ViewModel.h"
#include "config.inl"
#include "LightProperties.h"
#include "Transformation.h"

using namespace glm;
using namespace std;

Transformation ViewModel::transform = { mat4(1.0f), mat4(1.0f), mat4(1.0f) };
GLuint ViewModel::transform_id = 0;
GLuint ViewModel::transform_binding = 35;
GLuint ViewModel::lights_binding = 0;

ViewModel::ViewModel()
	: initialized{false}, level{game::GALAXY}, score_manager{std::make_unique<ScoreManager>()}
{
}

bool ViewModel::InitGLEW()
{
	//Next initialize GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cout << "Error initializing GLEW: " << glewGetErrorString(err) << endl;
		return false;
	}

	//The following code was adapted from the OpenGL 4.0 Shading Language Cookbook, by David Wolff
	//to provide information about the hardware and supported versions of OpenGL and GLSL. 
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	cout << "GL Vendor: " << vendor << endl;
	cout << "GL Renderer: " << renderer << endl;
	cout << "GL Version: " << version << endl;
	cout << "GLSL Version: " << glslVersion << endl << endl;

	return true;
}

// This method sets up all the shaders used in the game. This includes loading, compiling,
// linking, activating, getting uniform names, etc. 
bool ViewModel::SetupShaders()
{
	// Rules to follow:
	//		Uniforms  -> Pascal Case (ThisIsPascal)
	//		Textures  -> Pascal Case
	//		Attributes -> Camel Case (thisIsCamelCase)
	energizer_shader = make_shared<ShaderProgram>();
	energizer_shader->initFromFiles(SHADER_PATH + "/energizer.vert", SHADER_PATH + "/energizer.frag");
	energizer_shader->addAttribute("vertexPosition");
	energizer_shader->addAttribute("vertexColor");
	energizer_shader->addAttribute("vertexNormal");
	energizer_shader->addAttribute("vertexShininess");

	skybox_shader = make_shared<ShaderProgram>();
	skybox_shader->initFromFiles(SHADER_PATH + "/skybox.vert", SHADER_PATH + "/skybox.frag");
	skybox_shader->addAttribute("vertexPosition");
	skybox_shader->addUniform("CubeTexture");

	default_shader = make_shared<ShaderProgram>();
	default_shader->initFromFiles(SHADER_PATH + "/phong.vert", SHADER_PATH + "/phong.frag");
	default_shader->addAttribute("vertexPosition");
	default_shader->addAttribute("vertexColor");
	default_shader->addAttribute("vertexNormal");
	default_shader->addAttribute("vertexShininess");

	return true;
}

/**
* \brief This method sets up global lighting for the world.
*/
void ViewModel::SetUpLights() const
{
	//IMPORTANT - If you change this structure in any way you need to change it in all fragment shader(s) as well!!!
	struct Lights
	{
		LightProperties lights[4];
		vec3 globalAmbientLight;
		int totalLights;
	} lightInfo;

	//Now, set up the lights for the scene
	lightInfo.totalLights = 4;
	lightInfo.globalAmbientLight = vec3(0.3, 0.3, 0.3);

	lightInfo.lights[0].color = vec4(1.0, 0.0, 0.0, 1.0);
	lightInfo.lights[0].position = vec4(-4.0, 0.0, -4.0, 1.0);
	lightInfo.lights[0].spotLightValues = vec4(0.0, 0.0, 0.0, 0.0);
	lightInfo.lights[0].constantAttenuation = 2.0;
	lightInfo.lights[0].linearAttenuation = 0.0;
	lightInfo.lights[0].quadraticAttenuation = 0.0;
	lightInfo.lights[0].isEnabled = 1;

	lightInfo.lights[1].color = vec4(0.0, 1.0, 0.0, 1.0);
	lightInfo.lights[1].position = vec4(0.0, 3.0, 0.0, 1.0);  //positional light since w = 1
	lightInfo.lights[1].spotLightValues = vec4(0.0, 0.0, 0.0, 0.0);
	lightInfo.lights[1].constantAttenuation = 2.0;
	lightInfo.lights[1].linearAttenuation = 0.0;
	lightInfo.lights[1].quadraticAttenuation = 0.0;
	lightInfo.lights[1].isEnabled = 1;

	lightInfo.lights[2].color = vec4(0.0, 0.0, 1.0, 1.0);
	lightInfo.lights[2].position = vec4(5.0, 2.5, 0.0, 1.0);  //positional light since w = 1
	lightInfo.lights[2].spotLightValues = vec4(0.0, 0.0, 0.0, 0.0);
	lightInfo.lights[2].constantAttenuation = 2.0;
	lightInfo.lights[2].linearAttenuation = 0.0;
	lightInfo.lights[2].quadraticAttenuation = 0.0;
	lightInfo.lights[2].isEnabled = 1;

	lightInfo.lights[3].color = vec4(1.0, 1.0, 1.0, 1.0);
	lightInfo.lights[3].position = vec4(3.5, 1.75, -3.5, 1.0);  //positional light since w = 1
	lightInfo.lights[3].spotLightValues = vec4(1.0, 0.95, 4.0, 0.0);
	//If the first parameter to spotLightValues is > 0, then this is a spotlight
	//The second parameter to spotLightValues is the Spot Cosine Cutoff
	//The third parameter to spotLightValues is the Spotlight Exponent
	//The fourth parameter to spotLightValues is unused
	lightInfo.lights[3].spotConeDirection = vec4(0.25, -1.0, -0.25, 0.0);
	lightInfo.lights[3].constantAttenuation = 0.5;
	lightInfo.lights[3].linearAttenuation = 0.0;
	lightInfo.lights[3].quadraticAttenuation = 0.0;
	lightInfo.lights[3].isEnabled = 1;

	//Pass the light info to the shaders in a Uniform Buffer Object.
	//This allows ALL shaders to be able to access the light information.
	GLuint lightsLoc;
	glGenBuffers(1, &lightsLoc);
	glBindBuffer(GL_UNIFORM_BUFFER, lightsLoc);
	glBufferData(GL_UNIFORM_BUFFER, sizeof lightInfo, &lightInfo, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, lights_binding, lightsLoc); //The 0 needs to match the number used in the shaders for the lights
}

/**
* \brief Initialize the game objects.
* \return Returns true if intiailization of all objects was successful.
*/
bool ViewModel::Init(game::Levels level)
{
	this->level = level;

	if (InitGLEW() == false)
		throw std::runtime_error("Could not initialize GLEW");

	// Initialize OpenGL global settings
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	if (SetupShaders() == false)
		throw std::runtime_error("Could not setup shaders.");

	////////////////////////////////////////////////////////
	// Setup the bounding box for the player/glman/camera //
	////////////////////////////////////////////////////////
	std::vector<glm::vec3> glman_vertices = {
		{PLAYER_BOX_SIZE, 0.0f, 0.0f}, {PLAYER_BOX_SIZE, 0.0f, 0.0f}, // 0, 1
		{0.0f, PLAYER_BOX_SIZE, 0.0f}, {PLAYER_BOX_SIZE, PLAYER_BOX_SIZE, 0.0f}, // 2, 3
		{0.0f, 0.0f, PLAYER_BOX_SIZE}, {PLAYER_BOX_SIZE, 0.0f, PLAYER_BOX_SIZE}, // 4, 5
		{0.0f, PLAYER_BOX_SIZE, PLAYER_BOX_SIZE}, {PLAYER_BOX_SIZE, PLAYER_BOX_SIZE, PLAYER_BOX_SIZE} // 6, 7
	};

	glman_aabb.Init(glman_vertices);
	
	// setup visible bounding box for glman if in debug mode
	if (__DEBUG__)
	{
		glman_bounding_box_renderable.getObject()->DefineVertices(&glman_aabb);
		glman_bounding_box_renderable.Init(default_shader);
		origin.Init(default_shader);
	}
	//////////////////////////////////////////////////////
	/////			End setup for glman				 /////
	//////////////////////////////////////////////////////


	////////////////////////////////////////////////////////
	////		    Init tiles & energizers				////
	////////////////////////////////////////////////////////
	// first loads the .obj so we dont have to for any other
	EnergizerModel first = EnergizerModel(glm::vec3(0.0f), energizer_shader);
	tiles = std::make_unique<Tiles>(Tiles(ONE_UNIT));
	for (auto i = 0; i < tiles->tiles.size(); ++i)
	{
		for (auto j = 0; j < tiles->tiles[i].size(); ++j)
		{
			Tile *tile = tiles->tiles[i][j].get();
			if (tile != nullptr)
				tile->energizer = std::make_shared<EnergizerModel>(EnergizerModel(first, tile->center, energizer_shader));
		}
	}
	////////////////////////////////////////////////////////
	////		   End Init tiles/energizers		    ////
	////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////
	////		     Init environment				    ////
	////////////////////////////////////////////////////////
	//if (stage.Init(default_shader) == false)
	//	cout << "ERROR: Count not Init stage" << endl;

	
	if (level == game::DAYTIME)
	{
		std::string filenames[6] = {
			"/Daylight_Box_Back.bmp", "/Daylight_Box_Front.bmp", "/Daylight_Box_Top.bmp",
			"/Daylight_Box_Bottom.bmp", "/Daylight_Box_Left.bmp", "/Daylight_Box_Right.bmp"
		};
		skybox.getObject()->Init(skybox_shader, filenames);
	}
	else if (level == game::CITY)
	{
		std::string filenames[6] = {
			"/city_negz.bmp", "/city_posz.bmp", "/city_posy.bmp",
			"/city_negy.bmp", "/city_negx.bmp", "/city_posx.bmp"
		};
		skybox.getObject()->Init(skybox_shader, filenames);
	}
	
	if (skybox.Init(skybox_shader) == false)
		cout << "ERROR: Count not Init skybox" << endl;


	// Set up the uniform buffer objects that hold data that all of the shaders share. In this
	// application we have two uniform buffer objects: one for the lights and one for the matrices.
	// The lights don't change as the program runs so we can set them here as well.
	SetUpLights();  
	glGenBuffers(1, &transform_id);
	glBindBuffer(GL_UNIFORM_BUFFER, transform_id);

	////////////////////////////////////////////////////////
	////		    End Init environment				////
	////////////////////////////////////////////////////////

	//Since the projection matrix will not change during the program we can calculate it here
	//transform.projection_matrix = frustum(-0.2f, 0.2f, -0.1f, 0.1f, 0.1f, 100.0f);
	transform.projection_matrix = perspectiveFov(radians(90.0f), static_cast<float>(WINDOWWIDTH), static_cast<float>(WINDOWHEIGHT), 0.1f, 500.0f);

	initialized = true;
	return true;
}

/**
* \brief Updates the game world and things in the game world which should change independents
*		  of input from the human playing the game.
*		  
* \returns True if game should continue.
*/
bool ViewModel::OnTick()
{
	// redghost.OnTick();
	// purpleghost.OnTick();
	// blueghost.OnTick();
	// orangeghost.OnTick();

	// collision check with the energizers
	// TODO: this nullcount thing is so hacky lol
	auto nullcount = 0, loopcount= 0;
	for (auto i = 0; i < tiles->tiles.size(); ++i)
	{
		for (auto j = 0; j < tiles->tiles[i].size(); ++j)
		{
			loopcount++;
			
			auto tile = tiles->tiles[i][j].get();

			if (tile == nullptr || tile->energizer == nullptr)
			{
				nullcount++;
				continue;
			}

			tile->energizer->OnTick();
			
			if (glman_aabb.AABBtoAABB(*tile->energizer->GetAABB().get()))
			{
				tile->energizer->OnCollide();
				tile->energizer = nullptr;
				score_manager->IncrementScore();
			}
		}
	}
	if (nullcount == loopcount)
		return false; // all energizers collected, so quit

	// collision.Check(glman, redghost);
	// collision.Check(glman, purpleghost);
	// collision.Check(glman, blueghost);
	// collision.Check(glman, orangeghost);

	return true;
}

void ViewModel::DoMovement(vec3 displacement)
{
	this->displacement += displacement;
	// move player
	glman_aabb.Move(displacement);
}

void ViewModel::Draw()
{
	if (initialized == false)
	{
		cout << "ERROR: Cannot render a ViewModel object before it has been initialized." << endl;
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// draw skybox first!
	skybox.Draw();

	// draw energizers
	for (auto i = 0; i < tiles->tiles.size(); ++i)
	{
		for (auto j = 0; j < tiles->tiles[i].size(); ++j)
		{
			Tile *tile = tiles->tiles[i][j].get();
			if (tile != nullptr && tile->energizer != nullptr)
				tile->energizer->OnDraw();
		}
	}

	// Show bounding box for player if debugging, and show axes/origin.
	if (__DEBUG__)
	{
		//origin.Draw(vec4(1.0f, 1.0f, 0.0f, 1.0f));
		//transform.model_matrix = translate(mat4(1.0), displacement);
		//UpdateTransform();
		//glman_bounding_box_renderable.Draw();
	}

	glFlush();
}

void ViewModel::UpdateTransform()
{
	//Pass the matrix info to the shaders in a Uniform Buffer Object.
	//This allows ALL shaders to be able to access the matrix information.
	glBufferData(GL_UNIFORM_BUFFER, sizeof(transform), &transform, GL_DYNAMIC_DRAW);//use GL_DYNAMIC_DRAW since it changes a lot
	glBindBufferBase(GL_UNIFORM_BUFFER, transform_binding, transform_id);  //The 35 needs to match the number used in the shaders for the matrices
}