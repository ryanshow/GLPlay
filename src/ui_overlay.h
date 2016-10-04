#pragma once

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
};

}