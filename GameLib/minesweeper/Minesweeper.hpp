/*
** EPITECH PROJECT, 2025
** B-OOP-400-STG-4-1-arcade-paul1.emeriau
** File description:
** Minesweeper
*/

#ifndef MINESWEEPER_HPP_
#define MINESWEEPER_HPP_

#include <deque>
#include <random>
#include <thread>
#include <string>
#include <chrono>

#include "Game/AGame.hpp"

#define MINESWEEPER_WIDTH  70
#define MINESWEEPER_HEIGHT 40

namespace game {
    class Minesweeper : public AGame
    {
        public:
            Minesweeper();
            ~Minesweeper() = default;

            void update(DataHolder &data) override;
            const std::vector<display::Pixel> &getPixels() const override;

        private:
            void displayInstruction();
            void generateBomb();
            void displayMap();
            void displayBomb();
            void fillCloseBomb();
            short getBombAround(int i, int j);
            void displayNumber();
            bool isABomb(int x, int y);
            void displayPlayer();
            void handleEvent(DataHolder &data);
            void drawLost();
            bool isOkBombPosition(int x, int y);
            void revealBomb(int x, int y, DataHolder &data);
            bool displayTime();
            void displayScore(DataHolder &data);
            void addScoreAndReveal(int x, int y, DataHolder &data);
            bool isFinish();
            void displayFlags();
            void putOrRemoveFlags();
            void reset();

            std::chrono::time_point<std::chrono::steady_clock> _timeStart;
            int _timer;
            bool _isRunning;
            bool _inGame;
            bool _status;
            int _nbBomb;
            std::tuple<int, int> _playerPos;
            std::deque<std::tuple<int, int>> _bomb;
            std::vector<std::vector<short>> _closeBomb;
            std::vector<std::vector<bool>> _revaledBomb;
            std::deque<std::tuple<int, int>> _flags;
    };
}
#endif /* !MINESWEEPER_HPP_ */
