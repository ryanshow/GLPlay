#pragma once

#include <vector>

#include <glbinding/gl/gl.h>
#include <stb_truetype.h>

using namespace gl;

namespace GLPlay {

class BitmapFont {
public:
    BitmapFont(unsigned char *font_data, float size);
    ~BitmapFont();

    stbtt_packedchar *char_data();
    GLuint gl_texture_id();

    void GenerateTextMesh(std::string text, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices, float &offset_x, float &offset_y);

private:
    stbtt_packedchar *char_data_;
    GLuint gl_texture_id_;
    int bitmap_width_;
    int bitmap_height_;
    int start_char_;
    int char_count_;
};

}