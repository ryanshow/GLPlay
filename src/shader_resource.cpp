#include "shader_resource.h"

namespace GLPlay {

void ShaderResource::SetData(std::vector<unsigned char> & data) {
    std::string shader_str = reinterpret_cast<char *>(data.data());

    size_t shader_pos = shader_str.find("// === ", 0)+7;

    while (shader_pos != std::string::npos) {
        size_t next_shader_pos = shader_str.find("// === ", shader_pos);
        std::string shader_name = shader_str.substr(shader_pos, shader_str.find(" ===", shader_pos)-shader_pos);
        std::string shader_body = shader_str.substr(shader_str.find("\n", shader_pos)+1, next_shader_pos-shader_str.find("\n", shader_pos)+1);
        fmt::print("shader name: {}\n", shader_name);
        fmt::print("shader body: {}\n", shader_body);
        if (next_shader_pos != std::string::npos) {
            next_shader_pos += 7;
        }
        shader_pos = next_shader_pos;
    }

    fmt::print("ShaderResource::SetData\n");
    source_map_["vertex.gl3"] = "layout (location = 0) in vec3 inPos;\n"
        "layout (location = 1) in vec3 inNor;\n"
        "layout (location = 2) in vec4 inCol;\n"
        "layout (location = 3) in vec2 inTx0;\n"
        "layout (location = 4) in vec2 inTx1;\n"
        "layout (std140) uniform Matricies {\n"
        "        mat4 view_matrix;\n"
        "        mat4 proj_matrix;\n"
        "};\n"
        "uniform mat4 model_matrix;\n"
        "out vec2 tex0_coord;\n"
        "void main() {\n"
        "    gl_Position = proj_matrix * view_matrix * model_matrix * vec4(inPos, 1.0);\n"
        "    tex0_coord = inTx0;\n"
        "}\n";

    source_map_["fragment.gl3"] = "in vec2 tex0_coord;\n"
        "out vec4 outColor;\n"
        "uniform sampler2D tex0;\n"
        "void main() {\n"
        "    outColor = vec4(1.0, 1.0, 0.0, texture(tex0, tex0_coord).r);\n"
        "}\n";
}

GLuint ShaderResource::GetShader(GLenum shader_type) {
    std::string shader_string;
    if (shader_type == GL_VERTEX_SHADER) {
        shader_string = "vertex";
    }
    else if (shader_type == GL_FRAGMENT_SHADER) {
        shader_string = "fragment";
    }

    shader_string += ".gl3";


    try {
        return gl_shader_map_.at(shader_string);
    } catch (const std::out_of_range & e) {
        GLuint shader_id = glCreateShader(shader_type);
        gl_shader_map_[shader_string] = shader_id;

        const char ** shader_src = new const char * [2];

#ifdef __EMSCRIPTEN__
        if (shader_type == GL_VERTEX_SHADER) {
            shader_src[0] = "#version 300 es\n";
        } else if (shader_type == GL_FRAGMENT_SHADER) {
            shader_src[0] = "#version 300 es\nprecision highp float;\n";
        }
#else
        shader_src[0] = "#version 330\n";
#endif

        shader_src[1] = source_map_[shader_string].c_str();

        glShaderSource(shader_id, 2, shader_src, nullptr);
        glCompileShader(shader_id);
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
            fmt::print("ERROR::SHADER::XXX::COMPILATION_FAILED\n{}\n", infoLog);
        }
        return shader_id;
    }
}

}