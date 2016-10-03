#include "bitmap_font.h"

#define STB_RECT_PACK_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_rect_pack.h>
#include <stb_truetype.h>

using namespace gl;

namespace GLPlay {

BitmapFont::BitmapFont(unsigned char *font_buffer, float size) : bitmap_width_{256}, bitmap_height_{256}, start_char_{32}, char_count_{95} {
    stbtt_pack_context pack_cxt;
    char_data_ = new stbtt_packedchar[char_count_];
    unsigned char *temp_bitmap = new unsigned char[bitmap_width_ * bitmap_height_];

    stbtt_PackBegin(&pack_cxt, temp_bitmap, bitmap_width_, bitmap_height_, 0, 1, nullptr);
    stbtt_PackSetOversampling(&pack_cxt, 1, 1);
    stbtt_PackFontRange(&pack_cxt, font_buffer, 0, size, start_char_, char_count_, char_data_);
    stbtt_PackEnd(&pack_cxt);

    glGenTextures(1, &gl_texture_id_);
    glBindTexture(GL_TEXTURE_2D, gl_texture_id_); {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmap_width_, bitmap_height_, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    delete[] temp_bitmap;
}

BitmapFont::~BitmapFont() {
    glDeleteTextures(1, &gl_texture_id_);
    delete[] char_data_;
}

stbtt_packedchar *BitmapFont::char_data() {
    return char_data_;
}

GLuint BitmapFont::gl_texture_id() {
    return gl_texture_id_;
}

void BitmapFont::GenerateTextMesh(std::string text, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices, float &offset_x, float &offset_y) {
    unsigned int i = indices.size()/6*4;
    stbtt_aligned_quad q;

    for(auto c : text) {
        stbtt_GetPackedQuad(char_data(), bitmap_width_, bitmap_height_, c-start_char_, &offset_x, &offset_y, &q, 1);

        vertices.push_back(q.x1); vertices.push_back(-q.y0); vertices.push_back(0.0f); vertices.push_back(q.s1); vertices.push_back(q.t0);
        vertices.push_back(q.x1); vertices.push_back(-q.y1); vertices.push_back(0.0f); vertices.push_back(q.s1); vertices.push_back(q.t1);
        vertices.push_back(q.x0); vertices.push_back(-q.y1); vertices.push_back(0.0f); vertices.push_back(q.s0); vertices.push_back(q.t1);
        vertices.push_back(q.x0); vertices.push_back(-q.y0); vertices.push_back(0.0f); vertices.push_back(q.s0); vertices.push_back(q.t0);

        indices.push_back(i+0); indices.push_back(i+1); indices.push_back(i+3);
        indices.push_back(i+1); indices.push_back(i+2); indices.push_back(i+3);

        i += 4;
    }
}

}