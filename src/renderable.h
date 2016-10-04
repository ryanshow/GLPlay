#pragma once

#include <vector>

#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>

using namespace gl;

namespace GLPlay {

class Renderable {
public:
    Renderable();
    ~Renderable();

    void Bind();
    void Render(glm::mat4 view_matrix, glm::mat4 proj_matrix);
    void SetTextureFromBitmap(unsigned char *bitmap, int width, int height);

    glm::mat4 *model_matrix();
    std::vector<GLfloat> vertices_;
    std::vector<GLuint> indices_;
private:
    enum gl_buffer_types { ARRAY_BUFFER = 0, ELEMENT_ARRAY_BUFFER = 1 };
    enum gl_object_types { ARRAY_OBJECT = 0 };
    GLuint gl_buffers_[2];
    GLuint gl_objects_[1];
    GLuint gl_shader_;
    glm::mat4 model_matrix_;
    GLuint gl_texture_;
};

}