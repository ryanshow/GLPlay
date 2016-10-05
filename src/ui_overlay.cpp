#include "ui_overlay.h"

#include <fmt/format.h>
#include <glm/gtc/matrix_transform.hpp>

#include "bitmap_font.h"
#include "resources/fonts.h"

namespace GLPlay {

UiOverlay::UiOverlay(Window *window) {
    viewport_ = new Viewport(0, 0, window->width(), window->height());

    BitmapFont bitmap_font = GLPlay::BitmapFont(const_cast<unsigned char*>(DROID_SANS_MONO), 15.0f);

    text_renderable_ = new Renderable();
    text_renderable_->SetTextureFromBitmap(bitmap_font.bitmap(), bitmap_font.bitmap_width(), bitmap_font.bitmap_height());

    float offset_x = 0.0f;
    float offset_y = 0.0f;
    bitmap_font.GenerateTextMesh(fmt::format("GLPlay"), text_renderable_->vertices_, text_renderable_->indices_, offset_x, offset_y);

    text_renderable_->Bind();

    window->event_source_.RegisterHandler(Window::RESIZE_EVENT, [this](const EventData &event_data) {
        const ResizeEventData & resize_event_data = static_cast<const ResizeEventData&>(event_data);
        this->viewport_->Resize(0, 0, resize_event_data.width_, resize_event_data.height_);
    });
}

void UiOverlay::Render() {
    // TODO: This should exist as a method on the renderable
    *(text_renderable_->model_matrix()) = glm::mat4(1.0f);
    *(text_renderable_->model_matrix()) = glm::rotate(*(text_renderable_->model_matrix()), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    *(text_renderable_->model_matrix()) = glm::translate(*(text_renderable_->model_matrix()), glm::vec3(0.0f, viewport_->height()-15.0f, 0.0f));
    *(text_renderable_->model_matrix()) = glm::scale(*(text_renderable_->model_matrix()), glm::vec3(1.0f, 1.0f, 1.0f));
    text_renderable_->Render(*viewport_->view_matrix(), *viewport_->proj_matrix());
}

}