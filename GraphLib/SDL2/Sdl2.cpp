/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Thu, Mar, 2025                                                     *
 * Title           - ARC                                                                *
 * Description     -                                                                    *
 *     Sdl2                                                                             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _|           *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _|           *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_|           *
 *       _|        _|          _|        _|      _|        _|        _|    _|           *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _|           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "Sdl2.hpp"

/**
 * @brief return a string to be able to know which type of librairie it is.
 * In the case of SDL2 it is "DISPLAY"
 */
extern "C" std::string getType()
{
    return DISPLAY_TYPE;
}

/**
 * @brief return an instance of display::SFML
 */
extern "C" display::IDisplay &getInstance()
{
    static display::Sdl2 instance;

    return instance;
}

namespace display {

    Sdl2::Sdl2()
    : ADisplay("SDL2"),
    _window(nullptr),
    _renderer(nullptr),
    _isInitialized(false),
    _sdlEvent()
    {
        _eventAssignement = {
            {SDLK_UP,       display::UP},
            {SDLK_z,        display::UP},
            {SDLK_DOWN,     display::DOWN},
            {SDLK_s,        display::DOWN},
            {SDLK_q,        display::LEFT},
            {SDLK_LEFT,     display::LEFT},
            {SDLK_d,        display::RIGHT},
            {SDLK_RIGHT,    display::RIGHT},
            {SDLK_l,        display::LIB},
            {SDLK_m,        display::MENU},
            {SDLK_r,        display::RESTART},
            {SDLK_g,        display::GAME},
            {SDLK_ESCAPE,   display::QUIT},
            {SDLK_RETURN,   display::SELECT}
        };
        setenv("SDL_VIDEODRIVER", "x11", 1);
        setenv("GTK_THEME", "default", 1);

        if (_isInitialized)
            return;

        _window = SDL_CreateWindow("Arcade | SDL2",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            SCREEN_WIDTH * SDL_MULTIPLIER_X, SCREEN_HEIGHT * SDL_MULTIPLIER_Y,
            SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

        _renderer = SDL_CreateRenderer(_window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        TTF_Init();
        _font = TTF_OpenFont("assets/OpenSans-Regular.ttf", SDL_MULTIPLIER_Y);
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
        SDL_RenderClear(_renderer);
        SDL_RenderPresent(_renderer);
        _isInitialized = true;
    }

    Sdl2::~Sdl2()
    {
        if (_isInitialized) {
            if (_renderer) {
                SDL_DestroyRenderer(_renderer);
                _renderer = nullptr;
            }
            if (_window) {
                SDL_DestroyWindow(_window);
                _window = nullptr;
            }
            TTF_Quit();
            SDL_Quit();
            _isInitialized = false;
        }
    }

    /**
     * Get all the events and fill the DataHolder struct with them
     * @param std::unordered_map<int:Pixel> map with all the pixels
     */
    void Sdl2::display(std::vector<Pixel> &pix)
    {
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
        SDL_RenderClear(_renderer);
        drawPixel(pix);
        SDL_RenderPresent(_renderer);
        catchEvent();
    }

    /**
     * Draw all the pixels in the map of pixels one by one
     * @param std::unordered_map<int:Pixel> map of pixels to be drawn
     */
    void Sdl2::drawPixel(std::vector<Pixel> &pix)
    {
        static SDL_Rect rect;

        for (auto &pixel : pix) {
            if (pixel.c != '#' && std::isprint(pixel.c)) {
                drawText(pixel);
                continue;
            }
            rect.x = pixel.pos.x * SDL_MULTIPLIER_X;
            rect.y = pixel.pos.y * SDL_MULTIPLIER_Y;
            rect.w = SDL_MULTIPLIER_X;
            rect.h = SDL_MULTIPLIER_Y;

            SDL_SetRenderDrawColor(
                _renderer,
                pixel.color.r,
                pixel.color.g,
                pixel.color.b,
                255
            );
            SDL_RenderFillRect(_renderer, &rect);
        }
    }

    /**
     * Draw a Pixel containing the text
     * @param display::Pixel pixel Pixel struct containing the character
     */
    void Sdl2::drawText(display::Pixel pixel)
    {
        if (!_font) {
            return;
        }
        SDL_Color color = {pixel.color.r, pixel.color.g, pixel.color.b, 255};
        std::string text(1, pixel.c);
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(_font, text.c_str(), color);
        if (!surfaceMessage) {
            return;
        }
        SDL_Texture* message = SDL_CreateTextureFromSurface(_renderer, surfaceMessage);
        if (!message) {
            SDL_FreeSurface(surfaceMessage);
            return;
        }

        SDL_Rect messageRect;
        messageRect.x = pixel.pos.x * SDL_MULTIPLIER_X;
        messageRect.y = pixel.pos.y * SDL_MULTIPLIER_Y;
        messageRect.w = surfaceMessage->w;
        messageRect.h = surfaceMessage->h;

        SDL_RenderCopy(_renderer, message, NULL, &messageRect);

        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(message);
    }

    /**
     * Get the key code depending of the user key strokes
     * @param signed-int code
     */
    enum display::Key Sdl2::getKey(signed int code)
    {
        for (auto [key, value] : _eventAssignement) {
            if (key == code) {
                return value;
            }
        }
        return display::NONE;
    }

    /**
     * Get all the events from the user (only key strokes for now)
     */
    void Sdl2::catchEvent()
    {
        _sdlEvent = {0};
        while (SDL_PollEvent(&_sdlEvent)) {
            if (_sdlEvent.type == SDL_QUIT) {
                _event = display::event(display::QUIT, ' ');
                return;
            }
            if (_sdlEvent.type == SDL_KEYDOWN) {
                _event = display::event(getKey(_sdlEvent.key.keysym.sym), _sdlEvent.key.keysym.sym);
                return;
            }
        }
        _event = display::event();
    }
}
