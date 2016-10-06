#pragma once

#include "bitmap_font.h"
#include "renderable.h"
#include "window.h"

namespace GLPlay {

class UiOverlay {
public:
    UiOverlay(Window *window);

    int AddInfoText(std::string text);
    void UpdateInfoText(int handler, std::string text);

    void Update();

    void Render();
private:
    Renderable *text_renderable_;
    Viewport *viewport_;
    float font_size_;
    BitmapFont *bitmap_font_;
    std::map<int, std::string> info_text_;
    static int info_text_counter_;
};

}