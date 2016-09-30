#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <thread>
#include <cstdarg>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include "resources/shaders.h"
#include "resources/fonts.h"

using namespace gl;

static const double MAX_FPS_INTERVAL = (1/25.0f);

template <typename... T>
void EXIT(const char* format, const T & ... args) {
    fmt::print(stderr, format, args...);
    fmt::print(stderr, "\n");
    std::exit(EXIT_FAILURE);
}

template <typename T, typename... U>
void EXIT_CHECK(T ret_code, const char * format, const U & ... args) {
    if (!ret_code) {
        EXIT(format, args...);
    }
};

void initGL(GLFWwindow *& window, int width, int height, const char *title) {
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

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if(!window) {
        glfwTerminate();
        EXIT("GLFW failed to create the window");
    }

    glfwMakeContextCurrent(window);
    glbinding::Binding::initialize();
    glfwSwapInterval(1);

    glViewport(0, 0, width, height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main() {
    GLFWwindow * window;
    initGL(window, 640, 480, "GLPlay");

    // === FONT SETUP ===

    stbtt_pack_context pack_cxt;
    stbtt_packedchar packed_char[95];
    unsigned char font_bitmap[512 * 512];

    stbtt_PackBegin(&pack_cxt, font_bitmap, 512, 512, 0, 1, nullptr);

    stbtt_PackSetOversampling(&pack_cxt, 1, 1);
    stbtt_PackFontRange(&pack_cxt, const_cast<unsigned char*>(DROID_SANS_MONO), 0, 24.0f, 32, 95, packed_char);
    //stbtt_PackSetOversampling(&pack_cxt, 2, 2);
    //stbtt_PackFontRange(&pack_cxt, const_cast<unsigned char*>(OPEN_SANS), 0, 24.0f, 32, 95, packed_char);
    //stbtt_PackSetOversampling(&pack_cxt, 3, 1);
    //stbtt_PackFontRange(&pack_cxt, const_cast<unsigned char*>(OPEN_SANS), 0, 24.0f, 32, 95, packed_char);

    stbtt_PackEnd(&pack_cxt);

    GLuint ftex;
    glGenTextures(1, &ftex);

    glBindTexture(GL_TEXTURE_2D, ftex); {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, font_bitmap);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    float x = 10.0f;
    float y = 0.0f;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    stbtt_aligned_quad q;
    std::string text("GLPlay");
    unsigned int i=0;

    for(auto c : text) {
        stbtt_GetPackedQuad(packed_char, 512, 512, c-32, &x, &y, &q, 1);

        vertices.push_back(q.x1); vertices.push_back(-q.y0); vertices.push_back(0.0f); vertices.push_back(q.s1); vertices.push_back(q.t0);
        vertices.push_back(q.x1); vertices.push_back(-q.y1); vertices.push_back(0.0f); vertices.push_back(q.s1); vertices.push_back(q.t1);
        vertices.push_back(q.x0); vertices.push_back(-q.y1); vertices.push_back(0.0f); vertices.push_back(q.s0); vertices.push_back(q.t1);
        vertices.push_back(q.x0); vertices.push_back(-q.y0); vertices.push_back(0.0f); vertices.push_back(q.s0); vertices.push_back(q.t0);

        indices.push_back(i+0); indices.push_back(i+1); indices.push_back(i+3);
        indices.push_back(i+1); indices.push_back(i+2); indices.push_back(i+3);

        i+= 4;
    }

    // === ARRAY / BUFFER OBJECT SETUP ===

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

    // === MODEL / VIEW / PROJECTION MATRIX SETUP ===

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    viewMatrix *= glm::lookAt(glm::vec3(0.0f, 0.0f, 500.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 projMatrix = glm::mat4(1.0f);
    //projMatrix *= glm::perspective(45.0f, 640.0f/480.0f, 0.1f, 1000.0f);
    projMatrix = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, 0.1f, 1000.0f);

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::rotate(modelMatrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 450.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));

    // ===  SHADER CREATION ===

    const char * shader_ptr;
    GLuint vertexShader, fragmentShader;

    shader_ptr = reinterpret_cast<const char *>(VERT_SIMPLE);
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shader_ptr, nullptr);
    glCompileShader(vertexShader);

    shader_ptr = reinterpret_cast<const char *>(FRAG_SIMPLE);
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

    // === GAME LOOP ===

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

    // === CLEAN UP ===

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}