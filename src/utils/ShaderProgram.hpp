/***
author     : r3dux
version    : 0.3 - 15/01/2014
description: Gets GLSL source code either provided as strings or can load from filenames,
			 compiles the shaders, creates a shader program which the shaders are linked
			 to, then the program is validated and is ready for use via myProgram.use(),
			 <Draw-stuff-here> then calling myProgram.disable();

			 Attributes and uniforms are stored in <string, int> maps and can be added
			 via calls to addAttribute(<name-of-attribute>) and then the attribute
			 index can be obtained via myProgram.attribute(<name-of-attribute>) - Uniforms
			 work in the exact same way.

extended by: Cody Balos
***/

#pragma once

#include <glew.h>
#include <map>
#include "../config.inl"

class ShaderProgram
{
private:
	// static DEBUG flag - if set to false then, errors aside, we'll run completely silent
	static const bool DEBUG = __DEBUG__;

	// We'll use an enum to differentiate between shaders and shader programs when querying the info log
	enum class ObjectType
	{
		SHADER, PROGRAM
	};

	// Shader program and individual shader Ids
	GLuint programId;
	GLuint vertexShaderId;
	GLuint fragmentShaderId;

	// How many shaders are attached to the shader program
	GLuint shaderCount;

	// Map of attributes and their binding locations
	std::map<std::string, GLuint> attributeMap;

	// Map of uniforms and their binding locations
	std::map<std::string, GLuint> uniformMap;

	// Map of textures/samplers and thier binding locations
	std::map<std::string, GLuint> textureMap;

	// Has this shader program been initialised?
	bool initialised;

	// ---------- PRIVATE METHODS ----------

	// Private method to compile a shader of a given type
	static GLuint compileShader(std::string shaderSource, GLenum shaderType);

	// Private method to compile/attach/link/verify the shaders.
	// Note: Rather than returning a boolean as a success/fail status we'll just consider
	// a failure here to be an unrecoverable error and throw a runtime_error.
	void initialise(std::string vertexShaderSource, std::string fragmentShaderSource);

	// Private method to load the shader source code from a file
	static std::string loadShaderFromFile(const std::string filename);

	// Private method to return the current shader program info log as a string
	static std::string getInfoLog(ObjectType type, GLuint id);

public:
	// Constructor
	ShaderProgram();

	// Destructor
	~ShaderProgram();

	GLuint getProgramID() const { return programId; };

	// Method to initialise a shader program from shaders provided as files
	void initFromFiles(std::string vertexShaderFilename, std::string fragmentShaderFilename);

	// Method to initialise a shader program from shaders provided as strings
	void initFromStrings(std::string vertexShaderSource, std::string fragmentShaderSource);

	// Method to enable the shader program - we'll suggest this for inlining
	void use() const;

	// Method to disable the shader - we'll also suggest this for inlining
	static void disable();

	// Method to return the bound location of a named attribute, or -1 if the attribute was not found
	GLuint attribute(const std::string attributeName);

	// Method to returns the bound location of a named uniform
	GLuint uniform(const std::string uniformName);

	// Method to returns the bound location of a named uniform
	GLuint texture(const std::string textureName);

	// Method to add an attribute to the shader and return the bound location
	int addAttribute(const std::string attributeName);

	// Method to add a uniform to the shader and return the bound location
	int addUniform(const std::string uniformName);

	// Method to add a uniform to the shader with a index.
	int addIndexedUniform(const std::string uniformName, GLuint index);

	// Method to add a texture to the shader and return the generated ID.
	int addTexture(const std::string textureName);
}; // End of class