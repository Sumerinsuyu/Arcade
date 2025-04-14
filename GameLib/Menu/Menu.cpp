/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Fri, Mar, 2025                                                     *
 * Title           - ARC                                                                *
 * Description     -                                                                    *
 *     Menu                                                                             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _|           *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _|           *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_|           *
 *       _|        _|          _|        _|      _|        _|        _|    _|           *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _|           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "Menu.hpp"

extern "C" game::IGame &getInstance()
{
    static game::Menu instance;

    return instance;
}

namespace game {

    Menu::Menu()
    : AGame("Menu"),
    _section(PLAYER),
    _mode(SELECTION),
    _gameIndex(0),
    _graphicIndex(0)
    {
        _instruction.emplace_back("Use <- and -> to navigate between sections.");
        _instruction.emplace_back("Use up and down arrows to navigate between fields.");
        _instruction.emplace_back("Press G to change graphic librairie");
        _instruction.emplace_back("Press ESC to quit the arcade");
        _instruction.emplace_back("Press P to pause in game");
        _instruction.emplace_back("Press M to come back in the menu (in game)");
        _instruction.emplace_back("Press ENTER to change the username");
    }

    void Menu::update(DataHolder &data)
    {
        _pix.clear();

        setSection(data);
        setSelectedItem(data._events, data._gameLibs.size(), GAMES, _gameIndex);
        setSelectedItem(data._events, data._graphiLibs.size(), GRAPHICAL, _graphicIndex);
        if (data._gameLibs[_gameIndex].find("menu") != std::string::npos)
            setSelectedItem(data._events, data._gameLibs.size(), GAMES, _gameIndex);
        changeSelectedItem(data);

        drawBase();
        drawData(data._events, data._username, data._score);
        drawLibs(data._gameLibs, (SCREEN_WIDTH / 3 / 2 + (SCREEN_WIDTH / 3)),
            "Select a Game and press ENTER to start it", _gameIndex, GAMES);
        drawLibs(data._graphiLibs, (SCREEN_WIDTH / 3 / 2 + (SCREEN_WIDTH / 3 * 2)),
            "Select a Librairie and press ENTER to switch to it", _graphicIndex, GRAPHICAL);
        drawAsciiArt();
    }

    const std::vector<display::Pixel> &Menu::getPixels() const
    {
        return _pix;
    }

    void Menu::setSection(DataHolder &data)
    {
        if (_mode == TEXT && data._events.key == display::SELECT) {
            _mode = SELECTION;
            data._isTyping = false;
            return;
        }
        if (_section == PLAYER && data._events.key == display::SELECT) {
            data._username.clear();
            _mode = TEXT;
            data._isTyping = true;
        }

        if (_mode == SELECTION) {
            if (data._events.key == display::RIGHT) {
                _section = _section + 1 < TOTAL ? (sections)(_section + 1) : PLAYER;
            }
            if (data._events.key == display::LEFT) {
                _section = _section - 1 >= 0 ? (sections)(_section - 1) : (sections)(TOTAL - 1);
            }
        }
    }

    void Menu::setSelectedItem(display::event &events, int size,
        enum sections selectedSec, int &index)
    {
        if (_section == selectedSec && events.key == display::DOWN) {
            index = index + 1 < size ? index + 1 : 0;
        }
        if (_section == selectedSec && events.key == display::UP) {
            index = index - 1 >= 0 ? index - 1 : (size - 1);
        }
    }

    void Menu::drawBase()
    {
        display::Position posBorder(0, 0);
        display::Color borderColor(255, 255, 255);
        drawRectangle(posBorder, SCREEN_WIDTH, SCREEN_HEIGHT, borderColor, false);

        posBorder.x = (SCREEN_WIDTH / 3);
        posBorder.y = 0;
        drawRectangle(posBorder, 1, SCREEN_HEIGHT, borderColor, false);

        posBorder.x += (SCREEN_WIDTH / 3);
        drawRectangle(posBorder, 1, SCREEN_HEIGHT, borderColor, false);

    }

