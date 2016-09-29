#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <thread>
#include <cstdarg>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include "resources/shaders.h"
#include "resources/fonts.h"

using namespace gl;

static const double MAX_FPS_INTERVAL = (1/25.0f);

template <typename T, typename... U>
void EXIT_CHECK(T ret_code, const char* format, const U & ... args) {
    if (!ret_code) {
        fmt::print(stderr, format, args...);
        fmt::print(stderr, "\n");
        std::exit(EXIT_FAILURE);
    }
};

int main() {
    EXIT_CHECK(glfwInit(), "GLFW failed to init");

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
#endif

    GLFWwindow *window = glfwCreateWindow(640, 480, "GLPlay", nullptr, nullptr);
    if(!window) {
        glfwTerminate();
        EXIT_CHECK(window, "GLFW failed to create the window");
    }

    glfwMakeContextCurrent(window);
    glbinding::Binding::initialize();
    glfwSwapInterval(1);

    glViewport(0, 0, 640, 480);

    // === Load the font ===

    GLuint ftex;

    unsigned char temp_bitmap[512 * 512];
    stbtt_bakedchar cdata[96];

    stbtt_BakeFontBitmap(DROID_SANS_MONO, 0, 48.0, temp_bitmap, 512, 512, 32, 95, cdata);

    glGenTextures(1, &ftex);

    glBindTexture(GL_TEXTURE_2D, ftex); {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    float x = -130.0f;
    float y = 0.0f;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    stbtt_aligned_quad q;
    std::string text("GLPlay");
    unsigned int i=0;

    for(auto c : text) {
        float x0 = x;
        float y0 = y;

        fmt::print("X: {}, Y: {}\n\n", x, y);

        stbtt_GetBakedQuad(cdata, 512, 512, c-32, &x, &y, &q, 1);

        fmt::print("X0: {}, Y0: {}, S0: {}, T0: {}\n", q.x0, q.x1, q.s0, q.t0);
        fmt::print("X1: {}, Y1: {}, S1: {}, T1: {}\n\n", q.x1, q.y1, q.s1, q.t1);

        vertices.push_back(x0+q.x1); vertices.push_back(-q.y0); vertices.push_back(0.0f); vertices.push_back(q.s1); vertices.push_back(q.t0);
        vertices.push_back(x0+q.x1); vertices.push_back(-q.y1); vertices.push_back(0.0f); vertices.push_back(q.s1); vertices.push_back(q.t1);
        vertices.push_back(x0+q.x0); vertices.push_back(-q.y1); vertices.push_back(0.0f); vertices.push_back(q.s0); vertices.push_back(q.t1);
        vertices.push_back(x0+q.x0); vertices.push_back(-q.y0); vertices.push_back(0.0f); vertices.push_back(q.s0); vertices.push_back(q.t0);

        indices.push_back(i+0); indices.push_back(i+1); indices.push_back(i+3);
        indices.push_back(i+1); indices.push_back(i+2); indices.push_back(i+3);

        i+= 4;
    }

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    viewMatrix *= glm::lookAt(glm::vec3(0.0f, 0.0f, 500.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 projMatrix = glm::mat4(1.0f);
    //projMatrix *= glm::perspective(45.0f, 640.0f/480.0f, 0.1f, 1000.0f);
    projMatrix = glm::ortho(-320.0f, 320.0f, -240.0f, 240.0f, 0.1f, 1000.0f);

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::rotate(modelMatrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));

    // glShaderSource expects double pointer for it's source reference
    const char * shader_ptr = reinterpret_cast<const char *>(VERT_SIMPLE);
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shader_ptr, nullptr);
    glCompileShader(vertexShader);

    shader_ptr = reinterpret_cast<const char *>(FRAG_SIMPLE);
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shader_ptr, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO); {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*vertices.size(), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    // End Triangle Font Rendering Test


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window)) {

        glfwSetTime(0.0f);

        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO); {
            glUseProgram(shaderProgram);

            glUniform1i(glGetUniformLocation(shaderProgram, "tex0"), 0);
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projMatrix"), 1, GL_FALSE, glm::value_ptr(projMatrix));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));


            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ftex);

            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        if(glfwGetTime() < MAX_FPS_INTERVAL) {
            std::this_thread::sleep_for(
                    std::chrono::microseconds(
                            int((MAX_FPS_INTERVAL - glfwGetTime()) * 1000000)
                    )
            );
        }
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}