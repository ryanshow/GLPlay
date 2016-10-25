#pragma once

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GL/glext.h>
#else
#include <glbinding/gl/gl.h>
using namespace gl;
#endif

#include "resource.h"

namespace GLPlay {

class ShaderResource {
public:
    ShaderResource() {};

    void SetData(std::vector<unsigned char> & data);
    GLuint GetShader(GLenum shader_type, std::string name);

//    template<typename T>
//    void SetResource(std::string name, Resource<T> & resource);
private:
    std::map<std::string, std::string> source_map_;
    std::map<std::string, GLuint> gl_shader_map_;
};

//template<typename T>
//void ShaderResource::SetResource(std::string name, Resource<T> & resource) {}

}