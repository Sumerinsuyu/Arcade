/*
** EPITECH PROJECT, 2025
** B-OOP-400-STG-4-1-arcade-paul1.emeriau
** File description:
** IPosition
*/

#ifndef IPOSITION_HPP_
#define IPOSITION_HPP_

namespace display {
    struct Position {
        int x;
        int y;
        Position(int x = 0, int y = 0)
            : x(x), y(y) {}
    };
};

#endif /* !IPOSITION_HPP_ */
