#pragma once

#include <map>
#include <string>
#include <utility>

namespace GLPlay {

class BaseResource {
public:
    BaseResource() {};
    BaseResource(BaseResource && other) : resource_properties_{std::move(other.resource_properties_)} {};

    void SetProperty(std::string name, const unsigned char * value);
    const unsigned char * GetProperty(std::string key) const;

    void Compile() {};

private:
    std::map<std::string, const unsigned char *> resource_properties_;
};

}