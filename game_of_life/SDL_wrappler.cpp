#include "SDL_wrappler.h"
SDLInit::SDLInit()
{
    using namespace std::literals;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error("SDL_Init Error: "s + std::string(SDL_GetError()) + "\n"s);
    }
    if (TTF_Init() != 0) {
        SDL_Quit();
        throw std::runtime_error("TTF_Init Error: "s + TTF_GetError() + "\n"s);
    }
}

SDLInit::~SDLInit()
{
    TTF_Quit();
    SDL_Quit();
}