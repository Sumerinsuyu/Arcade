/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Thu, Apr, 2025                                                     *
 * Title           - ARC                                                                *
 * Description     -                                                                    *
 *     Sfml                                                                             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░             *
 *         ▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓             *
 *         █  ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "Sfml.hpp"

/**
 * @brief return a string to be able to know which type of librairie it is.
 * In the case of SFML it is "DISPLAY"
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
    static display::Sfml instance;

    return instance;
}

namespace display {

    Sfml::Sfml()
    : ADisplay("Sfml"),
    _window(sf::VideoMode(SCREEN_WIDTH * SDL_MULTIPLIER_X,
        SCREEN_HEIGHT * SDL_MULTIPLIER_Y), "Arcade | SFML",
        sf::Style::Titlebar | sf::Style::Close),
    _font(),
    _sfmlEvent(),
    _pixBuffer(),
    _sprite(),
    _isInit(false)
    {
        _window.setFramerateLimit(60);
        if (!_font.loadFromFile("assets/OpenSans-Regular.ttf"))
            throw Error("SFML: couldn't load font");

        __eventAssignement.insert({display::UP, sf::Keyboard::Up});
        __eventAssignement.insert({display::UP, sf::Keyboard::Z});
        __eventAssignement.insert({display::DOWN, sf::Keyboard::Down});
        __eventAssignement.insert({display::DOWN, sf::Keyboard::S});
        __eventAssignement.insert({display::LEFT, sf::Keyboard::Left});
        __eventAssignement.insert({display::LEFT, sf::Keyboard::Q});
        __eventAssignement.insert({display::RIGHT, sf::Keyboard::Right});
        __eventAssignement.insert({display::RIGHT, sf::Keyboard::D});
        __eventAssignement.insert({display::LIB, sf::Keyboard::L});
        __eventAssignement.insert({display::MENU, sf::Keyboard::M});
        __eventAssignement.insert({display::RESTART, sf::Keyboard::R});
        __eventAssignement.insert({display::GAME, sf::Keyboard::G});
        __eventAssignement.insert({display::QUIT, sf::Keyboard::Escape});
        __eventAssignement.insert({display::SELECT, sf::Keyboard::Return});

        _isInit = true;
    }

    Sfml::~Sfml()
    {
        if (_isInit) {
            _sprite = sf::Sprite();
            _pixBuffer.create(1, 1);
            _window.close();
            _isInit = false;
        }
    }

    /**
     * @brief take a vector of pixrls that need to be drawn and display them
     * @param std::vector<display::Pixel> &pix
     */
    void Sfml::display(std::vector<Pixel> &pix)
    {
        sf::Texture texture;

        _pixBuffer.create(SCREEN_WIDTH * SDL_MULTIPLIER_X + 100,
            SCREEN_HEIGHT * SDL_MULTIPLIER_Y + 100, sf::Color::Transparent);
        _window.clear();
        for (auto &pixel : pix) {
            if (pixel.c == '#') {
                setPix(pixel);
                continue;
            }
            setText(pixel);
        }
        texture.loadFromImage(_pixBuffer);
        _sprite.setTexture(texture);
        _window.draw(_sprite);
        _window.display();
        catchEvent();
    }

    /**
     * @brief Catch the events in SFML and update the display::event structure
     * to get them afterwards
     */
    void Sfml::catchEvent()
    {
        while (_window.pollEvent(_sfmlEvent)) {
            if (_sfmlEvent.type == sf::Event::Closed) {
                _event = display::event(display::QUIT, ' ');
                return;
            }
            if (_sfmlEvent.type == sf::Event::KeyPressed) {
                _event = display::event(getKey(_sfmlEvent.key.code), _sfmlEvent.key.code + 97);
                return;
            }
        }
        _event = display::event();
    }

    /**
     * @brief Check an sf::Keyboard::Key and assign to a display::event::Key
     * @param sf::Keyboard::Key key
     */
    display::Key Sfml::getKey(sf::Keyboard::Key key)
    {
        for (auto [my_key, value] : __eventAssignement) {
            if (value == key) {
                return my_key;
            }
        }
        return NONE;
    }

    /**
     * @brief use an sf::Image to load all the displayable pixels
     * (set their color and positions) on it and make only one draw using
     * the sf::Image _pixBuffer
     * @param display::Pixel pixel
     */
    void Sfml::setPix(display::Pixel pixel)
    {
        unsigned int x = 0;
        unsigned int y = 0;

        for (int i = 0; i < SDL_MULTIPLIER_X; ++i) {
            for (int j = 0; j < SDL_MULTIPLIER_Y; ++j) {
                x = pixel.pos.x * SDL_MULTIPLIER_X + i;
                y = pixel.pos.y * SDL_MULTIPLIER_Y + j;

                if (x < _pixBuffer.getSize().x && y < _pixBuffer.getSize().y) {
                    _pixBuffer.setPixel(x, y, sf::Color(pixel.color.r, pixel.color.g, pixel.color.b));
                }
            }
        }
    }

    /**
     * @brief display the characters that are not '#' (getted from pix->c) as text
     * @param display::Pixel pix
     */
    void Sfml::setText(display::Pixel pix)
    {
        sf::Text text;

        text.setFont(_font);
        text.setString(std::string(1, pix.c));
        text.setCharacterSize(SDL_MULTIPLIER_Y);
        text.setPosition(pix.pos.x * SDL_MULTIPLIER_X, pix.pos.y * SDL_MULTIPLIER_Y);
        text.setFillColor(sf::Color(pix.color.r, pix.color.g, pix.color.b));
        text.setStyle(sf::Text::Regular);
        _window.draw(text);
    }
}
