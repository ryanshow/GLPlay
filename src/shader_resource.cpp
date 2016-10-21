#include "shader_resource.h"

namespace GLPlay {

void ShaderResource::SetData(std::vector<unsigned char> & data) {
    std::string shader_str(data.begin(), data.end());

    size_t shader_pos = shader_str.find("// === ", 0);

    int line_count = 1;

    for(size_t i=shader_str.find("\n", 0); i < shader_pos; i = shader_str.find("\n", i+1)) {
        line_count++;
    }

    while (shader_pos != std::string::npos) {
        size_t shader_name_pos = shader_pos + 7;
        size_t next_shader_pos = shader_str.find("// === ", shader_name_pos);
        if (next_shader_pos == std::string::npos) {
            next_shader_pos = shader_str.length();
        }

        std::string shader_name = shader_str.substr(shader_name_pos, shader_str.find(" ===", shader_name_pos)-shader_name_pos);
        std::string shader_body = shader_str.substr(shader_pos, (next_shader_pos)-shader_pos);

        shader_body.insert(0, fmt::format("#line {}\n", line_count));

        for(size_t i=shader_str.find("\n", shader_pos); i < next_shader_pos; i = shader_str.find("\n", i+1)) {
            line_count++;
        }

        source_map_[shader_name] = shader_body;

        if (next_shader_pos == shader_str.length()) {
            break;
        }
        shader_pos = next_shader_pos;
    }
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
            fmt::print("ERROR: Shader Compilation Failed\n{}\n", infoLog);
        }
        return shader_id;
    }
}

}