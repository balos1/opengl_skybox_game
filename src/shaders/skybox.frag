#version 420

in vec3 texcoords;
uniform samplerCube CubeTexture;
out vec4 fColor;

void main() {
  fColor = texture(CubeTexture, texcoords);
}