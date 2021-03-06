#pragma once

#include <vector>

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GL/glext.h>
#else
#include <glbinding/gl/gl.h>
using namespace gl;
#endif

#include <glm/glm.hpp>
#include <map>
#include "vertex.h"
#include "event_source.h"
#include "resource.h"
#include "shader.h"


namespace GLPlay {

class VertexEventData : public EventData {
public:
    VertexEventData(int vertices, int indices) : vertices_(vertices), indices_(indices) {}
    int vertices_;
    int indices_;
};

class Renderable {
public:
    Renderable(std::string frag_shader, std::string vert_shader);
    ~Renderable();

    void Bind();
    void Render(glm::mat4 view_matrix, glm::mat4 proj_matrix);
    void SetTextureFromBitmap(unsigned char *bitmap, int width, int height);

    void SetTranslation(const glm::vec3 & translation);
    void SetRotation(const float rotation_theta, const glm::vec3 & rotation);
    void SetScale(const glm::vec3 & scale);

    void AddMesh(std::vector<Vertex> & vertices, std::vector<GLuint> & indices);
    void ClearMesh();

    void CalculateModelMatrix();

    glm::mat4 & model_matrix();

    enum RenderableEvent {VERTEX_EVENT};
    static EventSource<RenderableEvent> event_source_;

private:
    enum gl_buffer_types { ARRAY_BUFFER = 0, ELEMENT_ARRAY_BUFFER = 1, UNIFORM_BUFFER=2 };
    enum gl_object_types { ARRAY_OBJECT = 0 };
    GLuint gl_buffers_[3];
    GLuint gl_objects_[1];
    Shader * shader_;
    glm::vec3 translation_;
    float rotation_theta_;
    glm::vec3 rotation_vector_;
    glm::vec3 scale_;
    glm::mat4 model_matrix_;
    GLuint gl_texture_;

    std::vector<Vertex> vertices_;
    std::vector<GLuint> indices_;

};

}