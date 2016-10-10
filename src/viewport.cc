#include "viewport.h"

#include <fmt/format.h>

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#else
#include <glbinding/gl/gl.h>
using namespace gl;
#endif

#include <glm/gtc/matrix_transform.hpp>


namespace GLPlay {

Viewport::Viewport(int width, int height) : width_{width}, height_{height} {
    Update();
}

void Viewport::Resize(int width, int height) {
    width_ = width;
    height_ = height;
    Update();
}

void Viewport::Update() {
    // TODO: Accept a bitmask as argument so we can select which ones to update

    glViewport(0, 0, width_, height_);

    view_matrix_ = glm::mat4(1.0f);
    view_matrix_ *= glm::lookAt(
        glm::vec3(0.0f, 0.0f, 500.0f),
        glm::vec3(0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

const glm::mat4 & Viewport::proj_matrix() {
    return proj_matrix_;
}

const glm::mat4 & Viewport::view_matrix() {
    return view_matrix_;
}

int Viewport::width() {
    return width_;
}

int Viewport::height() {
    return height_;
}

}