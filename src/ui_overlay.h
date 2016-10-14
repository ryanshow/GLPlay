#pragma once

#include "bitmap_font.h"
#include "renderable.h"
#include "window.h"
#include "ortho_viewport.h"

namespace GLPlay {

class UiOverlay {
public:
    UiOverlay(Window *window);
    ~UiOverlay();

    int AddInfoText(std::string text);
    void UpdateInfoText(int handler, std::string text);

    void Render();
private:
    void Update(int handler);

    OrthoViewport *viewport_;
    float font_size_;
    Resource<BitmapFont> * bitmap_font_;
    static int info_text_counter_;

    struct TextRenderable {
        std::string info_text_;
        Renderable * renderable_;
        bool dirty_;
    };
    std::map<int, TextRenderable> text_renderable_map_;
};

}