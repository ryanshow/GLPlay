#include "raw_resource.h"

namespace GLPlay {

void RawResource::SetData(std::vector<unsigned char> & data) {
    data_ = std::move(data);
}

const std::vector<unsigned char> & RawResource::GetData() const {
    return data_;
}



}