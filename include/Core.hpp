/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sat, Mar, 2025                                                     *
 * Title           - ARC                                                                *
 * Description     -                                                                    *
 *     Core                                                                             *
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

#ifndef INCLUDED_CORE_HPP
    #define INCLUDED_CORE_HPP

#include <iostream>
#include <vector>
#include <map>
#include <functional>

#include "Display/IDisplay.hpp"
#include "LibLoader.hpp"

    #define USER_FILE   "users/users.arc"

namespace core {

    class Core
    {
        public:

            class Error : public std::exception {
                public:
                    Error(const std::string &msg);
                    const char *what() const noexcept override;

                    private:
                    std::string _msg;
            };

            ~Core() = default;
            static Core &getInstance(LibLoader &libLoader);
            void run();

        private:

            Core(LibLoader &libLoader);

            void setGameIndex();
            void setGraphicIndex();

            void checkEvent(DataHolder &data, game::IGame **games,
                display::IDisplay **render);

            void eventQuit(DataHolder &data, game::IGame **games);
            void eventMenu(DataHolder &data, game::IGame **games);
            void eventLib(DataHolder &data, game::IGame **games);
            void eventGame(DataHolder &data, game::IGame **games);

            void createUserData();
            void getUserData(DataHolder &data);
            void setUserData(DataHolder &data);

            std::string _userName;
            int _score;
            bool _isRunning;
            LibLoader _libLoader;
            int _gameIndex;
            int _graphicIndex;
            std::map<enum display::Key, std::function<void(DataHolder &data, game::IGame **games)>> _eventsFcts;
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
