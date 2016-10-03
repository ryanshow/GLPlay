#pragma once

namespace GLPlay {

class BitmapFont {
public:
    BitmapFont(unsigned char *font_data, float size);

private:
    unsigned char *bitmap;

};

}