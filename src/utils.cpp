#define STB_RECT_PACK_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION

#include "utils.h"

#include <stb_rect_pack.h>
#include <stb_truetype.h>

namespace GLPlay {
unsigned char *CreateTextureAtlas(unsigned char *font_buffer, float size) {
    stbtt_pack_context pack_cxt;
    stbtt_packedchar packed_char[95];
    unsigned char *font_bitmap = new unsigned char[256 * 256];

    stbtt_PackBegin(&pack_cxt, font_bitmap, 256, 256, 0, 1, nullptr);
    stbtt_PackSetOversampling(&pack_cxt, 1, 1);
    stbtt_PackFontRange(&pack_cxt, font_buffer, 0, size, 32, 95, packed_char);
    stbtt_PackEnd(&pack_cxt);

    return font_bitmap;
}
}