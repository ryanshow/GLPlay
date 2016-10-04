#include "viewport.h"

#include <fmt/format.h>
#include <glbinding/gl/gl.h>
#include <glm/gtc/matrix_transform.hpp>

namespace GLPlay {

Viewport::Viewport(int x, int y, int width, int height) : x_{x}, y_{y}, width_{width}, height_{height} {
    Update();
}

void Viewport::Resize(int x, int y, int width, int height) {
    x_ = x;
    y_ = y;
    width_ = width;
    height_ = height;
    Update();
}

void Viewport::Update() {
    // TODO: Accept a bitmask as argument so we can select which ones to update

    gl::glViewport(x_, y_, width_, height_);
    proj_matrix_ = glm::ortho(static_cast<float>(x_), static_cast<float>(width_), static_cast<float>(y_), static_cast<float>(height_), 0.1f, 1000.0f);

    view_matrix_ = glm::mat4(1.0f);
    view_matrix_ *= glm::lookAt(glm::vec3(0.0f, 0.0f, 500.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 * Viewport::proj_matrix() {
    return &proj_matrix_;
}

glm::mat4 * Viewport::view_matrix() {
    return &view_matrix_;
}

int Viewport::width() {
    return width_;
}

int Viewport::height() {
    return height_;
}

}