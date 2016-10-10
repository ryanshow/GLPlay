#pragma once

#include "renderable.h"

namespace GLPlay {

class Entity {
public:
    Entity();
private:
    Renderable * renderable_;
    glm::vec3 pos_;
};

}