#pragma once

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GL/glext.h>
#else
#include <glbinding/gl/gl.h>
using namespace gl;
#endif

#include <map>

namespace GLPlay {

class Shader {
public:
    constexpr static const char *resource_namespace = "shaders";

    void SetProperty(std::string name, const unsigned char * value);
    void Compile();

    GLuint gl_shader_;
private:
    std::map<std::string, const unsigned char *> resource_properties_;
};

}