#pragma once

#include "viewport.h"

namespace GLPlay {

class PerspectiveViewport : public Viewport {
public:
    PerspectiveViewport(int width, int height, float fov);

    virtual void Update();
private:
    float fov_;
};
}