#include "base_resource.h"

namespace GLPlay {

void BaseResource::SetProperty(std::string key, const unsigned char * value) {
    resource_properties_[key] = value;
}

const unsigned char * BaseResource::GetProperty(std::string key) const {
    return resource_properties_.at(key);
}

};
