#pragma once

#include <vector>
#include "vertex.h"
#include "renderable.h"

namespace GLPlay {

class MeshResource {
public:
    MeshResource();

    void SetData(std::vector<unsigned char> & data);

//private:
    // TODO: renderable is probably not the right type here. Should probably exist as an intermediate
    Renderable * renderable_;
};

}