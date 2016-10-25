/*
   _____ _                 __        _____ __              __
  / ___/(_)___ ___  ____  / /__     / ___// /_  ____ _____/ /__  _____
  \__ \/ / __ `__ \/ __ \/ / _ \    \__ \/ __ \/ __ `/ __  / _ \/ ___/
 ___/ / / / / / / / /_/ / /  __/   ___/ / / / / /_/ / /_/ /  __/ /    
/____/_/_/ /_/ /_/ .___/_/\___/   /____/_/ /_/\__,_/\__,_/\___/_/     
                /_/
*/

// ==========================
// === vertex.gl3.default ===
// ==========================

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_nor;
layout (location = 2) in vec4 in_col;
layout (location = 3) in vec2 in_tx0;
layout (location = 4) in vec2 in_tx1;

layout (std140) uniform Matricies {
    mat4 view_matrix;
    mat4 proj_matrix;
};

uniform mat4 model_matrix;

out vec4 v_col;
out vec2 v_tx0;

void main() {
  gl_Position = proj_matrix * view_matrix * model_matrix * vec4(in_pos, 1.0);
  v_tx0 = in_tx0;
  v_col = in_col;
}

// ============================
// === fragment.gl3.r8alpha ===
// ============================

in vec4 v_col;
in vec2 v_tx0;

out vec4 outColor;

uniform sampler2D tex0;

void main() {
  outColor = vec4(1.0, 1.0, 0.0, texture(tex0, v_tx0).r);
}

// ============================
// === fragment.gl3.default ===
// ============================

in vec4 v_col;
in vec2 v_tx0;

out vec4 out_color;

void main() {
  out_color = vec4(v_col);
}
