#pragma once

#include "base_resource.h"

namespace GLPlay {

class Font : public BaseResource {
public:
    Font() {};
    Font(Font && other) : BaseResource(std::move(other)) {};
    constexpr static const char *resource_namespace = "fonts";
};

}