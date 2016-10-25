#pragma once

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GL/glext.h>
#else
#include <glbinding/gl/gl.h>
using namespace gl;
#endif

#include "resource.h"
#include "shader_resource.h"

#include <map>

namespace GLPlay {

class Shader {
public:
    Shader(std::string frag_shader, std::string vert_shader);

    GLuint gl_shader_;
private:
    Resource<ShaderResource> * shader_resource_;
};

}