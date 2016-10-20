#include "renderable.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fmt/format.h>

namespace GLPlay {

EventSource<Renderable::RenderableEvent> Renderable::event_source_;

Renderable::Renderable() {
    glGenBuffers(3, gl_buffers_);
    glGenVertexArrays(1, gl_objects_);

    shader_ = new Shader("default");

    // FIXME: This should be defined on the shader object (once it exists)
    GLuint ubi = glGetUniformBlockIndex((*shader_).gl_shader_, "Matricies");
    glUniformBlockBinding((*shader_).gl_shader_, ubi, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, gl_buffers_[UNIFORM_BUFFER]);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, gl_buffers_[UNIFORM_BUFFER], 0, 2 * sizeof(glm::mat4));

    rotation_theta_ = 0.0f;
    rotation_vector_ = glm::vec3(1.0f, 0.0f, 0.0f);
    translation_ = glm::vec3(0.0f, 0.0f, 0.0f);
    scale_  = glm::vec3(1.0f, 1.0f, 1.0f);

    CalculateModelMatrix();
}

Renderable::~Renderable() {
    glDeleteVertexArrays(1, gl_objects_);
    glDeleteBuffers(2, gl_buffers_);
    event_source_.GenerateEvent(VERTEX_EVENT, VertexEventData(-vertices_.size(), -indices_.size()));
}

void Renderable::Bind() {
    glBindVertexArray(gl_objects_[ARRAY_OBJECT]); {
        glBindBuffer(GL_ARRAY_BUFFER, gl_buffers_[ARRAY_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices_.size(), vertices_.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_buffers_[ELEMENT_ARRAY_BUFFER]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices_.size(), indices_.data(), GL_STATIC_DRAW);

        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, pos)));
        glEnableVertexAttribArray(0);

        // Normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, nor)));
        glEnableVertexAttribArray(1);

        // Color
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, col)));
        glEnableVertexAttribArray(2);

        // Texture 0
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, tx0)));
        glEnableVertexAttribArray(3);

        // Texture 1
        glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, tx1)));
        glEnableVertexAttribArray(4);

        glBindVertexArray(0);
    }

}

void Renderable::AddMesh(std::vector<Vertex> & vertices, std::vector<GLuint> & indices) {
    int i = vertices_.size();
    for (auto & vertex : vertices) {
        vertices_.emplace_back(vertex);
    }
    for (auto & index : indices) {
        indices_.emplace_back(i+index);
    }
    Bind();
    event_source_.GenerateEvent(VERTEX_EVENT, VertexEventData(vertices.size(), indices.size()));
}

void Renderable::ClearMesh() {
    event_source_.GenerateEvent(VERTEX_EVENT, VertexEventData(-vertices_.size(), -indices_.size()));
    vertices_.clear();
    indices_.clear();
}

void Renderable::Render(glm::mat4 view_matrix, glm::mat4 proj_matrix) {
    // FIXME: matricies should not be passed in here. Maybe observer on the shader object which monitors matrix changes?

    glBindBuffer(GL_UNIFORM_BUFFER, gl_buffers_[UNIFORM_BUFFER]);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view_matrix));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(proj_matrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindVertexArray(gl_objects_[ARRAY_OBJECT]); {
        glUseProgram((*shader_).gl_shader_);

        glUniform1i(glGetUniformLocation((*shader_).gl_shader_, "tex0"), 0);
        glUniformMatrix4fv(glGetUniformLocation((*shader_).gl_shader_, "model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix_));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gl_texture_);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_.size()), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }
}

void Renderable::SetTextureFromBitmap(unsigned char *bitmap, int width, int height) {
    glGenTextures(1, &gl_texture_);
    glBindTexture(GL_TEXTURE_2D, gl_texture_); {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Renderable::CalculateModelMatrix() {
    model_matrix_ = glm::mat4(1.0f);
    model_matrix_ = glm::rotate(model_matrix_, rotation_theta_, rotation_vector_);
    model_matrix_ = glm::translate(model_matrix_, translation_);
    model_matrix_ = glm::scale(model_matrix_, scale_);
}

void Renderable::SetTranslation(const glm::vec3 & translation) {
    translation_ = translation;
    CalculateModelMatrix();
}

void Renderable::SetRotation(const float rotation_theta, const glm::vec3 & rotation_vector) {
    rotation_theta_ = rotation_theta;
    rotation_vector_ = rotation_vector;
    CalculateModelMatrix();
}

void Renderable::SetScale(const glm::vec3 & scale) {
    scale_ = scale;
    CalculateModelMatrix();
}

glm::mat4 & Renderable::model_matrix() {
    return model_matrix_;
}

}