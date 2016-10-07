#pragma once

#include <glm/glm.hpp>

namespace GLPlay {
class Viewport {
public:
    Viewport(int width, int height);

    void Resize(int width, int height);
    virtual void Update();

    const glm::mat4 & proj_matrix();
    const glm::mat4 & view_matrix();

    int width();
    int height();

protected:
    glm::mat4 proj_matrix_;
    glm::mat4 view_matrix_;

private:
    int width_;
    int height_;
};
}
