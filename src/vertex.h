#pragma once

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 nor;
    glm::vec4 col;
    glm::vec2 tx0;
    glm::vec2 tx1;
};