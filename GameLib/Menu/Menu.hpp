/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Fri, Mar, 2025                                                     *
 * Title           - ARC                                                                *
 * Description     -                                                                    *
 *     Menu                                                                             *
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

#ifndef INCLUDED_MENU_HPP
    #define INCLUDED_MENU_HPP

#include <iostream>
#include <map>
#include <vector>

#include "Game/AGame.hpp"

namespace game {

    class Menu : public AGame
    {

        enum sections {
            PLAYER,
            GAMES,
            GRAPHICAL,
            TOTAL
        };

        enum game_selection {
            SNAKE,
            MINESWEEPER,
            PACMAN,
            GAME_TOTAL
        };

        enum menu_mode {
            TEXT,
            SELECTION
        };

        public:
            Menu();
            ~Menu() = default;

            void update(DataHolder &data) override;
            const std::vector<display::Pixel> &getPixels() const override;

        private:
            void setSection(DataHolder &data);
            void setSelectedItem(display::event &events, int size,
                enum sections selectedSec, int &index);

            void drawBase();
            void drawData(display::event &events, std::string &username, int score);
            void drawLibs(std::vector<std::string> games, int leftCol,
                std::string helper, int index, enum sections section);
            void drawAsciiArt();

            std::string trimGame(std::string gameName);

            void changeSelectedItem(DataHolder &data);

            enum sections _section;
            enum menu_mode _mode;
            int _gameIndex;
            int _graphicIndex;
            std::vector<std::string> _instruction;
    };
}

#endif

/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * MIT License                                                                          *
 * Copyright (c) 2025                                                                   *
 *                                                                                      *
 * Permission is hereby granted, free of charge, to any person obtaining a copy         *
 * of this software and associated documentation files (the "Software"), to deal        *
 * in the Software without restriction, including without limitation the rights         *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell            *
 * copies of the Software, and to permit persons to whom the Software is                *
 * furnished to do so, subject to the following conditions:                             *
 *                                                                                      *
 * The above copyright notice and this permission notice shall be included in all       *
 * copies or substantial portions of the Software.                                      *
 *                                                                                      *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR           *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,             *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE          *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER               *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,        *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE        *
 * SOFTWARE.                                                                            *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */
