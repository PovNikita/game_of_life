#include "user_interface.h"
#include <stdexcept>
#include <filesystem>

Button::Button(SDL_Rect rect, SDL_Color color, 
                        std::function<void()> Click, std::string text,
                        SDL_Renderer* renderer)
    : rect_(rect)
    , color_(color)
    , Click_(Click)
    , text_(std::move(text))
    , renderer_(renderer)
{
    using namespace std::literals;
    using namespace std::filesystem;
    path current_path = std::filesystem::current_path() / path("arial.ttf");
    font_ = TTF_OpenFont(current_path.string().data(), 20);
    if (!font_) {
        throw std::runtime_error("TTF_OpenFont Error: "s + TTF_GetError());
    }
    text_surface_ = TTF_RenderText_Blended(font_, text_.data(), text_color_);
    text_texture_ = SDL_CreateTextureFromSurface(renderer_, text_surface_);
    PositioningText();

}

Button::~Button()
{
    SDL_DestroyTexture(text_texture_);
    SDL_FreeSurface(text_surface_);
    TTF_CloseFont(font_);
}

void Button::Render() const {
    SDL_SetRenderDrawColor(renderer_, color_.r, color_.g, color_.b, color_.a);
    SDL_RenderFillRect(renderer_, &rect_);
    SDL_RenderCopy(renderer_, text_texture_, NULL, &text_rect_);
}

void Button::HandleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int x = event.button.x;
        int y = event.button.y;

        if (x >= rect_.x && x <= rect_.x + rect_.w &&
            y >= rect_.y && y <= rect_.y + rect_.h) {
            if (Click_) {
                Click_();
            }
        }
    }
}

std::string Button::GetText() const {
    return text_;
}
void Button::SetText(std::string text) {
    text_ = std::move(text);
    if (text_texture_) {
        SDL_DestroyTexture(text_texture_);
        text_texture_ = nullptr;
    }
    if (text_surface_) {
        SDL_FreeSurface(text_surface_);
        text_surface_ = nullptr;
    }

    text_surface_ = TTF_RenderText_Blended(font_, text_.data(), text_color_);
    if (!text_surface_) {
        throw std::runtime_error("TTF_RenderText_Blended failed: " + std::string(TTF_GetError()));
    }
    text_texture_ = SDL_CreateTextureFromSurface(renderer_, text_surface_);
    if (!text_texture_) {
        SDL_FreeSurface(text_surface_);
        text_surface_ = nullptr;
        throw std::runtime_error("SDL_CreateTextureFromSurface failed: " + std::string(SDL_GetError()));
    }
    PositioningText();
}

void Button::PositioningText() {
    int text_w = 0, text_h = 0;
    SDL_QueryTexture(text_texture_, NULL, NULL, &text_w, &text_h);
    text_rect_.w = text_w;
    text_rect_.h = text_h;
    text_rect_.x = rect_.x + (rect_.w - text_w) / 2;
    text_rect_.y = rect_.y + (rect_.h - text_h) / 2;
}
