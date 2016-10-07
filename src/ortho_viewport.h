#pragma once

#include "viewport.h"

namespace GLPlay {

class OrthoViewport : public Viewport {
public:
    OrthoViewport(int width, int height);

    virtual void Update();
private:

};

}