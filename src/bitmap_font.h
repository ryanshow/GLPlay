#pragma once

#include <vector>

#ifndef __EMSCRIPTEN__
#include <glbinding/gl/gl.h>
using namespace gl;
#endif

#include <stb_truetype.h>

#include "renderable.h"
#include "raw_resource.h"


namespace GLPlay {

class BitmapFont {
public:
    BitmapFont(std::string name, float size);
    BitmapFont(BitmapFont&& other);
    ~BitmapFont();

    unsigned char * bitmap() const;
    int bitmap_width() const;
    int bitmap_height() const;
    int size() const;
    stbtt_packedchar * char_data() const;

    void GenerateTextMesh(std::string text, Renderable & renderable, float &offset_x, float &offset_y) const;

private:
    stbtt_packedchar *char_data_;
    unsigned char *bitmap_;
    int bitmap_width_;
    int bitmap_height_;
    int start_char_;
    int char_count_;
    float size_;
    Resource<RawResource> * font_resource_;
};

}