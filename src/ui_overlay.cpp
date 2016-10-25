#include "ui_overlay.h"

#include <fmt/format.h>
#include <glm/gtc/matrix_transform.hpp>

namespace GLPlay {

int UiOverlay::info_text_counter_ = 0;

UiOverlay::UiOverlay(Window *window) {
    fmt::printf("+++ Constructing UiOverlay +++\n");

    viewport_ = new OrthoViewport(window->width(), window->height());

    bitmap_font_ = new BitmapFont("default", 24.0f);

    int info_text_win_size = AddInfoText("Hello");

    auto window_resize_callback = [info_text_win_size, this](const EventData &event_data) {
        const ResizeEventData & resize_event_data = static_cast<const ResizeEventData&>(event_data);
        viewport_->Resize(resize_event_data.width_, resize_event_data.height_);
        UpdateInfoText(info_text_win_size, fmt::format("Window: {}, {}", resize_event_data.width_, resize_event_data.height_));

        for (auto & info_text : text_renderable_map_) {
            info_text.second.dirty_ = true;
        }
    };

    window_resize_callback(ResizeEventData(window->width(), window->height()));

    window->event_source_.RegisterHandler(Window::RESIZE_EVENT, window_resize_callback);
}

UiOverlay::~UiOverlay() {
    fmt::printf("--- Destroying UiOverlay ---\n");
    delete viewport_;
    delete bitmap_font_;
    for (auto & info_text : text_renderable_map_) {
        delete info_text.second.renderable_;
    }
}

void UiOverlay::Update(int handle) {
    float x, y;
    TextRenderable &text_renderable = text_renderable_map_[handle];

    x = 0.0f;
    y = 0.0f;
    text_renderable.renderable_->ClearMesh();
    bitmap_font_->GenerateTextMesh(text_renderable.info_text_, *text_renderable.renderable_, x, y);
    text_renderable.renderable_->SetTranslation(glm::vec3(0.0f, viewport_->height()-(bitmap_font_->size()*(handle+1)), 0.0f));
    text_renderable.renderable_->Bind();
    text_renderable.dirty_ = false;
}

void UiOverlay::Render() {
    for (auto & info_text : text_renderable_map_) {
        if (info_text.second.dirty_) {
            Update(info_text.first); // TODO: Update should be passed a handler
        }
        info_text.second.renderable_->Render(viewport_->view_matrix(), viewport_->proj_matrix());
    }

}

int UiOverlay::AddInfoText(std::string text) {
    int handler = info_text_counter_++;

    text_renderable_map_.emplace(handler, TextRenderable {});

    text_renderable_map_[handler].info_text_ = text;
    text_renderable_map_[handler].dirty_ = true;
    text_renderable_map_[handler].renderable_ = new Renderable("default", "r8alpha");
    text_renderable_map_[handler].renderable_->SetTextureFromBitmap(bitmap_font_->bitmap(), bitmap_font_->bitmap_width(), bitmap_font_->bitmap_height());

    return handler;
}

void UiOverlay::UpdateInfoText(int handler, std::string text) {
    text_renderable_map_[handler].info_text_ = text;
    text_renderable_map_[handler].dirty_ = true;
}

}