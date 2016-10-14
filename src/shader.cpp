#include <string>
#include <fmt/format.h>
#include "shader.h"

namespace GLPlay {

void Shader::Compile() {

    fmt::print("Compiling Shader...\n");

    const char * shader_ptr[2];
    GLuint vertexShader, fragmentShader;

#ifdef __EMSCRIPTEN__
    const char *vertex_shader_preamble = \
        "#version 300 es\n";
#else
    const char *vertex_shader_preamble = \
        "#version 330\n";
#endif

    shader_ptr[0] = vertex_shader_preamble;
    shader_ptr[1] = reinterpret_cast<const char*>(GetProperty("vertex"));

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 2, shader_ptr, nullptr);
    glCompileShader(vertexShader);
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fmt::print("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}\n", infoLog);
    };

#ifdef __EMSCRIPTEN__
    const char *frag_shader_preamble = \
        "#version 300 es\n" \
        "precision highp float;\n";
#else
    const char *frag_shader_preamble = \
        "#version 330\n";
#endif

    shader_ptr[0] = frag_shader_preamble;
    shader_ptr[1] = reinterpret_cast<const char*>(GetProperty("fragment"));

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 2, shader_ptr, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fmt::print("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{}\n", infoLog);
    };

    gl_shader_ = glCreateProgram();
    glAttachShader(gl_shader_, vertexShader);
    glAttachShader(gl_shader_, fragmentShader);
    glLinkProgram(gl_shader_);
    glGetProgramiv(gl_shader_, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(gl_shader_, 512, NULL, infoLog);
        fmt::print("ERROR::SHADER::PROGRAM::LINKING_FAILED\n{}\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

}