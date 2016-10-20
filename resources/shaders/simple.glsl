/*
   _____ _                 __        _____ __              __
  / ___/(_)___ ___  ____  / /__     / ___// /_  ____ _____/ /__  _____
  \__ \/ / __ `__ \/ __ \/ / _ \    \__ \/ __ \/ __ `/ __  / _ \/ ___/
 ___/ / / / / / / / /_/ / /  __/   ___/ / / / / /_/ / /_/ /  __/ /    
/____/_/_/ /_/ /_/ .___/_/\___/   /____/_/ /_/\__,_/\__,_/\___/_/     
                /_/
*/

// ==================
// === vertex.gl3 ===
// ==================

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

// ====================
// === fragment.gl3 ===
// ====================

in vec2 tex0_coord;

out vec4 outColor;

uniform sampler2D tex0;

void main() {
  outColor = vec4(1.0, 1.0, 0.0, texture(tex0, tex0_coord).r);
}
