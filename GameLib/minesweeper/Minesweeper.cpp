/*
** EPITECH PROJECT, 2025
** B-OOP-400-STG-4-1-arcade-paul1.emeriau
** File description:
** Minesweeper
*/

#include "Minesweeper.hpp"

extern "C" game::IGame &getInstance()
{
    static game::Minesweeper instance;

    return instance;
}

namespace game  {
    Minesweeper::Minesweeper() : AGame("Minesweeper"), _isRunning(true)
    {
        _playerPos = std::make_tuple(0, 0);
        _status = true;
        _inGame = false;
        _timeStart = std::chrono::steady_clock::now();
        _timer = 300;
        _closeBomb.resize(MINESWEEPER_WIDTH / 5, std::vector<short>(MINESWEEPER_HEIGHT / 5, 0));
        _revaledBomb.resize(MINESWEEPER_WIDTH / 5, std::vector<bool>(MINESWEEPER_HEIGHT / 5, 0));
    }

    bool Minesweeper::isABomb(int x, int y)
    {
        for (const auto& [bombX, bombY] :_bomb){
            if (x == bombX && bombY == y)
                return true;
        }
        return false;
    }

    bool Minesweeper::isOkBombPosition(int x, int y)
    {
        int x_player = std::get<0>(_playerPos);
        int y_player = std::get<1>(_playerPos);

        return !(std::abs(x - x_player) < 6 && std::abs(y - y_player) < 6);
    }

    void Minesweeper::generateBomb()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(20, 25);
        int x;
        int y;

        _nbBomb = distrib(gen);
        std::uniform_int_distribution<> distrib2(0, (MINESWEEPER_WIDTH / 5) - 1);
        std::uniform_int_distribution<> distrib3(0, (MINESWEEPER_HEIGHT / 5) - 1);
        y = distrib3(gen);

