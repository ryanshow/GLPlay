#include "bitmap_font.h"

#define STB_RECT_PACK_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_rect_pack.h>
#include <stb_truetype.h>
#include <fmt/format.h>

#include "vertex.h"

namespace GLPlay {

BitmapFont::BitmapFont(std::string name, float size) : bitmap_width_{256}, bitmap_height_{256}, start_char_{32}, char_count_{95}, size_{size} {
    fmt::print("+++ Constructing Bitmap +++\n");
    font_ = new Resource<Font>(name);
    char_data_ = new stbtt_packedchar[char_count_];
    bitmap_ = new unsigned char[bitmap_width_ * bitmap_height_];

    fmt::print("Compiling bitmap.\n");
    stbtt_pack_context pack_cxt;
    stbtt_PackBegin(&pack_cxt, bitmap_, bitmap_width_, bitmap_height_, 0, 1, nullptr);
    stbtt_PackSetOversampling(&pack_cxt, 1, 1);
    stbtt_PackFontRange(&pack_cxt, const_cast<unsigned char *>((**font_).GetProperty("font")), 0, size_, start_char_, char_count_, char_data_);
    stbtt_PackEnd(&pack_cxt);
}

BitmapFont::BitmapFont(BitmapFont && other) : size_{other.size_}, bitmap_width_{other.bitmap_width_}, bitmap_height_{other.bitmap_height_}, start_char_{other.start_char_}, char_count_{other.char_count_}, bitmap_{other.bitmap_}, char_data_{other.char_data_} {
    other.bitmap_ = nullptr;
    other.char_data_ = nullptr;
}

BitmapFont::~BitmapFont() {
    fmt::print("--- Destroying bitmap ---\n");
    delete[] bitmap_;
    delete[] char_data_;
}

stbtt_packedchar * BitmapFont::char_data() const {
    return char_data_;
}

void BitmapFont::GenerateTextMesh(std::string text, Renderable & renderable, float &offset_x, float &offset_y) const {
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

unsigned char *BitmapFont::bitmap() const {
    return bitmap_;
}

int BitmapFont::bitmap_width() const {
    return bitmap_width_;

}

int BitmapFont::bitmap_height() const {
    return bitmap_height_;
}

int BitmapFont::size() const {
    return size_;
}

}