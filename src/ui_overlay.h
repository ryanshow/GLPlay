#pragma once

#include "bitmap_font.h"
#include "renderable.h"
#include "window.h"

namespace GLPlay {

class UiOverlay {
public:
    UiOverlay(Window *window);

    void Render();
private:
    Renderable *text_renderable_;
    Viewport *viewport_;
    float font_size_;
    BitmapFont *bitmap_font_;
};

}