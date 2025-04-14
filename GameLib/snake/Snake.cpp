/*
** EPITECH PROJECT, 2025
** B-OOP-400-STG-4-1-arcade-paul1.emeriau
** File description:
** Snake
*/

#include "Snake.hpp"

extern "C" game::IGame &getInstance()
{
    static game::Snake instance;

    return instance;
}

namespace game {
    Snake::Snake() : AGame("Snake"), _isRunning(true)
    {
        _player.push_back(std::make_tuple(SNAKE_MAP_WIDTH / 2 - 3, 37));
        _player.push_back(std::make_tuple(SNAKE_MAP_WIDTH / 2 - 2, 37));
        _player.push_back(std::make_tuple(SNAKE_MAP_WIDTH / 2 - 1, 37));
        _player.push_back(std::make_tuple(SNAKE_MAP_WIDTH / 2, 37));
        _isPaused = false;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(3, SNAKE_MAP_WIDTH - 5);

        for (int i = 0; i < 3; i++) {
            int x = distrib(gen);
            int y = distrib(gen) % SNAKE_MAP_HEIGHT;
            _apple.push_back(std::make_tuple(x + 1, y + 1));
        }
        _status = true;
        _lastEvent = display::RIGHT;
    }

    void Snake::update(DataHolder &data)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        _pix.clear();

        if (data._events.c == 'p') {
            _isPaused = !_isPaused;
            return;
        }
        if (isPaused(data))
            return;

