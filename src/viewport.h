#pragma once

#include <glm/glm.hpp>

namespace GLPlay {
class Viewport {
public:
    Viewport(int x, int y, int width, int height);

    void Resize(int x, int y, int width, int height);
    void Update();

    glm::mat4 * proj_matrix();

private:
    int x_;
    int y_;
    int width_;
    int height_;
    glm::mat4 proj_matrix_;
};
}
