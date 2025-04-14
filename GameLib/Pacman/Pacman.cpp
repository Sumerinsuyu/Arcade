/*
** EPITECH PROJECT, 2025
** B-OOP-400-STG-4-1-arcade-paul1.emeriau
** File description:
** Pacman
*/

#include "Pacman.hpp"
#include <chrono>
#include <thread>

extern "C" game::IGame &getInstance()
{
    static game::Pacman instance;

    return instance;
}

namespace game {

    Pacman::Pacman()
    : AGame("Pacman"),
      _isRunning(true),
      _refreshRate(0.1f),
      _score(0),
      _highScore(0),
      _gameOver(false),
      _powerMode(false),
      _powerModeTimer(0),
      _paused(false),
      _nextDirection(display::NONE),
      _lastUpdateTime(std::chrono::high_resolution_clock::now())
    {
        initMap();
        _player = {display::Position(14, 23), display::LEFT, display::Color(255, 255, 0), 'C'};
        _ghosts.push_back({display::Position(14, 11), display::UP, display::Color(255, 0, 0), 'M'});
        _ghosts.push_back({display::Position(13, 11), display::LEFT, display::Color(255, 192, 203), 'M'});
        _ghosts.push_back({display::Position(14, 11), display::DOWN, display::Color(0, 255, 255), 'M'});
        _ghosts.push_back({display::Position(15, 11), display::RIGHT, display::Color(255, 165, 0), 'M'});
    }

