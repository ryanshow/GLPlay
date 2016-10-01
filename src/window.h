#pragma once

#include <GLFW/glfw3.h>
#include <map>

#include "viewport.h"

namespace GLPlay {

class Window {
public:
    Window(int width, int height, const char * title);

    void Resize(int width, int height);
    Viewport *ui_viewport();
    GLFWwindow *glfw_window();
    static Window *GetWindowByGlfw(GLFWwindow *glfw_window);

private:
    GLFWwindow *glfw_window_;
    Viewport *ui_viewport_;
    int width_;
    int height_;
    const char *title_;
    static std::map<GLFWwindow*, Window*> window_map_;
};

}

extern "C" {

void WindowResizeCallback(GLFWwindow *window, int width, int height);

}
