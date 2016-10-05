#pragma once

#include <GLFW/glfw3.h>
#include <map>
#include <vector>

#include "viewport.h"
#include "event_source.h"

namespace GLPlay {

class ResizeEventData : public EventData {
public:
    ResizeEventData(int width, int height) : width_(width), height_(height) {}
    int width_;
    int height_;
};

class Window {
public:
    Window(int width, int height, const char * title);

    enum WindowEvent {RESIZE_EVENT};
    EventSource<WindowEvent> event_source_;

    void Resize(int width, int height);
    static Window *GetWindowByGlfw(GLFWwindow *glfw_window);

    GLFWwindow *glfw_window();
    int width();
    int height();

private:
    GLFWwindow *glfw_window_;
    int width_;
    int height_;
    const char *title_;
    static std::map<GLFWwindow*, Window*> window_map_;
};

}

extern "C" {

void WindowResizeCallback(GLFWwindow *window, int width, int height);

}
