#include <string>
#include <fmt/format.h>
#include "shader.h"

namespace GLPlay {

Shader::Shader(std::string frag_shader, std::string vert_shader) {
    shader_resource_ = new Resource<ShaderResource>("shaders/default");
    gl_shader_ = glCreateProgram();
    glAttachShader(gl_shader_, (**shader_resource_).GetShader(GL_VERTEX_SHADER, frag_shader));
    glAttachShader(gl_shader_, (**shader_resource_).GetShader(GL_FRAGMENT_SHADER, vert_shader));
    glLinkProgram(gl_shader_);
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(gl_shader_, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(gl_shader_, 512, NULL, infoLog);
        fmt::print("ERROR::SHADER::PROGRAM::LINKING_FAILED\n{}\n", infoLog);
    }

    GLuint ubi = glGetUniformBlockIndex(gl_shader_, "Matricies");
    glUniformBlockBinding(gl_shader_, ubi, 0);
}

}