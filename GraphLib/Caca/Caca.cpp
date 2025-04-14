/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Thu, Apr, 2025                                                     *
 * Title           - ARC                                                                *
 * Description     -                                                                    *
 *     Caca                                                                             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄             *
 *      ▐  ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀            *
 *        █▄▄▄▄▄  ▐  █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             *
 *        █    ▌     █          █        █        █    ▌    █         █   █             *
 *       ▄▀▄▄▄▄    ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             *
 *       █    ▐   █         █       █ █          █    ▐   █     ▐   █   █               *
 *       ▐        ▐         ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "Caca.hpp"

extern "C" std::string getType()
{
    return DISPLAY_TYPE;
}

extern "C" display::IDisplay &getInstance()
{
    static display::Caca instance;

    return instance;
}

namespace display {

    Caca::Caca()
    : ADisplay("Caca"),
    _canvas(caca_create_canvas(SCREEN_WIDTH, SCREEN_HEIGHT)),
    _display(caca_create_display(_canvas))
    {
        if (!_canvas) {
            throw Error("Caca: error creating canva");
        }

        if (!_display) {
            caca_free_canvas(_canvas);
            throw Error("Failed to create libcaca display");
        }

        caca_set_display_title(_display, "Arcade | Caca");

        _eventAssignment = {
            {CACA_KEY_UP, display::UP},
            {'z', display::UP},
            {CACA_KEY_DOWN, display::DOWN},
            {'s', display::DOWN},
            {CACA_KEY_LEFT, display::LEFT},
            {'q', display::LEFT},
            {CACA_KEY_RIGHT, display::RIGHT},
            {'d', display::RIGHT},
            {CACA_KEY_ESCAPE, display::QUIT},
            {CACA_KEY_RETURN, display::SELECT},
            {'l', display::LIB},
            {'m', display::MENU},
            {'g', display::GAME},
            {'r', display::RESTART},
        };

    }

    Caca::~Caca()
    {
        if (_display) {
            caca_free_display(_display);
        }
        if (_canvas) {
            caca_free_canvas(_canvas);
        }
    }

    /**
     * take an rgb value as parameter and transform it to make it work with
     * lib caca's color (uint16_t)
     * @param display::Color
     */
    uint16_t Caca::setColor(display::Color color)
    {
        if (color.r == 0 && color.g == 0 && color.b == 0)
            return 0x0000;

        uint16_t r4 = (color.r * 15) / 255;
        uint16_t g4 = (color.g * 15) / 255;
        uint16_t b4 = (color.b * 15) / 255;
        uint16_t argb = (0xF << 12) | (r4 << 8) | (g4 << 4) | b4;

        return argb;
    }

    /**
     * main loop to draw all the updated pixels
     * @param std::vector<Pixel> &pixel
     */
    void Caca::display(std::vector<Pixel> &pixel)
    {
        uint16_t argb = 0;

        caca_set_color_argb(_canvas, CACA_BLACK, CACA_BLACK);
        caca_clear_canvas(_canvas);
        for (const auto &pix : pixel) {
            argb = setColor(pix.color);
            if (pix.c == '#') {
                caca_set_color_argb(_canvas, argb, argb);
                caca_put_char(_canvas, pix.pos.x, pix.pos.y, ' ');
            }
            else {
                caca_set_color_argb(_canvas, argb, CACA_BLACK);
                caca_put_char(_canvas, pix.pos.x, pix.pos.y, pix.c);
            }
        }
        caca_refresh_display(_display);

        catchEvent();
    }

    /**
     * catch the libcaca's events and set the _event variable from ADisplay
     */
    void Caca::catchEvent()
    {
        _event = display::event(NONE, ' ');
        while (caca_get_event(_display, CACA_EVENT_KEY_PRESS | CACA_EVENT_QUIT, &_cacaEvent, 0)) {
            if (caca_get_event_type(&_cacaEvent) & CACA_EVENT_QUIT) {
                _event = display::event(QUIT, ' ');
            }
            if (caca_get_event_type(&_cacaEvent) & CACA_EVENT_KEY_PRESS) {
            uint32_t key = caca_get_event_key_ch(&_cacaEvent);
                _event = display::event(getKey(key), key);
            }
        }
    }

    /**
     * check the character and return a display::Key value if match otherwise
     * NONE
     * @param key integer of the last pressed key
     */
    display::Key Caca::getKey(int key)
    {
        for (auto [c, v] : _eventAssignment) {
            if (c == key) {
                return v;
            }
        }
        return NONE;
    }
}
