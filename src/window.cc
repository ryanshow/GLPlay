#include "window.h"

#ifndef __EMSCRIPTEN__
#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
using namespace gl;
#endif

#include "utils.h"
#include "viewport.h"

std::map<GLFWwindow*, GLPlay::Window*> GLPlay::Window::window_map_;

extern "C" {

void WindowResizeCallback(GLFWwindow *glfw_window, int width, int height) {
    GLPlay::Window *window = GLPlay::Window::GetWindowByGlfw(glfw_window);
    window->Resize(width, height);
}

}

namespace GLPlay {


Window::Window(int width, int height, const char * title) :
    width_{width},
    height_{height},
    title_{title} {

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
#endif

    glfwWindowHint(GLFW_SAMPLES, 4);

    glfw_window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!glfw_window_) {
        glfwTerminate();
        Exit("GLFW failed to create the window");
    }

    Window::window_map_.emplace(glfw_window_, this);

    glfwMakeContextCurrent(glfw_window_);
#ifndef __EMSCRIPTEN__
    glbinding::Binding::initialize();
#endif
    glfwSwapInterval(1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_MULTISAMPLE);

    glfwSetWindowSizeCallback(glfw_window_, WindowResizeCallback);
}

Window::~Window() {
    fmt::print("Destroying Window");
    glfwDestroyWindow(glfw_window_);
}

void Window::Resize(int width, int height) {
    width_ = width;
    height_ = height;
    event_source_.GenerateEvent(RESIZE_EVENT, ResizeEventData(width, height));
}

GLFWwindow *Window::glfw_window() {
    return glfw_window_;
}

Window *Window::GetWindowByGlfw(GLFWwindow *window) {
    return Window::window_map_[window];
}

int Window::width() {
    return width_;
}

int Window::height() {
    return height_;
}

}
