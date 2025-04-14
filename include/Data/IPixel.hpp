/*
** EPITECH PROJECT, 2025
** B-OOP-400-STG-4-1-arcade-paul1.emeriau
** File description:
** IPixel
*/

#ifndef IPIXEL_HPP_
    #define IPIXEL_HPP_
    #include "IColor.hpp"
    #include "IPosition.hpp"

namespace display {
    struct Pixel {
        Position pos;
        Color color;
        char c;
        Pixel(const Position &pos, const Color &color, char c)
            : pos(pos), color(color), c(c){}
    };
}

#endif /* !IPIXEL_HPP_ */
