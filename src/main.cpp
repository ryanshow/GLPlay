#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <thread>
#include <fstream>
#include <iostream>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#define ERROR(...) do { fmt::print(stderr, "{}:{} ", __FILE__, __LINE__); fmt::print(stderr, __VA_ARGS__); fmt::print(stderr, "\n"); } while (0)

using namespace gl;

static const double MAX_FPS_INTERVAL = (1/25.0f);

int main() {
    GLFWwindow *window;

    if(!glfwInit()) {
        ERROR("GLFW failed to init");
        exit(EXIT_FAILURE);
    }

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
#endif

    window = glfwCreateWindow(640, 480, "GLPlay", nullptr, nullptr);
    if(!window) {
        ERROR("GLFW failed to create the window");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glbinding::Binding::initialize();
    glfwSwapInterval(1);

    // === Load the font ===

    const char* ttf_filename = "assets/DroidSansMono.ttf";
    GLuint ftex;

    std::ifstream is (ttf_filename, std::ifstream::binary);

    if (!is) {
        ERROR("Unable to open {}", ttf_filename);
        exit(EXIT_FAILURE);
    }

    // determine the length of the file
    is.seekg(0, is.end);
    int length = static_cast<int>(is.tellg());
    is.seekg(0, is.beg);

    char *ttf_buffer = new char[length];

    fmt::print("Reading {} bytes from {}\n", length, ttf_filename);

    is.read(ttf_buffer, length);
    is.close();

    unsigned char temp_bitmap[512 * 512];

    stbtt_bakedchar cdata[96];

    stbtt_BakeFontBitmap(reinterpret_cast<unsigned char *>(ttf_buffer), 0, 48.0, temp_bitmap, 512, 512, 32, 95, cdata);

    glGenTextures(1, &ftex);

    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ftex); {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Begin Triangle Rendering Test

    GLfloat vertices[] = {
        // Positions         // Texture Coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f,   // Top Right
        0.5f, -0.5f, 0.0f,   1.0f, 1.0f,   // Bottom Right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f,   // Bottom Left
        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f    // Top Left
    };

    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };

    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    const GLchar* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 inPosition;\n"
    "layout (location = 1) in vec2 inTexCoord;\n"
    "out vec2 texCoord;\n"
    "void main() {\n"
    "  gl_Position = vec4(inPosition, 1.0);\n"
    "  texCoord = inTexCoord;\n"
    "}\0";

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    const GLchar* fragmentShaderSource = "#version 330 core\n"
    "in vec2 texCoord;\n"
    "out vec4 outColor;\n"
    "uniform sampler2D tex0;\n"
    "void main() {\n"
    "  outColor = vec4(1.0, 1.0, 0.0, texture(tex0, texCoord).r);\n"
    "}\0";

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO); {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ftex);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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