#include <thread>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#include <glbinding/gl/gl.h>
using namespace gl;
#endif

#include <GLFW/glfw3.h>
#include <physfs.h>

#include "bitmap_font.h"
#include "utils.h"
#include "window.h"
#include "ui_overlay.h"
#include "resource.h"
#include "shader.h"
#include "resource_data.h"
#include "mesh_resource.h"

#include <glm/gtc/matrix_transform.hpp>


namespace {
    constexpr int MAX_FPS = 30;
    constexpr double MAX_FPS_INTERVAL = (1.0f / MAX_FPS);

    static GLPlay::Window *window;
    static GLPlay::UiOverlay *ui_overlay;
    static int info_text_avg_frame_time;
    static int info_text_min_frame_time;
    static int info_text_max_frame_time;
    static int info_text_tris;
    static double render_time, min_render_time, max_render_time, avg_render_time;
    static double rolling_render_time[MAX_FPS];
    static int ticks = 0, tris = 0, rendered_tris = -1;

    static GLPlay::Resource<GLPlay::MeshResource> * mesh_resource;

    static glm::mat4 view_matrix = glm::mat4(1.0f);

    static glm::mat4 proj_matrix = glm::perspective(
        45.0f,
        static_cast<float>(640)/static_cast<float>(480),
        0.1f, 1000.0f
    );

}

void MainLoop() {
    glfwSetTime(0.0f);

    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ui_overlay->Render();

    (**mesh_resource).renderable_->Render(view_matrix, proj_matrix);

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

    if (tris != rendered_tris) {
        ui_overlay->UpdateInfoText(info_text_tris, fmt::format("Triangles: {}", tris));
        rendered_tris = tris;
    }

    glfwSwapBuffers(window->glfw_window());

    render_time = glfwGetTime();

    rolling_render_time[ticks++ % MAX_FPS] = render_time;
}

int main(int argc, char ** argv) {
    PHYSFS_init(argv[0]);
    //PHYSFS_setSaneConfig("ryanshow", "GLPlay", nullptr, 0, 0);

    PHYSFS_mountMemory(generated_resources_7z, generated_resources_7z_len, nullptr, nullptr, nullptr, 0);

    GLPlay::ExitCheck(glfwInit(), "GLFW failed to init");

    window = new GLPlay::Window(640, 480, "GLPlay");

    const GLubyte *gl_version = glGetString(GL_VERSION);
    const GLubyte *glsl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);

    fmt::print("OpenGL: {}\nGLSL: {}\n", gl_version, glsl_version);

    ui_overlay = new GLPlay::UiOverlay(window);

    view_matrix *= glm::lookAt(
            glm::vec3(2.0f, 3.0f, 5.0f),
            glm::vec3(0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));

    auto vertex_callback = [&](const GLPlay::EventData &event_data) {
        const GLPlay::VertexEventData & vertex_event_data = static_cast<const GLPlay::VertexEventData&>(event_data);
        tris += (vertex_event_data.indices_/3);
    };

    GLPlay::Renderable::event_source_.RegisterHandler(GLPlay::Renderable::VERTEX_EVENT, vertex_callback);

    mesh_resource = new GLPlay::Resource<GLPlay::MeshResource>("meshes/monkey");

    info_text_avg_frame_time = ui_overlay->AddInfoText("Frame Avg: ");
    info_text_min_frame_time = ui_overlay->AddInfoText("Frame Min: ");
    info_text_max_frame_time = ui_overlay->AddInfoText("Frame Max: ");
    info_text_tris = ui_overlay->AddInfoText("Triangles: ");


#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(MainLoop, MAX_FPS, 1);
#else
    while (!glfwWindowShouldClose(window->glfw_window())) {

        MainLoop();

        if(render_time < MAX_FPS_INTERVAL) {
            std::this_thread::sleep_for(
                std::chrono::microseconds(
                    int((MAX_FPS_INTERVAL - render_time) * 1000000)
                )
            );
        }
    }
#endif

    delete ui_overlay;
    delete window;

    glfwTerminate();
    PHYSFS_deinit();

    exit(EXIT_SUCCESS);
}