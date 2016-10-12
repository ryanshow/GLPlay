#pragma once

#include <map>

class Shader {
public:
    Shader();

private:
    static std::map<const unsigned char *, Shader *> shader_map_;
};