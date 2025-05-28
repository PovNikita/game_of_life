#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <functional>
#include <string>

class Button {
public:

    explicit Button(SDL_Rect rect, SDL_Color color,
                    std::function<void()> Click, std::string text,
                    SDL_Renderer* renderer);

    ~Button();
    void Render() const;
    void HandleEvent(const SDL_Event& event);
    std::string GetText() const;
    void SetText(std::string text);
private:
    SDL_Rect rect_;
    SDL_Color color_;
    std::function<void()> Click_;

    SDL_Rect text_rect_;
    std::string text_;
    SDL_Color text_color_{100, 100, 100, 255};

    void PositioningText();

    SDL_Renderer* renderer_;
    TTF_Font* font_ = nullptr;
    SDL_Surface* text_surface_ = nullptr;
    SDL_Texture* text_texture_ = nullptr;
};