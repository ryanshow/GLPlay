#include "bitmap_font.h"

#define STB_RECT_PACK_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_rect_pack.h>
#include <stb_truetype.h>
#include <fmt/format.h>

#include "vertex.h"

using namespace gl;

namespace GLPlay {

BitmapFont::BitmapFont(unsigned char *font_buffer, float size) : bitmap_width_{256}, bitmap_height_{256}, start_char_{32}, char_count_{95} {
    stbtt_pack_context pack_cxt;
    char_data_ = new stbtt_packedchar[char_count_];
    bitmap_ = new unsigned char[bitmap_width_ * bitmap_height_];

    stbtt_PackBegin(&pack_cxt, bitmap_, bitmap_width_, bitmap_height_, 0, 1, nullptr);
    stbtt_PackSetOversampling(&pack_cxt, 1, 1);
    stbtt_PackFontRange(&pack_cxt, font_buffer, 0, size, start_char_, char_count_, char_data_);
    stbtt_PackEnd(&pack_cxt);
}

BitmapFont::~BitmapFont() {
    delete[] bitmap_;
    delete[] char_data_;
}

stbtt_packedchar *BitmapFont::char_data() {
    return char_data_;
}

void BitmapFont::GenerateTextMesh(std::string text, Renderable & renderable, float &offset_x, float &offset_y) {
    stbtt_aligned_quad q;
    for (auto c : text) {
        stbtt_GetPackedQuad(char_data(), bitmap_width_, bitmap_height_, c-start_char_, &offset_x, &offset_y, &q, 1);

        std::vector<Vertex> verts = {
            {{q.x1, -q.y0, 0.0f}, {}, {}, {q.s1, q.t0}, {}},
            {{q.x1, -q.y1, 0.0f}, {}, {}, {q.s1, q.t1}, {}},
            {{q.x0, -q.y1, 0.0f}, {}, {}, {q.s0, q.t1}, {}},
            {{q.x0, -q.y0, 0.0f}, {}, {}, {q.s0, q.t0}, {}}
        };

        std::vector<GLuint> indices = {
            0, 1, 3,
            1, 2, 3
        };

        renderable.AddMesh(verts, indices);
    }
}

unsigned char *BitmapFont::bitmap() {
    return bitmap_;
}

int BitmapFont::bitmap_width() {
    return bitmap_width_;

}

int BitmapFont::bitmap_height() {
    return bitmap_height_;
}

}