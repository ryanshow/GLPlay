#include <string>
#include <fmt/format.h>
#include "shader.h"

namespace GLPlay {

Shader::Shader(std::string name) {
    shader_resource_ = new Resource<ShaderResource>(fmt::format("shaders/{}", name));
    gl_shader_ = glCreateProgram();
    glAttachShader(gl_shader_, (**shader_resource_).GetShader(GL_VERTEX_SHADER));
    glAttachShader(gl_shader_, (**shader_resource_).GetShader(GL_FRAGMENT_SHADER));
    glLinkProgram(gl_shader_);
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(gl_shader_, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(gl_shader_, 512, NULL, infoLog);
        fmt::print("ERROR::SHADER::PROGRAM::LINKING_FAILED\n{}\n", infoLog);
    }
}

}