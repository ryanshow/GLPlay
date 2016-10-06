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
    constexpr int MAX_FPS = 25;
    constexpr double MAX_FPS_INTERVAL = (1.0f / MAX_FPS);
}
int main() {
    GLPlay::ExitCheck(glfwInit(), "GLFW failed to init");

    GLPlay::Window *window = new GLPlay::Window(640, 480, "GLPlay");

    GLPlay::UiOverlay *ui_overlay = new GLPlay::UiOverlay(window);

    int info_text_avg_frame_time = ui_overlay->AddInfoText("Frame Avg: ");
    int info_text_min_frame_time = ui_overlay->AddInfoText("Frame Min: ");
    int info_text_max_frame_time = ui_overlay->AddInfoText("Frame Max: ");
    int info_text_tris = ui_overlay->AddInfoText("Triangles: ");

    double render_time, min_render_time, max_render_time, avg_render_time;
    double rolling_render_time[MAX_FPS];
    int ticks = 0, tris = 0;

    while (!glfwWindowShouldClose(window->glfw_window())) {

        glfwSetTime(0.0f);

        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        ui_overlay->Render();

        glfwSwapBuffers(window->glfw_window());
        glfwPollEvents();

        if (ticks != 0 && ticks % MAX_FPS == 0) {
            min_render_time = -1.0f;
            max_render_time = -1.0f;
            avg_render_time = 0.0f;
            for (int i = 0; i < MAX_FPS; i++) {
                avg_render_time += rolling_render_time[i];
                if (min_render_time < 0 || rolling_render_time[i] < min_render_time) min_render_time = rolling_render_time[i];
                if (max_render_time < 0 || rolling_render_time[i] > max_render_time) max_render_time = rolling_render_time[i];
            }
            avg_render_time /= MAX_FPS;

            ui_overlay->UpdateInfoText(info_text_min_frame_time, fmt::format("Frame Min: {:5.2f}ms ({:5.1f} FPS)", min_render_time * 1000, 1.0f / min_render_time));
            ui_overlay->UpdateInfoText(info_text_avg_frame_time, fmt::format("Frame Avg: {:5.2f}ms ({:5.1f} FPS)", avg_render_time * 1000, 1.0f / avg_render_time));
            ui_overlay->UpdateInfoText(info_text_max_frame_time, fmt::format("Frame Max: {:5.2f}ms ({:5.1f} FPS)", max_render_time * 1000, 1.0f / max_render_time));
        }

        if (GLPlay::Renderable::bound_dirty_) {
            tris = 0;
            for (auto renderable : GLPlay::Renderable::renderables_) {
                tris += renderable.second->Triangles();
            }
            GLPlay::Renderable::bound_dirty_ = false;
            ui_overlay->UpdateInfoText(info_text_tris, fmt::format("Triangles: {}", tris));
        }

        render_time = glfwGetTime();

        rolling_render_time[ticks++ % MAX_FPS] = render_time;

        if(render_time < MAX_FPS_INTERVAL) {
            std::this_thread::sleep_for(
                std::chrono::microseconds(
                    int((MAX_FPS_INTERVAL - render_time) * 1000000)
                )
            );
        }
    }

    delete ui_overlay;
    delete window;

    glfwTerminate();
    exit(EXIT_SUCCESS);
}