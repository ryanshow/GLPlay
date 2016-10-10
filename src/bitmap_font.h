#pragma once

#include <vector>

#ifndef __EMSCRIPTEN__
#include <glbinding/gl/gl.h>
using namespace gl;
#endif

#include <stb_truetype.h>

#include "renderable.h"


namespace GLPlay {

class BitmapFont {
public:
    BitmapFont(unsigned char *font_data, float size);
    ~BitmapFont();

    unsigned char *bitmap();
    int bitmap_width();
    int bitmap_height();
    stbtt_packedchar *char_data();

    void GenerateTextMesh(std::string text, Renderable & renderable, float &offset_x, float &offset_y);

private:
    stbtt_packedchar *char_data_;
    unsigned char *bitmap_;
    int bitmap_width_;
    int bitmap_height_;
    int start_char_;
    int char_count_;
};

}