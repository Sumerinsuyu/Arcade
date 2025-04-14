/*
** EPITECH PROJECT, 2025
** B-OOP-400-STG-4-1-arcade-paul1.emeriau
** File description:
** Snake
*/

#ifndef SNAKE_HPP_
    #define SNAKE_HPP_

#include <iostream>
#include <deque>
#include <random>
#include <thread>
#include <chrono>
#include <vector>

#include "Game/AGame.hpp"

#define SNAKE_MAP_WIDTH     SCREEN_WIDTH / 3 * 2
#define SNAKE_MAP_HEIGHT    SCREEN_HEIGHT - 2

namespace game {
    class Snake : public AGame
    {
        public:
            Snake();
            ~Snake() = default;

            const std::vector<display::Pixel> &getPixels() const override;
            void update(DataHolder &data) override;

        private:
            void displayMap();
            void drawPlayer();
            void drawApple();
            void handle_event(DataHolder &data);
            bool isAppleEated(DataHolder &data);
            void regenNewApple();
            std::tuple<int, int> getNewPixel();
            void move(DataHolder &data);
            void isLost();
            void drawLost(DataHolder &data);
            void drawPlayerData(DataHolder &data);

            void reset();

            bool _isRunning;
            std::deque<std::tuple<int, int>> _player;
            std::deque<std::tuple<int, int>> _apple;
            bool _status; //true if alive
            display::Key _lastEvent;
    };
}

#endif /* !SNAKE_HPP_ */