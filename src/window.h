#pragma once

#include <GLFW/glfw3.h>
#include <map>
#include <vector>

#include "viewport.h"

namespace GLPlay {

class Window {
public:
    Window(int width, int height, const char * title);

    void Resize(int width, int height);
    static Window *GetWindowByGlfw(GLFWwindow *glfw_window);
    Viewport *CreateViewport();

    Viewport *ui_viewport();
    GLFWwindow *glfw_window();
    int width();
    int height();

private:
    GLFWwindow *glfw_window_;
    int width_;
    int height_;
    const char *title_;
    static std::map<GLFWwindow*, Window*> window_map_;
    std::vector<Viewport*> viewports_;
};

}

extern "C" {

void WindowResizeCallback(GLFWwindow *window, int width, int height);

}
