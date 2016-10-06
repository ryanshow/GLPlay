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

    int info_text_frame_time = ui_overlay->AddInfoText("Frame: ");

    double render_time;

    while (!glfwWindowShouldClose(window->glfw_window())) {

        glfwSetTime(0.0f);

        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        ui_overlay->Render();

        glfwSwapBuffers(window->glfw_window());
        glfwPollEvents();

        render_time = glfwGetTime();
        ui_overlay->UpdateInfoText(info_text_frame_time, fmt::format("Frame: {:5.2f}", render_time*1000));

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