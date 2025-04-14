/*
** EPITECH PROJECT, 2025
** B-OOP-400-STG-4-1-arcade-paul1.emeriau
** File description:
** IColor
*/

#ifndef ICOLOR_HPP_
    #define ICOLOR_HPP_
    #include <cstdint>

namespace display {
    struct Color {
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
        Color(std::uint8_t r = 0, std::uint8_t g = 0, std::uint8_t b = 0)
            : r(r), g(g), b(b) {}
    };
}
#endif /* !ICOLOR_HPP_ */
