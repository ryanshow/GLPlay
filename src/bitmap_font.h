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
    BitmapFont();
    BitmapFont(BitmapFont&& other);
    ~BitmapFont();

    constexpr static const char *resource_namespace = "fonts";
    void SetProperty(std::string name, const unsigned char * value);
    void Compile();

    unsigned char * bitmap() const;
    int bitmap_width() const;
    int bitmap_height() const;
    stbtt_packedchar * char_data() const;

    void GenerateTextMesh(std::string text, Renderable & renderable, float &offset_x, float &offset_y) const;

private:
    stbtt_packedchar *char_data_;
    unsigned char *bitmap_;
    int bitmap_width_;
    int bitmap_height_;
    int start_char_;
    int char_count_;
    std::map<std::string, const unsigned char *> resource_properties_;
};

}