#pragma once

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GL/glext.h>
#else
#include <glbinding/gl/gl.h>
using namespace gl;
#endif

#include <map>
#include "base_resource.h"

namespace GLPlay {

class Shader : public BaseResource {
public:
    Shader() {};
    Shader(Shader && other) : BaseResource(std::move(other)), gl_shader_{other.gl_shader_} {};

    constexpr static const char *resource_namespace = "shaders";

    void Compile();

    GLuint gl_shader_;
};

}