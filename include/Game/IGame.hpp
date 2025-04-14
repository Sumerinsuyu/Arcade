/*
** EPITECH PROJECT, 2025
** B-OOP-400-STG-4-1-arcade-paul1.emeriau
** File description:
** IGame
*/

#ifndef IGAME_HPP_
    #define IGAME_HPP_

    #include <string>
    #include <vector>

    #include "Display/IDisplay.hpp"
    #include "Data/IPixel.hpp"
    #include "Data/DataTransfer.hpp"

    #define GAME_TYPE   "GAME"

    namespace game {

        class IGame {
            public :
                virtual ~IGame() = default;
                virtual const std::vector<display::Pixel> &getPixels() const = 0;
                virtual void update(DataHolder &data) = 0;
        };
    }

#endif /* !IGame_HPP_ */
