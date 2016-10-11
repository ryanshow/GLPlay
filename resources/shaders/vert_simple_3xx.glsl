layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec4 inCol;
layout (location = 3) in vec2 inTx0;
layout (location = 4) in vec2 inTx1;

layout (std140) uniform Matricies {
    mat4 view_matrix;
    mat4 proj_matrix;
};

uniform mat4 model_matrix;

out vec2 tex0_coord;

void main() {
  gl_Position = proj_matrix * view_matrix * model_matrix * vec4(inPos, 1.0);
  tex0_coord = inTx0;
}