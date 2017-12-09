//This is the fragment shader for the "overdraw count" example described in the 
//OpenGL Programming Guide (8th edition) and used to produce Figure 11.4 on page 582.  
//The code is available for download from http://www.opengl-redbook.com 

#version 420 core

// Buffer containing the rendered image
layout (binding = 0, r32ui) uniform uimage2D output_image;

// This is the output color
layout (location = 0) out vec4 color;

void main(void)
{
    color = vec4(imageLoad(output_image, ivec2(gl_FragCoord.xy)).xxxx) / 32.0;
}