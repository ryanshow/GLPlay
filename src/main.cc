#include <cstdarg>

#include <thread>

#include <fmt/format.h>
#include <glbinding/gl/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "bitmap_font.h"
#include "renderable.h"
#include "resources/fonts.h"
#include "utils.h"
#include "window.h"

using namespace gl;

namespace {
    constexpr double MAX_FPS_INTERVAL = (1 / 25.0f);
}

int main() {
    GLPlay::ExitCheck(glfwInit(), "GLFW failed to init");

    GLPlay::Window *window = new GLPlay::Window(640, 480, "GLPlay");

    GLPlay::BitmapFont bitmap_font = GLPlay::BitmapFont(const_cast<unsigned char*>(DROID_SANS_MONO), 48.0f);

    // == CREATE THE TEXT RENDER MESH ===

    GLPlay::Renderable ui_text = GLPlay::Renderable();

    float offset_x = 0.0f, offset_y = 0.0f;
    bitmap_font.GenerateTextMesh("Hello World", ui_text.vertices_, ui_text.indices_, offset_x, offset_y);
    offset_x = 0;
    offset_y += 100;
    bitmap_font.GenerateTextMesh("GLPlay", ui_text.vertices_, ui_text.indices_, offset_x, offset_y);

    ui_text.Bind();
    ui_text.gl_texture_ = bitmap_font.gl_texture_id();

    glm::mat4 view_matrix = glm::mat4(1.0f);
    view_matrix *= glm::lookAt(glm::vec3(0.0f, 0.0f, 500.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // === GAME LOOP ===

    while (!glfwWindowShouldClose(window->glfw_window())) {

        glfwSetTime(0.0f);

        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        ui_text.Render(view_matrix, *window->ui_viewport()->proj_matrix());

        glfwSwapBuffers(window->glfw_window());
        glfwPollEvents();

        if(glfwGetTime() < MAX_FPS_INTERVAL) {
            std::this_thread::sleep_for(
                std::chrono::microseconds(
                    int((MAX_FPS_INTERVAL - glfwGetTime()) * 1000000)
                )
            );
        }
    }


    glfwDestroyWindow(window->glfw_window());

    glfwTerminate();
    exit(EXIT_SUCCESS);
}