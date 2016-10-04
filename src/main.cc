#include <thread>

#include <glbinding/gl/gl.h>
#include <GLFW/glfw3.h>

#include "bitmap_font.h"
#include "renderable.h"
#include "utils.h"
#include "window.h"
#include "ui_overlay.h"

using namespace gl;

namespace {
    constexpr double MAX_FPS_INTERVAL = (1 / 25.0f);
}

int main() {
    GLPlay::ExitCheck(glfwInit(), "GLFW failed to init");

    GLPlay::Window *window = new GLPlay::Window(640, 480, "GLPlay");

    GLPlay::UiOverlay *ui_overlay = new GLPlay::UiOverlay(window);

    int tris_count = 0;
    double render_time = 0.0f;

    while (!glfwWindowShouldClose(window->glfw_window())) {

        glfwSetTime(0.0f);

        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        ui_overlay->Render();

        glfwSwapBuffers(window->glfw_window());
        glfwPollEvents();

        /*
        float offset_x = 0.0f, offset_y = 0.0f;
        ui_text.vertices_.clear();
        ui_text.indices_.clear();
        bitmap_font.GenerateTextMesh(fmt::format("Render Time: {:05.2f}ms", render_time*1000), ui_text.vertices_, ui_text.indices_, offset_x, offset_y);
        offset_x = 0.0f;
        offset_y += 15.0f;
        bitmap_font.GenerateTextMesh(fmt::format("Triangles: {}", tris_count), ui_text.vertices_, ui_text.indices_, offset_x, offset_y);

        ui_text.Bind();

        tris_count = ui_text.indices_.size()/3;
        */

        render_time = glfwGetTime();

        if(render_time < MAX_FPS_INTERVAL) {
            std::this_thread::sleep_for(
                std::chrono::microseconds(
                    int((MAX_FPS_INTERVAL - render_time) * 1000000)
                )
            );
        }
    }


    glfwDestroyWindow(window->glfw_window());

    glfwTerminate();
    exit(EXIT_SUCCESS);
}