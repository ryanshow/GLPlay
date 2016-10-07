#include "perspective_viewport.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GLPlay {

PerspectiveViewport::PerspectiveViewport(int width, int height, float fov) : Viewport(width, height), fov_(fov) {
}

void PerspectiveViewport::Update() {
    proj_matrix_ = glm::perspective(
        fov_,
        static_cast<float>(width())/static_cast<float>(height()),
        0.1f, 1000.0f
    );

    Viewport::Update();
}

}