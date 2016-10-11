layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec4 inCol;
layout (location = 3) in vec2 inTx0;
layout (location = 4) in vec2 inTx1;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 modelMatrix;

out vec2 texCoord;

void main() {
  gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(inPos, 1.0);
  texCoord = inTx0;
}