    void Pacman::initMap()
    {
        _map = {
            "############################",
            "#O........................O#",
            "#.####.#####.##.#####.####.#",
            "#.####.#####.##.#####.####.#",
            "#.####.#####.##.#####.####.#",
            "#..........................#",
            "#.####.##.########.##.####.#",
            "#.####.##.########.##.####.#",
            "#......##....##....##.....O#",
            "######.##### ## #####.######",
            "     #.##### ## #####.#     ",
            "     #.##          ##.#     ",
            "     #.## ######## ##.#     ",
            "######.## #      # ##.######",
            "      .   #      #   .      ",
            "######.## #      # ##.######",
            "     #.## ######## ##.#     ",
            "     #.##          ##.#     ",
            "     #.## ######## ##.#     ",
            "######.## ######## ##.######",
            "#O...........##............#",
            "#.####.#####.##.#####.####.#",
            "#.####.#####.##.#####.####.#",
            "#...##.......  .......##...#",
            "###.##.##.########.##.##.###",
            "###.##.##.########.##.##.###",
            "#......##....##....##......#",
            "#.##########.##.##########.#",
            "#.##########.##.##########.#",
            "#O........................O#",
            "############################"
        };
        _dots.clear();
        _powerPellets.clear();
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                if (_map[y][x] == '.') {
                    _dots.push_back(display::Position(x, y));
                } else if (_map[y][x] == 'O') {
                    _powerPellets.push_back(display::Position(x, y));
                }
            }
        }
    }

    /**
     * main function to update the pixel vector
     * @param DataHolder &data
     */
    void Pacman::update(DataHolder &data)
    {
        _pix.clear();

        if (data._events.c == 'p') {
            _isPaused = !_isPaused;
            return;
        }
        if (isPaused(data))
            return;

        handleInput(data._events);
        if (!_paused && !_gameOver) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime - _lastUpdateTime).count();
            if (deltaTime >= _refreshRate) {
                movePlayer(data);
                moveGhosts();
                checkCollisions();
                if (_powerMode) {
                    _powerModeTimer--;
                    if (_powerModeTimer <= 0) {
                        _powerMode = false;
                        for (auto &ghost : _ghosts) {
                            ghost.color = display::Color(255, 0, 0);
                        }
                    }
                }
                _lastUpdateTime = currentTime;
            }
        }
        drawMap();
        drawEntities();
        drawScore();
        if (_gameOver) {
            drawGameOver();
        }
    }

    const std::vector<display::Pixel> &Pacman::getPixels() const
    {
        return _pix;
    }

    void Pacman::handleInput(const display::event &event)
    {
        if (event.key == display::UP) {
            _nextDirection = display::UP;
        } else if (event.key == display::DOWN) {
            _nextDirection = display::DOWN;
        } else if (event.key == display::LEFT) {
            _nextDirection = display::LEFT;
        } else if (event.key == display::RIGHT) {
            _nextDirection = display::RIGHT;
        } else if (event.key == display::RESTART) {
            resetGame();
        }
    }

    /**
     * main function to move the player
     * @param DataHolder &data
     */
    void Pacman::movePlayer(DataHolder &data)
    {
        if (_nextDirection != display::NONE) {
            if (isValidMove(_player.pos, _nextDirection)) {
                _player.dir = _nextDirection;
            }
            _nextDirection = display::NONE;
        }
        if (_player.dir != display::NONE && isValidMove(_player.pos, _player.dir)) {
            switch (_player.dir) {
                case display::UP:
                    _player.pos.y--;
                    break;
                case display::DOWN:
                    _player.pos.y++;
                    break;
                case display::LEFT:
                    _player.pos.x--;
                    if (_player.pos.x < 0) _player.pos.x = MAP_WIDTH - 1;
                    break;
                case display::RIGHT:
                    _player.pos.x++;
                    if (_player.pos.x >= MAP_WIDTH) _player.pos.x = 0;
                    break;
                default:
                    break;
            }
        }
        for (auto it = _dots.begin(); it != _dots.end();) {
            if (it->x == _player.pos.x && it->y == _player.pos.y) {
                _score += 10;
                data._score += 10;
                it = _dots.erase(it);
            } else {
                it++;
            }
        }
        for (auto it = _powerPellets.begin(); it != _powerPellets.end();) {
            if (it->x == _player.pos.x && it->y == _player.pos.y) {
                _score += 50;
                data._score += 50;
                _powerMode = true;
                _powerModeTimer = 50;
                for (auto &ghost : _ghosts) {
                    ghost.color = display::Color(0, 0, 255);
                }
                it = _powerPellets.erase(it);
            } else {
                it++;
            }
        }
        if (_dots.empty() && _powerPellets.empty()) {
            _gameOver = true;
            if (_score > _highScore) {
                _highScore = _score;
            }
        }
    }

    /**
     * main function to move the ghost
     */
    void Pacman::moveGhosts()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, 3);

        for (auto &ghost : _ghosts) {
            if (dist(gen) == 0) {
                display::Key newDir;
                switch (dist(gen)) {
                    case 0: newDir = display::UP; break;
                    case 1: newDir = display::DOWN; break;
                    case 2: newDir = display::LEFT; break;
                    case 3: newDir = display::RIGHT; break;
                    default: newDir = display::RIGHT; break;
                }
                if (isValidMove(ghost.pos, newDir)) {
                    ghost.dir = newDir;
                }
            }
            if (isValidMove(ghost.pos, ghost.dir)) {
                switch (ghost.dir) {
                    case display::UP:
                        ghost.pos.y--;
                        break;
                    case display::DOWN:
                        ghost.pos.y++;
                        break;
                    case display::LEFT:
                        ghost.pos.x--;
                        if (ghost.pos.x < 0) ghost.pos.x = MAP_WIDTH - 1;
                        break;
                    case display::RIGHT:
                        ghost.pos.x++;
                        if (ghost.pos.x >= MAP_WIDTH) ghost.pos.x = 0;
                        break;
                    default:
                        break;
                }
            } else {
                display::Key dirs[4] = {display::UP, display::DOWN, display::LEFT, display::RIGHT};
                bool moved = false;
                for (int i = 0; i < 4 && !moved; i++) {
                    display::Key testDir = dirs[dist(gen)];
                    if (isValidMove(ghost.pos, testDir)) {
                        ghost.dir = testDir;
                        switch (ghost.dir) {
                            case display::UP:
                                ghost.pos.y--;
                                break;
                            case display::DOWN:
                                ghost.pos.y++;
                                break;
                            case display::LEFT:
                                ghost.pos.x--;
                                break;
                            case display::RIGHT:
                                ghost.pos.x++;
                                break;
                            default:
                                break;
                        }
                        moved = true;
                    }
                }
            }
        }
    }

    void Pacman::checkCollisions()
    {
        for (auto &ghost : _ghosts) {
            if (_player.pos.x == ghost.pos.x && _player.pos.y == ghost.pos.y) {
                if (_powerMode) {
                    _score += 200;
                    ghost.pos = display::Position(14, 11);
                } else {
                    _gameOver = true;
                    if (_score > _highScore) {
                        _highScore = _score;
                    }
                }
            }
        }
    }

    bool Pacman::isValidMove(const display::Position &pos, display::Key dir)
    {
        int testX = pos.x;
        int testY = pos.y;

        switch (dir) {
            case display::UP:
                testY--;
                break;
            case display::DOWN:
                testY++;
                break;
            case display::LEFT:
                testX--;
                if (testX < 0) testX = MAP_WIDTH - 1;
                break;
            case display::RIGHT:
                testX++;
                if (testX >= MAP_WIDTH) testX = 0;
                break;
            default:
                return false;
        }
        if (testY >= 0 && testY < MAP_HEIGHT && testX >= 0 && testX < MAP_WIDTH) {
            return _map[testY][testX] != '#';
        }
        return false;
    }

    void Pacman::drawMap()
{
    display::Color wallColor(0, 0, 255);
    display::Color dotColor(255, 255, 255);
    display::Color powerColor(255, 255, 255);

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (_map[y][x] == '#') {
                display::Position pos(x, y);
                display::Pixel pixel(pos, wallColor, '#');
                std::vector<display::Pixel> pixels = {pixel};
                drawPixel(pixels);
            }
        }
    }
    for (const auto &dot : _dots) {
        display::Pixel pixel(dot, dotColor, '*');
        std::vector<display::Pixel> pixels = {pixel};
        drawPixel(pixels);
    }
    static bool flashPower = true;
    static int flashCounter = 0;
    flashCounter++;
    if (flashCounter >= 5) {
        flashCounter = 0;
        flashPower = !flashPower;
    }
    if (flashPower) {
        for (const auto &pellet : _powerPellets) {
            display::Pixel pixel(pellet, powerColor, 'O');
            std::vector<display::Pixel> pixels = {pixel};
            drawPixel(pixels);
        }
    }
}

    void Pacman::drawEntities()
    {
        display::Pixel playerPixel(_player.pos, _player.color, _player.character);
        std::vector<display::Pixel> playerPixels = {playerPixel};
        drawPixel(playerPixels);

        for (const auto &ghost : _ghosts) {
            display::Pixel ghostPixel(ghost.pos, ghost.color, ghost.character);
            std::vector<display::Pixel> ghostPixels = {ghostPixel};
            drawPixel(ghostPixels);
        }
    }

    void Pacman::drawScore()
    {
        display::Position scorePos(1, MAP_HEIGHT + 1);
        display::Color textColor(255, 255, 255);
        drawText(scorePos, "Score: " + std::to_string(_score), textColor);
        display::Position hiScorePos(15, MAP_HEIGHT + 1);
        drawText(hiScorePos, "High Score: " + std::to_string(_highScore), textColor);
        display::Position controlPos(1, MAP_HEIGHT + 3);
        drawText(controlPos, "Controls: Arrow keys to move, Enter to pause, M for menu", textColor);
    }

    void Pacman::drawGameOver()
    {
        display::Position gameOverPos(9, 15);
        display::Color textColor(255, 0, 0);
        drawText(gameOverPos, "GAME OVER", textColor);
        display::Position instructPos(5, 17);
        display::Color instrColor(255, 255, 255);
        drawText(instructPos, "Press R to play again", instrColor);
    }

    // void Pacman::drawGamePause()
    // {
    //     display::Position gameOverPos(9, 15);
    //     display::Color textColor(255, 0, 0);
    //     drawText(gameOverPos, "PAUSE", textColor);
    //     display::Position instructPos(5, 17);
    //     display::Color instrColor(255, 255, 255);
    //     drawText(instructPos, "Press P to play again", instrColor);
    // }

    void Pacman::resetGame()
    {
        _gameOver = false;
        _score = 0;
        _powerMode = false;
        _powerModeTimer = 0;
        _player.pos = display::Position(14, 23);
        _player.dir = display::LEFT;
        _ghosts.clear();
        _ghosts.push_back({display::Position(14, 11), display::UP, display::Color(255, 0, 0), 'M'});
        _ghosts.push_back({display::Position(13, 11), display::LEFT, display::Color(255, 192, 203), 'M'});
        _ghosts.push_back({display::Position(14, 11), display::DOWN, display::Color(0, 255, 255), 'M'});
        _ghosts.push_back({display::Position(15, 11), display::RIGHT, display::Color(255, 165, 0), 'M'});
        initMap();
    }

}