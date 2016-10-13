#pragma once

#include <map>
#include <fmt/format.h>
#include "shader.h"

namespace GLPlay {

template<typename T>
class Resource {
public:
    Resource(const Resource<T> & resource);
    Resource(const char * name);
    ~Resource();

private:
    const char * name_;
    T * resource_;

    static std::map<const char *, int> ref_count_map_;
    static std::map<const char *, T> resource_map_;
};

template<typename T>
Resource::Resource(const Resource<T> & resource) : name_(resource.name_), resource_(resource.resource_) {
    ref_count_map_[name_] += 1;
}

template<typename T>
Resource::Resource(const char * name) : name_(name) {
    try {
        ref_count_map_.at(name) += 1;
    } catch (const std::out_of_range & e) {
        ref_count_map_[name] = 1;
    }

    try {
        resource_ = resource_map_.at(name);
    } catch (const std::out_of_range & e) {
        T resource = T(name);
        resource.set_attribute(attr_name, attr_data);
        resource_map_.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(name),
            std::forward_as_tuple(data)
        );
    }
}

template<typename T>
Resource::~Resource() {
    ref_count_map_[name_] -= 1;
    resource_ = nullptr;
    if (ref_count_map_[name_] == 0) {
        resource_map_.erase(name_);
    }
}

}