    void Menu::drawData(display::event &events, std::string &username, int score)
    {
        display::Position pos(0, 0);
        display::Color color(255, 255, 255);
        std::string name = "Player: ";

        pos.x = 1;
        pos.y = 1;
        if (_mode == TEXT && std::isalpha(events.c)) {
            username += events.c;
        }
        if (_section == PLAYER) {
            color = display::Color(0, 255, 0);
        }
        drawText(pos, name + username, color);

        color = display::Color(255, 255, 255);

        pos.y += 2;
        drawText(pos, "Total Score: " + std::to_string(score), color);

        pos.y = SCREEN_HEIGHT / 2;
        color.b = 0;

        for (const auto &instr : _instruction) {
            pos.x = (SCREEN_WIDTH / 3 / 2) - (instr.length() / 2);
            drawText(pos, instr, color);
            pos.y += 2;
        }
    }

    void Menu::drawLibs(std::vector<std::string> libs, int leftCol,
        std::string helper, int index, enum sections section)
    {
        display::Position pos(SCREEN_WIDTH / 3 + 1, 1);
        display::Color color(255, 255, 255);
        display::Color colorGreen(0, 255, 0);
        std::string instruction = helper;

        pos.x = leftCol - (instruction.length() / 2);
        pos.y = 3;
        drawText(pos, instruction, color);

        for (int i = 0; i < static_cast<int>(libs.size()); ++i) {
            instruction = trimGame(libs[i]);
            if (instruction.find("Menu") != std::string::npos)
                continue;
            pos.x = leftCol - (instruction.length() / 2);
            pos.y += 5;

            if (i == index && _section == section && _mode == SELECTION) {
                instruction = "-> " + instruction + " <-";
                pos.x = leftCol - (instruction.length() / 2);
                drawText(pos, instruction, colorGreen);
                continue;
            }
            drawText(pos, instruction, color);
        }
    }

    void Menu::drawAsciiArt()
    {
        display::Position pos(SCREEN_WIDTH / 3 + 1, SCREEN_HEIGHT - 7);
        display::Color color(255, 255, 0);
        std::string line = "       ,@@.                   ,==.";

        pos.x = (SCREEN_WIDTH / 3 / 2 + (SCREEN_WIDTH / 3)) - (line.length() / 2);
        drawText(pos, line, color);

        line = "     ,@    @.                 \\ o ',";
        pos.y += 1;
        pos.x = (SCREEN_WIDTH / 3 / 2 + (SCREEN_WIDTH / 3)) - (line.length() / 2);
        drawText(pos, line, color);

        line = "    @        @     _     _     \\    \\";
        pos.y += 1;
        pos.x = (SCREEN_WIDTH / 3 / 2 + (SCREEN_WIDTH / 3)) - (line.length() / 2);
        drawText(pos, line, color);

        line = "    @        @    (_)   (_)    /    ;";
        pos.y += 1;
        pos.x = (SCREEN_WIDTH / 3 / 2 + (SCREEN_WIDTH / 3)) - (line.length() / 2);
        drawText(pos, line, color);

        line = "     `@    @'                 /   .'";
        pos.y += 1;
        pos.x = (SCREEN_WIDTH / 3 / 2 + (SCREEN_WIDTH / 3)) - (line.length() / 2);
        drawText(pos, line, color);

        line = "       `@@'                   \"==\"";
        pos.y += 1;
        pos.x = (SCREEN_WIDTH / 3 / 2 + (SCREEN_WIDTH / 3)) - (line.length() / 2);
        drawText(pos, line, color);
    }

    std::string Menu::trimGame(std::string gameName)
    {
        size_t start = gameName.find('_') + 1;
        size_t end = gameName.find_last_of('.');

        if (start == std::string::npos || end == std::string::npos || start >= end) {
            return "";
        }
        std::string trimmed = gameName.substr(start, end - start);
        if (!trimmed.empty()) {
            trimmed[0] = std::toupper(trimmed[0]);
        }
        return trimmed;
    }

    void Menu::changeSelectedItem(DataHolder &data)
    {
        if (_section == GAMES && data._events.key == display::SELECT) {
            data._gameIndex = _gameIndex;
        }
        if (_section == GRAPHICAL && data._events.key == display::SELECT) {
            data._graphicIndex = _graphicIndex;
        }
    }
}

