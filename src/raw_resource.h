#pragma once

#include <string>
#include <vector>

#include "resource.h"

namespace GLPlay {

class RawResource {
public:
    RawResource() {};

    void SetData(std::vector<unsigned char> & data);
    const std::vector<unsigned char> & GetData() const;

//    template<typename T>
//    void SetResource(std::string name, Resource<T> & resource);

private:
    std::vector<unsigned char> data_;
};

//template<typename T>
//void RawResource::SetResource(std::string name, Resource<T> & resource) {
//}

}