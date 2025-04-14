/*
** EPITECH PROJECT, 2025
** B-OOP-400-STG-4-1-arcade-paul1.emeriau
** File description:
** AGame
*/

#include "Game/AGame.hpp"
#include "Display/IDisplay.hpp"

extern "C" std::string getType()
{
    return GAME_TYPE;
}

namespace game {

    /**
     * @brief Take the Pixel Buffer _pix
     * And put a color at position x y
     * @param std::vector<display::Pixel> &_pix
     * @param int x
     * @param int y
     * @param display::Color &color
     * @param char c
    */
    void UpdatePixel(std::vector<display::Pixel> &_pix, int x, int y, display::Color &color, char c) {
        int index = 0;
        for (auto &existingPixel : _pix) {
            index++;
            if (x == existingPixel.pos.x && y == existingPixel.pos.y) {
                existingPixel.color = color;
                existingPixel.c = c;
                return;
            }
        }
        display::Position pos = display::Position(x, y);
        display::Pixel newPix = display::Pixel(pos, color, c);
        _pix.emplace_back(newPix);
    }
    /**
     * @brief Take a Pixel vector in Argument
     * Take the Color of a Pixel
     * And put it in _pix for the same Position
     * @param std::vector<display::Pixel> &pix
    */
    void  AGame::drawPixel(std::vector<display::Pixel> &pix) {
        for (auto &newPixel : pix) {
            UpdatePixel(_pix, newPixel.pos.x, newPixel.pos.y, newPixel.color, newPixel.c);
        }
    }

    /**
     * @brief Updating _pix for a rectangle ( and square)
     * With the top left point at Position pos
     * By size width and height
     * With a precise Color
     * If fill = true the rectangle is fill
     * Else he is empty
    */
    void  AGame::drawRectangle(display::Position &pos, int width, int height, display::Color &color, bool fill) {
        if (fill == true) {
            for (int y = pos.y; y < pos.y + height; y++){
                for (int x = pos.x; x <  pos.x + width; x++){
                    UpdatePixel(_pix, x, y, color, '#');
                }
            }
        } else {
            for (int i = 0; i < width; i++)
                UpdatePixel(_pix, pos.x + i, pos.y, color, '#');
            for (int i = 0; i < height; i++)
                UpdatePixel(_pix, pos.x , pos.y + i, color, '#');
            for (int i = 0; i < width; i++)
                UpdatePixel(_pix, pos.x + i, pos.y + height, color, '#');
            for (int i = 0; i <= height; i++)
                UpdatePixel(_pix, pos.x + width, pos.y + i, color, '#');
        }
    }

    /**
     * @brief Updating _pix for a Circle
     * With Position of the center
     * By size radius
     * With a precise Color
    */
    void AGame::drawCircle(display::Position &pos, int radius, display::Color &color) {
        for (int y = pos.y - radius; y < pos.y + radius; y++) {
            for (int x = pos.x - radius; x < pos.x + radius; x++) {
                if ((x - pos.x) * (x - pos.x) + (y - pos.y) * (y - pos.y) < radius * radius) {
                    UpdatePixel(_pix, x, y, color, '#');
                }
            }
        }
    }

    /**
     * @brief Immadiatly display a Text
     * At pos and a color
    */
    void AGame::drawText(display::Position &pos, std::string text, display::Color &color) {
        for (int i = 0; text[i] != '\0'; i++){
            UpdatePixel(_pix, pos.x + i, pos.y, color, text[i]);
        }
    }

    bool AGame::isPaused(DataHolder &data)
    {
        display::Position pos(0, 0);
        display::Color colorYellow(255, 255, 0);
        display::Color colorGreen(0, 255, 0);
        display::Color colorWhite(255, 255, 255);

        if (!_isPaused)
            return false;
        checkIndex(data);
        drawRectangle(pos, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, colorYellow, false);

        pos.x = (SCREEN_WIDTH / 2) - 4;
        pos.y = SCREEN_HEIGHT / 2;
        if (!_pauseIndex) {
            pos.x -= 3;
            drawText(pos, "-> Continue <-", colorGreen);
        }
        else {
            drawText(pos, "Continue", colorWhite);
        }

        pos.x = (SCREEN_WIDTH / 2) - 2;
        pos.y = SCREEN_HEIGHT / 2 + 10;
        if (_pauseIndex) {
            pos.x -= 3;
            drawText(pos, "-> Menu <-", colorGreen);
        }
        else {
            drawText(pos, "Menu", colorWhite);
        }
        return true;
    }

    void AGame::checkIndex(DataHolder &data)
    {
        if (data._events.key == display::DOWN || data._events.key == display::UP)
            _pauseIndex = !_pauseIndex;

        if (!_pauseIndex && data._events.key == display::SELECT) {
            _isPaused = false;
        }
        if (_pauseIndex && data._events.key == display::SELECT) {
            _isPaused = false;
            data._events.key = display::MENU;
        }
    }
}