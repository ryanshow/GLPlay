#version 330 core

in vec2 texCoord;

out vec4 outColor;

uniform sampler2D tex0;

void main() {
  outColor = vec4(1.0, 1.0, 0.0, texture(tex0, texCoord).r);
}
