//This is a simple pass-through vertex shader used by the filter 

#version 400 core

layout(location = 0) in vec4  vertexPosition;
layout(location = 1) in vec4  vertexColor;
layout(location = 2) in vec3  vertexNormal;

vec2 dummy;

void main()
{
	// need this so GLSL compiler doesnt optimize vertexColor and vertexNormal out
	dummy = vec2(vertexNormal.x, vertexColor.r);
	gl_Position = vertexPosition;
}