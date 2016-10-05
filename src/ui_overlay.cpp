#include "ui_overlay.h"

#include <fmt/format.h>
#include <glm/gtc/matrix_transform.hpp>

#include "resources/fonts.h"

namespace GLPlay {

UiOverlay::UiOverlay(Window *window) {
    font_size_ = 15.0f;

    viewport_ = new Viewport(0, 0, window->width(), window->height());

    bitmap_font_ = new BitmapFont(const_cast<unsigned char*>(DROID_SANS_MONO), font_size_);

    text_renderable_ = new Renderable();
    text_renderable_->SetTranslation(glm::vec3(0.0f, window->height()-font_size_, 0.0f));
    text_renderable_->SetTextureFromBitmap(bitmap_font_->bitmap(), bitmap_font_->bitmap_width(), bitmap_font_->bitmap_height());

    float offset_x = 0.0f;
    float offset_y = 0.0f;
    bitmap_font_->GenerateTextMesh(fmt::format("GLPlay"), text_renderable_->vertices_, text_renderable_->indices_, offset_x, offset_y);

    text_renderable_->Bind();

    auto window_resize_callback = [this](const EventData &event_data) {
        const ResizeEventData & resize_event_data = static_cast<const ResizeEventData&>(event_data);
        viewport_->Resize(0, 0, resize_event_data.width_, resize_event_data.height_);
        text_renderable_->SetTranslation(glm::vec3(0.0f, resize_event_data.height_-this->font_size_, 0.0f));

        // TODO: Following code should reside in it's own instance method. Need an instance variable of string/screencord
        // TODO: pairs that it iterates over. Maybe then have pointers to specific items so they can be found and updated
        float x = 0.0f;
        float y = 0.0f;
        text_renderable_->vertices_.clear();
        text_renderable_->indices_.clear();
        bitmap_font_->GenerateTextMesh(fmt::format("Window: {}, {}", resize_event_data.width_, resize_event_data.height_), text_renderable_->vertices_, text_renderable_->indices_, x, y);

        text_renderable_->Bind();
    };

    window_resize_callback(ResizeEventData(window->width(), window->height()));

    window->event_source_.RegisterHandler(Window::RESIZE_EVENT, window_resize_callback);
}

void UiOverlay::Render() {
    text_renderable_->Render(*viewport_->view_matrix(), *viewport_->proj_matrix());
}

}