/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Mar, 2025                                                     *
 * Title           - ARC                                                                *
 * Description     -                                                                    *
 *     GraphicLibLoader                                                                 *
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

#ifndef INCLUDED_LIBLOADER_HPP
    #define INCLUDED_LIBLOADER_HPP

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

#include "Display/IDisplay.hpp"
#include "Game/IGame.hpp"

#include <dlfcn.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

typedef display::IDisplay &(*func_t)(void);
typedef game::IGame &(*gfunc_t)(void);
typedef std::string (*type_func_t)(void);

class LibLoader
{
    public:
        LibLoader(std::string grLibName="./lib/arcade_ncurses.so",
            std::string gaLibName="./lib/arcade_menu.so");
        ~LibLoader() = default;

        class Error : public std::exception {
            public:
                Error(const std::string &msg);
                const char *what() const noexcept override;

            private:
                std::string _msg;
        };

        display::IDisplay *getGraphicLib();
        game::IGame *getGameLib();

        game::IGame &getGameLibInstance(const char *libPath, void **gameHandle);
        display::IDisplay &getGraphicalLibInstance(const char *libPath, void **graphHandle);

        std::vector<std::string> getGameLibNames() const;
        std::vector<std::string> getGraphLibNames() const;

        void setGameLib(int index);
        void setGraphicLib(int index);
        std::string getLibType(const char *libPath);

        private:
        void refreshNames();

        std::vector<std::string> _gameLibsNames;
        std::vector<std::string> _graphLibsNames;

        display::IDisplay *_graphicLib;
        game::IGame *_gameLib;

        void *_gameHandle;
        void *_graphicHandle;
};

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
