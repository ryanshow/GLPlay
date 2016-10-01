#include "window.h"

#include <functional>

#include <glbinding/Binding.h>

#include "utils.h"
#include "viewport.h"

std::map<GLFWwindow*, GLPlay::Window*> GLPlay::Window::window_map_;

extern "C" {

void WindowResizeCallback(GLFWwindow *glfw_window, int width, int height) {
    GLPlay::Window *window = GLPlay::Window::GetWindowByGlfw(glfw_window);
    window->Resize(width, height);
    fmt::print("Window was resized! width: {}, height: {}\n", width, height);
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

    glfw_window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!glfw_window_) {
        glfwTerminate();
        Exit("GLFW failed to create the window");
    }

    Window::window_map_.emplace(glfw_window_, this);

    glfwMakeContextCurrent(glfw_window_);
    glbinding::Binding::initialize();
    glfwSwapInterval(1);

    ui_viewport_ = new Viewport(0, 0, width_, height_);
    ui_viewport_->Resize(0, 0, width_, height_);

    glfwSetWindowSizeCallback(glfw_window_, WindowResizeCallback);
}

void Window::Resize(int width, int height) {
    width_ = width;
    height_ = height;
    ui_viewport_->Resize(0, 0, width, height);
}

Viewport *Window::ui_viewport() {
    return ui_viewport_;
}

GLFWwindow *Window::glfw_window() {
    return glfw_window_;
}

Window *Window::GetWindowByGlfw(GLFWwindow *window) {
    return Window::window_map_[window];
}

}
