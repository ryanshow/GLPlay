#include "ortho_viewport.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GLPlay {

OrthoViewport::OrthoViewport(int width, int height) : Viewport(width, height) {
}

void OrthoViewport::Update() {
    proj_matrix_ = glm::ortho(
        0.0f, static_cast<float>(width()),
        0.0f, static_cast<float>(height()),
        0.1f, 1000.0f
    );

    Viewport::Update();
}

}