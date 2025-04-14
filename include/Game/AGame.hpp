/*
** EPITECH PROJECT, 2025
** B-OOP-400-STG-4-1-arcade-paul1.emeriau
** File description:
** AGame
*/

#ifndef AGAME_HPP_
    #define AGAME_HPP_

    #include "Game/IGame.hpp"

namespace game {

    class AGame : public IGame {
        public:
            AGame(std::string name)
            : _name(name), _pix(), _pauseIndex(0) {};
            ~AGame() = default;

            const std::string &getName() const { return _name; };
            void drawPixel(std::vector<display::Pixel> &pix);
            void drawRectangle(display::Position &pos, int width, int height, display::Color &color, bool fill);
            void drawCircle(display::Position &pos, int radius, display::Color &color);
            void drawText(display::Position &pos, std::string text, display::Color &color);
            const std::vector<display::Pixel> &getPixels() const override
            {return _pix;};
            bool isPaused(DataHolder &data);

        protected:
            const std::string _name;
            std::vector<display::Pixel> _pix;
            bool _isPaused;

        private:
            bool _pauseIndex;

            void checkIndex(DataHolder &data);
    };
}

#endif /* !AGAME_HPP_ */
