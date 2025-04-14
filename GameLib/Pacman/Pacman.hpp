/*
** EPITECH PROJECT, 2025
** B-OOP-400-STG-4-1-arcade-paul1.emeriau
** File description:
** Pacman
*/

#ifndef PACMAN_HPP_
    #define PACMAN_HPP_
    #include <iostream>
    #include <vector>
    #include <random>
    #include <chrono>
    #include "Game/AGame.hpp"

namespace game {

    struct Entity {
        display::Position pos;
        display::Key dir;
        display::Color color;
        char character;
    };

    class Pacman : public AGame
    {
        public:
            Pacman();
            ~Pacman() = default;

            void update(DataHolder &data) override;
            const std::vector<display::Pixel> &getPixels() const override;

        private:
            bool _isRunning;
            float _refreshRate;
            int _score;
            int _highScore;
            bool _gameOver;

            static const int MAP_WIDTH = 28;
            static const int MAP_HEIGHT = 31;
            std::vector<std::string> _map;

            Entity _player;
            std::vector<Entity> _ghosts;
            std::vector<display::Position> _dots;
            std::vector<display::Position> _powerPellets;

            bool _powerMode;
            int _powerModeTimer;
            bool _paused;
            display::Key _nextDirection;
            std::chrono::time_point<std::chrono::high_resolution_clock> _lastUpdateTime;

            void initMap();
            void drawMap();
            void drawEntities();
            void drawScore();
            void drawGameOver();
            // void drawGamePause();

            void handleInput(const display::event &event);
            void movePlayer(DataHolder &data);
            void moveGhosts();
            void checkCollisions();
            bool isValidMove(const display::Position &pos, display::Key dir);
            void resetGame();
    };
}

#endif /* !PACMAN_HPP_ */