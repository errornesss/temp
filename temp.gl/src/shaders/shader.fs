#version 460 core

// in vec4 oCol;
in vec2 oTexCoord;

// uniform vec4 uCol;
uniform sampler2D texture0;
uniform sampler2D texture1;

out vec4 fragCol;

void main() {
  fragCol = mix(texture(texture0, oTexCoord), texture(texture1, oTexCoord), 0.2f) /* * oCol * uCol */;
}