        handle_event(data);
        displayMap();
        if (data._events.key == display::RESTART) {
            reset();
        }
        if (_status == true){
            move(data);
            drawPlayer();
            drawApple();
            drawPlayerData(data);
            isLost();
        }
        if (_status == false){
            drawLost(data);
        }
    }

    const std::vector<display::Pixel> &Snake::getPixels() const
    {
        return _pix;
    }

    void Snake::drawApple()
    {
        std::vector<display::Pixel> pix;
        int index = 0;

        for (const auto& [x, y] : _apple) {
            display::Position pos = display::Position(x, y);
            display::Color color = display::Color(255, 0, 0);
            display::Pixel newPix = display::Pixel(pos, color, '#');
            pix.emplace_back(newPix);
            index++;
        }
        drawPixel(pix);
    }

    void Snake::regenNewApple()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(3, SNAKE_MAP_WIDTH - 5);
        int x = distrib(gen);
        int y = distrib(gen) % SNAKE_MAP_HEIGHT;

        _apple.push_back(std::make_tuple(x + 1, y == 1 ? 2 : y + 1));
    }

    std::tuple<int, int> Snake::getNewPixel()
    {
        std::tuple<int, int> queue2 = _player[1];
        int queue2_x = std::get<0>(queue2);
        int queue2_y = std::get<1>(queue2);

        std::tuple<int, int> queue = _player.front();
        int queue_x = std::get<0>(queue);
        int queue_y = std::get<1>(queue);

        std::tuple<int, int> newPixel = std::make_tuple(0, 0);

        if (queue2_x > queue_x)
            newPixel = std::make_tuple(queue_x - 1, queue_y);
        else if (queue2_x < queue_x)
            newPixel = std::make_tuple(queue_x + 1, queue_y);
        else if (queue2_y > queue_y)
            newPixel = std::make_tuple(queue_x, queue_y - 1);
        else if (queue2_y < queue_y)
            newPixel = std::make_tuple(queue_x, queue_y + 1);
        return newPixel;
    }

    bool Snake::isAppleEated(DataHolder &data)
    {
        std::tuple<int, int> head = _player.back();
        int head_x = std::get<0>(head);
        int head_y = std::get<1>(head);

        std::tuple<int, int> newPixel = getNewPixel();

        for (auto it = _apple.begin(); it != _apple.end(); it++) {
            int x = std::get<0>(*it);
            int y = std::get<1>(*it);

            if (x == head_x && y == head_y) {
                data._score += 10;
                _player.push_front(newPixel);
                _apple.erase(it);
                regenNewApple();
                return true;
            }
        }
        return false;
    }

    void Snake::handle_event(DataHolder &data)
    {
        if (data._events.key == display::Key::UP && _lastEvent != display::DOWN)
            _lastEvent = display::Key::UP;
        if (data._events.key == display::Key::DOWN && _lastEvent != display::UP)
            _lastEvent = display::Key::DOWN;
        if (data._events.key == display::Key::RIGHT && _lastEvent != display::LEFT)
            _lastEvent = display::Key::RIGHT;
        if (data._events.key == display::Key::LEFT && _lastEvent != display::RIGHT)
            _lastEvent = display::Key::LEFT;
    }

    void Snake::move(DataHolder &data)
    {
        if (_lastEvent == display::Key::UP){
            std::tuple<int, int> newPixelPlayer = _player.back();
            std::get<1>(newPixelPlayer) -= 1;
            _player.push_back(newPixelPlayer);
            if (isAppleEated(data) == false){
                _player.pop_front();
            }
        }
        if (_lastEvent == display::Key::DOWN){
            std::tuple<int, int> newPixelPlayer = _player.back();
            std::get<1>(newPixelPlayer) += 1;
            _player.push_back(newPixelPlayer);
            if (isAppleEated(data) == false){
                _player.pop_front();
            }
        }
        if (_lastEvent == display::Key::RIGHT){
            std::tuple<int, int> newPixelPlayer = _player.back();
            std::get<0>(newPixelPlayer) += 1;
            _player.push_back(newPixelPlayer);
            if (isAppleEated(data) == false){
                _player.pop_front();
            }
        }
        if (_lastEvent == display::Key::LEFT){
            std::tuple<int, int> newPixelPlayer = _player.back();
            std::get<0>(newPixelPlayer) -= 1;
            _player.push_back(newPixelPlayer);
            if (isAppleEated(data) == false){
                _player.pop_front();
            }
        }
    }

    /**
     * Fonction to display the map as background
    */
    void Snake::displayMap()
    {
        display::Position pos = display::Position(0, 0);
        display::Color color = display::Color(255, 255, 0);
        AGame::drawRectangle(pos, SNAKE_MAP_WIDTH, SCREEN_HEIGHT - 1, color, false);
    }

    void Snake::drawPlayer()
    {
        std::vector<display::Pixel> pix;
        int index = 0;
        display::Position pos = display::Position(0, 0);
        display::Color color = display::Color(50, 255, 50);

        for (const auto& [x, y] : _player) {
            pos = {x, y};
            display::Pixel newPix = display::Pixel(pos, color, '#');
            pix.emplace_back(newPix);
            index++;
        }
        drawPixel(pix);
    }

     void Snake::isLost()
    {
        std::tuple<int, int> head = _player.back();
        int head_x = std::get<0>(head);
        int head_y = std::get<1>(head);

        if (head_x <= 0 || head_x >= SNAKE_MAP_WIDTH)
            _status = false;
        if (head_y <= 0 || head_y >= SNAKE_MAP_HEIGHT)
            _status = false;
        _player.pop_back();
        for (const auto& [x, y] : _player){
            if (x == head_x && y == head_y)
                _status = false;
        }
        _player.push_back(head);
    }

    void Snake::drawLost(DataHolder &data)
    {
        display::Position pos = display::Position(SNAKE_MAP_WIDTH / 2 - 5, SCREEN_HEIGHT / 2 - 1);
        display::Color color = display::Color(255, 255, 255);

        drawText(pos, "You lost!", color);
        pos = display::Position(SNAKE_MAP_WIDTH / 2 - 6, SCREEN_HEIGHT / 2 + 1);
        drawText(pos, "Shame on you", color);
        pos = display::Position(SNAKE_MAP_WIDTH / 2 - 9, SCREEN_HEIGHT / 2 + 3);
        color.b = 0;
        drawText(pos, "Press R to restart", color);

        drawPlayerData(data);
        if (data._events.key == display::RESTART) {
            _player.clear();
            _player.push_back(std::make_tuple(SNAKE_MAP_WIDTH / 2 - 3, 37));
            _player.push_back(std::make_tuple(SNAKE_MAP_WIDTH / 2 - 2, 37));
            _player.push_back(std::make_tuple(SNAKE_MAP_WIDTH / 2 - 1, 37));
            _player.push_back(std::make_tuple(SNAKE_MAP_WIDTH / 2, 37));
            _status = true;
            _lastEvent = display::NONE;
        }
    }

    void Snake::drawPlayerData(DataHolder &data)
    {
        int center = ((SCREEN_WIDTH / 3 * 2) + (SCREEN_WIDTH / 3 / 2));
        display::Position pos(center, 10);
        display::Color color(255, 255, 255);
        std::string score = "SCORE: " + std::to_string(data._score);
        std::string player = "PLAYER: ";

        player += data._username == "" ? "GUEST" : data._username;

        pos.x -= player.length() / 2;
        drawText(pos, player, color);

        pos.y += 5;
        pos.x = center - (score.length() / 2);
        drawText(pos, score, color);
    }

    void Snake::reset()
    {
        _player.clear();
        _player.push_back(std::make_tuple(SNAKE_MAP_WIDTH / 2 - 3, 37));
        _player.push_back(std::make_tuple(SNAKE_MAP_WIDTH / 2 - 2, 37));
        _player.push_back(std::make_tuple(SNAKE_MAP_WIDTH / 2 - 1, 37));
        _player.push_back(std::make_tuple(SNAKE_MAP_WIDTH / 2, 37));

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(3, SNAKE_MAP_WIDTH - 5);

        _apple.clear();
        for (int i = 0; i < 3; i++) {
            int x = distrib(gen);
            int y = distrib(gen) % SNAKE_MAP_HEIGHT;
            _apple.push_back(std::make_tuple(x + 1, y + 1));
        }
        _status = true;
        _lastEvent = display::NONE;
    }

}
