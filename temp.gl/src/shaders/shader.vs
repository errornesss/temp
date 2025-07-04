#version 460 core

layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aCol;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// out vec4 oCol;
out vec2 oTexCoord;

void main() {
  // oCol = vec4(aCol, 1.0f);
  oTexCoord = aTexCoord;
  gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
