#include "renderable.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "resources/shaders.h"


namespace GLPlay {

Renderable::Renderable() {
    glGenBuffers(2, gl_buffers_);
    glGenVertexArrays(1, gl_objects_);

    model_matrix_ = glm::mat4(1.0f);
    model_matrix_ = glm::rotate(model_matrix_, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    model_matrix_ = glm::translate(model_matrix_, glm::vec3(0.0f, 0.0f, 0.0f));
    model_matrix_ = glm::scale(model_matrix_, glm::vec3(1.0f, 1.0f, 1.0f));

    const char * shader_ptr;
    GLuint vertexShader, fragmentShader;

    shader_ptr = reinterpret_cast<const char *>(VERT_SIMPLE);
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shader_ptr, nullptr);
    glCompileShader(vertexShader);

    shader_ptr = reinterpret_cast<const char *>(FRAG_SIMPLE);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shader_ptr, nullptr);
    glCompileShader(fragmentShader);

    gl_shader_ = glCreateProgram();
    glAttachShader(gl_shader_, vertexShader);
    glAttachShader(gl_shader_, fragmentShader);
    glLinkProgram(gl_shader_);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Renderable::~Renderable() {
    glDeleteVertexArrays(1, gl_objects_);
    glDeleteBuffers(2, gl_buffers_);
}

void Renderable::Bind() {
    glBindVertexArray(gl_objects_[ARRAY_OBJECT]); {
        glBindBuffer(GL_ARRAY_BUFFER, gl_buffers_[ARRAY_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices_.size(), vertices_.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_buffers_[ELEMENT_ARRAY_BUFFER]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices_.size(), indices_.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

}

void Renderable::Render(glm::mat4 view_matrix, glm::mat4 proj_matrix) {
    glBindVertexArray(gl_objects_[ARRAY_OBJECT]); {
        glUseProgram(gl_shader_);

        glUniform1i(glGetUniformLocation(gl_shader_, "tex0"), 0);
        glUniformMatrix4fv(glGetUniformLocation(gl_shader_, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));
        glUniformMatrix4fv(glGetUniformLocation(gl_shader_, "projMatrix"), 1, GL_FALSE, glm::value_ptr(proj_matrix));
        glUniformMatrix4fv(glGetUniformLocation(gl_shader_, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model_matrix_));

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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

glm::mat4 *Renderable::model_matrix() {
    return &model_matrix_;
}

}