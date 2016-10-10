#pragma once

#include "renderable.h"

namespace GLPlay {

class Scene {
public:
    Scene();
public:
    std::vector<Renderable> renderables_;
};

}
