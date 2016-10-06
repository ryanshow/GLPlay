#include "ui_overlay.h"

#include <fmt/format.h>
#include <glm/gtc/matrix_transform.hpp>

#include "resources/fonts.h"

namespace GLPlay {

int UiOverlay::info_text_counter_ = 0;

UiOverlay::UiOverlay(Window *window) {
    font_size_ = 15.0f;

    viewport_ = new Viewport(0, 0, window->width(), window->height());

    bitmap_font_ = new BitmapFont(const_cast<unsigned char*>(DROID_SANS_MONO), font_size_);

    text_renderable_ = new Renderable();
    text_renderable_->SetTranslation(glm::vec3(0.0f, window->height()-font_size_, 0.0f));
    text_renderable_->SetTextureFromBitmap(bitmap_font_->bitmap(), bitmap_font_->bitmap_width(), bitmap_font_->bitmap_height());

    int info_text_win_size = AddInfoText("Hello");

    auto window_resize_callback = [info_text_win_size, this](const EventData &event_data) {
        const ResizeEventData & resize_event_data = static_cast<const ResizeEventData&>(event_data);
        viewport_->Resize(0, 0, resize_event_data.width_, resize_event_data.height_);
        text_renderable_->SetTranslation(glm::vec3(0.0f, resize_event_data.height_-this->font_size_, 0.0f));
        UpdateInfoText(info_text_win_size, fmt::format("Window: {}, {}", resize_event_data.width_, resize_event_data.height_));
    };

    window_resize_callback(ResizeEventData(window->width(), window->height()));

    window->event_source_.RegisterHandler(Window::RESIZE_EVENT, window_resize_callback);
}

void UiOverlay::Update() {
    float x = 0.0f;
    float y = 0.0f;
    text_renderable_->vertices_.clear();
    text_renderable_->indices_.clear();
    for (auto info_text : info_text_) {
        bitmap_font_->GenerateTextMesh(info_text.second, text_renderable_->vertices_, text_renderable_->indices_, x, y);
        x=0.0f;
        y+=font_size_;
    }
    text_renderable_->Bind();
}

void UiOverlay::Render() {
    text_renderable_->Render(*viewport_->view_matrix(), *viewport_->proj_matrix());
}

int UiOverlay::AddInfoText(std::string text) {
    int handler = info_text_counter_++;
    info_text_.emplace(handler, text);
    Update();
    return handler;
}

void UiOverlay::UpdateInfoText(int handler, std::string text) {
    info_text_[handler] = text;
    Update();
}

}