        for (int i = 0; i < _nbBomb; i++){
            x = distrib2(gen);
            y = distrib3(gen);
            if (isABomb(x, y) == true || isOkBombPosition(x * 5, y * 5) == false)
                continue;
            _bomb.push_back(std::make_tuple(x, y));
            _revaledBomb[x][y] = true;
        }
    }

    short Minesweeper::getBombAround(int i, int j)
    {
        short count = 0;

        for (const auto& [x, y] :_bomb){
            if (i == x + 1 && j == y) {count++;}
            if (i == x + 1 && j == y + 1) {count++;}
            if (i == x && j == y + 1) {count++;}
            if (i == x - 1 && j == y + 1) {count++;}
            if (i == x - 1 && j == y) {count++;}
            if (i == x - 1 && j == y - 1) {count++;}
            if (i == x && j == y - 1) {count++;}
            if (i == x + 1 && j == y - 1) {count++;}
        }
        return count;
    }

    void Minesweeper::fillCloseBomb()
    {
        for (int i = 0; i < MINESWEEPER_WIDTH / 5; i ++){
            for (int j = 0; j < MINESWEEPER_HEIGHT / 5; j++){
                _closeBomb[i][j] = getBombAround(i, j);
                _revaledBomb[i][j] = false;
            }
        }
    }

    void Minesweeper::update(DataHolder &data)
    {
        _pix.clear();

        if (data._events.c == 'p') {
            _isPaused = !_isPaused;
            return;
        }
        if (isPaused(data))
            return;

        if (_inGame == false){
            displayMap();
            displayPlayer();
        } else {
            if (_status == true){
                displayInstruction();
                displayMap();
                displayNumber();
                displayFlags();
                displayPlayer();
                if (displayTime() == false || isFinish() == true)
                    _status = false;
                displayScore(data);
            } else {
                displayMap();
                displayBomb();
                drawLost();
                displayScore(data);
            }
        }
        handleEvent(data);
    }

    void Minesweeper::displayFlags()
    {
        display::Position pos;
        display::Color color = display::Color(80, 80, 255);

        for (const auto& [x, y] : _flags){
            pos = display::Position(x + 1, y + 1);
            AGame::drawRectangle(pos, 4, 4, color, true);
        }
    }

    void Minesweeper::displayInstruction()
    {
        display::Position pos = display::Position(MINESWEEPER_WIDTH + 2, 30);
        display::Color color = display::Color(255, 255, 255);
        AGame::drawText(pos, "Press f to put a Flag", color);
        pos = display::Position(MINESWEEPER_WIDTH + 2, 33);
        AGame::drawText(pos, "It will appear in Blue", color);
    }

    bool Minesweeper::isFinish()
    {
       for (std::size_t i = 0; i < _revaledBomb.size(); ++i) {
            for (std::size_t j = 0; j < _revaledBomb[i].size(); ++j) {
                if (_revaledBomb[i][j] == false)
                    return false;
            }
        }
        return true;
    }

    void Minesweeper::displayScore(DataHolder &data)
    {
        display::Position pos = display::Position(MINESWEEPER_WIDTH + 2, 4);
        display::Color color = display::Color(255, 255, 255);
        AGame::drawText(pos, "Score: ", color);

        pos = display::Position(MINESWEEPER_WIDTH + 8, 4);
        std::string score = std::to_string(data._score);
        AGame::drawText(pos, score, color);
    }

    bool Minesweeper::displayTime()
    {
        display::Position pos = display::Position(MINESWEEPER_WIDTH + 2, 2);
        display::Color color = display::Color(255, 255, 255);

        AGame::drawText(pos, "Timer:", color);
        pos = display::Position(MINESWEEPER_WIDTH + 10, 2);
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - _timeStart);

        int timeRemaining = _timer - elapsed.count();
        if (timeRemaining < 0)
            return false;
        int minutes = timeRemaining / 60;
        int seconds = timeRemaining % 60;

        std::string timeStr = (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" +
                            (seconds < 10 ? "0" : "") + std::to_string(seconds);

        AGame::drawText(pos, timeStr, color);
        return true;
    }

    void Minesweeper::addScoreAndReveal(int x, int y, DataHolder &data)
    {
        if (x < 0 || y < 0 || x >= MINESWEEPER_WIDTH / 5 || y > MINESWEEPER_HEIGHT / 5)
            return;
        if (_revaledBomb[x][y] == false){
            data._score += 20;
            _revaledBomb[x][y] = true;
        }
    }

    void Minesweeper::revealBomb(int x, int y, DataHolder &data)
    {
        if (_closeBomb[x][y] == 0){
            addScoreAndReveal(x, y, data);
            addScoreAndReveal(x - 1, y -1, data);
            addScoreAndReveal(x - 1, y, data);
            addScoreAndReveal(x - 1, y + 1, data);
            addScoreAndReveal(x, y - 1, data);
            addScoreAndReveal(x, y, data);
            addScoreAndReveal(x, y + 1, data);
            addScoreAndReveal(x + 1, y - 1, data);
            addScoreAndReveal(x + 1, y, data);
            addScoreAndReveal(x + 1, y + 1, data);
        } else {
            addScoreAndReveal(x, y, data);
        }
    }

    void Minesweeper::handleEvent(DataHolder &data)
    {
        if (data._events.key == display::Key::UP){
            if (std::get<1>(_playerPos) != 0)
                std::get<1>(_playerPos) -= 5;
        }
        if (data._events.key == display::Key::DOWN){
            if (std::get<1>(_playerPos) != MINESWEEPER_HEIGHT - 5)
                std::get<1>(_playerPos) += 5;
        }
        if (data._events.key == display::Key::RIGHT){
            if (std::get<0>(_playerPos) != MINESWEEPER_WIDTH - 5)
                std::get<0>(_playerPos) += 5;
        }
        if (data._events.key == display::Key::LEFT){
            if (std::get<0>(_playerPos) != 0)
                std::get<0>(_playerPos) -= 5;
        }
        if (data._events.key == display::Key::SELECT){
            if (_inGame == false){
                _inGame = true;
                generateBomb();
                fillCloseBomb();
            }
            if (isABomb(std::get<0>(_playerPos) / 5, std::get<1>(_playerPos) / 5) == true){
                _status = false;
            } else {
                revealBomb(std::get<0>(_playerPos) / 5, std::get<1>(_playerPos) / 5, data);
            }
        }
        if (data._events.c == 'f'){
            putOrRemoveFlags();
        }
        if (data._events.key == display::RESTART) {
            reset();
        }
    }

    void Minesweeper::putOrRemoveFlags()
    {
        for (auto it = _flags.begin(); it != _flags.end(); it++) {
            if (std::get<0>(*it) == std::get<0>(_playerPos) && std::get<1>(*it) == std::get<1>(_playerPos)) {
                _flags.erase(it);
                return;
            }
        }
        _flags.push_back(std::make_tuple(std::get<0>(_playerPos), std::get<1>(_playerPos)));
    }


    void Minesweeper::displayPlayer()
    {
        int x = std::get<0>(_playerPos);
        int y = std::get<1>(_playerPos);
        display::Position pos = display::Position(x + 1, y + 1);
        display::Color color = display::Color(255, 255, 80);
        AGame::drawRectangle(pos, 4, 4, color, true);
    }

    void Minesweeper::displayNumber()
    {
        display::Position pos;
        display::Color color = display::Color(255, 255, 255);

        for (int i = 0; i < MINESWEEPER_WIDTH / 5; i ++) {
            for (int j = 0; j < MINESWEEPER_HEIGHT / 5; j ++) {
                pos = display::Position(i * 5 + 2, j * 5 + 2);
                if (_revaledBomb[i][j] == true)
                    AGame::drawText(pos, std::to_string(_closeBomb[i][j]), color);
            }
        }

    }

    void Minesweeper::displayBomb()
    {
        display::Position pos;
        display::Color color = display::Color(250, 0, 0);

        for (const auto& [x, y] : _bomb){
            pos = display::Position(x * 5 + 1, y * 5 + 1);
            AGame::drawRectangle(pos, 4, 4, color, true);
        }
    }

    void Minesweeper::displayMap()
    {
        display::Position pos = display::Position(0, 0);
        display::Color color = display::Color(255, 255, 255);
        AGame::drawRectangle(pos, MINESWEEPER_WIDTH, MINESWEEPER_HEIGHT, color, false);

        for (int i = 0; i < MINESWEEPER_HEIGHT; i+= 5){
            for (int j = 0; j < MINESWEEPER_WIDTH; j+= 5){
                color = display::Color(255, 255, 255);
                pos = display::Position(j, i);
                AGame::drawRectangle(pos, 5, 5, color, false);
            }
        }
    }

    void Minesweeper::drawLost()
    {
        display::Position pos = display::Position(MINESWEEPER_WIDTH + 4, 20);
        display::Color color = display::Color(255, 255, 255);

        drawText(pos, "You lost!", color);
        pos = display::Position(MINESWEEPER_WIDTH + 2, 21);
        drawText(pos, "Shame on you", color);
    }

    const std::vector<display::Pixel> &Minesweeper::getPixels() const
    {
        return _pix;
    }

    void Minesweeper::reset()
    {
        _closeBomb.clear();
        _revaledBomb.clear();
        _bomb.clear();
        _flags.clear();

        _playerPos = std::make_tuple(0, 0);
        _status = true;
        _inGame = false;
        _timeStart = std::chrono::steady_clock::now();
        _timer = 300;
        _closeBomb.resize(MINESWEEPER_WIDTH / 5, std::vector<short>(MINESWEEPER_HEIGHT / 5, 0));
        _revaledBomb.resize(MINESWEEPER_WIDTH / 5, std::vector<bool>(MINESWEEPER_HEIGHT / 5, 0));
    }
}
