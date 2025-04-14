/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sat, Mar, 2025                                                     *
 * Title           - ARC                                                                *
 * Description     -                                                                    *
 *     Core                                                                             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄             *
 *      ▐  ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀            *
 *        █▄▄▄▄▄  ▐  █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             *
 *        █    ▌     █          █        █        █    ▌    █         █   █             *
 *       ▄▀▄▄▄▄    ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             *
 *       █    ▐   █         █       █ █          █    ▐   █     ▐   █   █               *
 *       ▐        ▐         ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include <chrono>
#include <thread>
#include <fstream>

#include "Core.hpp"
#include "Display/IDisplay.hpp"
#include "Game/IGame.hpp"
#include "Data/DataTransfer.hpp"

namespace core {

    Core::Core(LibLoader &libLoader)
    : _userName(""),
    _score(0),
    _isRunning(true),
    _libLoader(libLoader),
    _graphicIndex(-1)
    {
        setGameIndex();
        _eventsFcts = {
            {display::QUIT,     [this](DataHolder &data, game::IGame **games) {this->eventQuit(data, games);}},
            {display::MENU,     [this](DataHolder &data, game::IGame **games) {this->eventMenu(data, games);}},
            {display::LIB,      [this](DataHolder &data, game::IGame **games) {this->eventLib(data, games);}},
            {display::GAME,     [this](DataHolder &data, game::IGame **games) {this->eventGame(data, games);}},
        };

        if (_gameIndex == -1) {
            throw Error("No menu found");
            return;
        }
    }

    Core::Error::Error(const std::string &msg)
    : _msg(msg)
    {

    }

    /**
     * Return the error message
     * @return const char *
     */
    const char *Core::Error::what() const noexcept
    {
        return _msg.c_str();
    }

    /**
     * @brief Return a static instance of the Core class so that it can have only one
     * instance running at the same time.
     * To start it the main should be 'Core::getInstance().run'
     * @param LibLoader a reference to a libLoader class for the Core constructor
     * @return Core
     */
    Core &Core::getInstance(LibLoader &libLoader)
    {
        static Core instance(libLoader);

        return instance;
    }

    /**
     * @brief Start the core and display the "menu" with the selected lib
     */
    void Core::run()
    {
        display::IDisplay *render = _libLoader.getGraphicLib();
        game::IGame *games = _libLoader.getGameLib();
        display::event ev(render->getEvent());
        DataHolder data(ev,
            _libLoader.getGameLibNames(),
            _libLoader.getGraphLibNames(), _gameIndex, _graphicIndex);
        std::vector<display::Pixel> updatedPixels;
        int tmpScore = 0;

        while (_isRunning)
        {
            getUserData(data);
            tmpScore = data._score;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 24));
            data._events = render->getEvent();
            games->update(data);
            if (tmpScore != data._score) {
                tmpScore = data._score;
                setUserData(data);
            }
            updatedPixels = games->getPixels();
            render->display(updatedPixels);
            checkEvent(data, &games, &render);
        }
    }

    /**
     * @brief Set the game index to the one with the Menu
     */
    void Core::setGameIndex()
    {
        std::vector<std::string> libs = _libLoader.getGameLibNames();

        for (int i = 0; i < (int)libs.size(); ++i) {
            if (libs[i].find("menu") != std::string::npos) {
                _gameIndex = i;
                break;
            }
        }
    }

    /**
     * @brief check the DataHolder structure's events and act afterwards depending on
     * those.
     * (ex:
     *      QUIT -> close the window and stop the application
     *      LIB  -> change the graphic librairie in use
     * )
     * @param DataHolder structure with all the events lib names...
     * @param game::IGame** pointer to Igame reference from the run loop
     * @param render::IDisplay** pointer to IDisplay reference from the run loop
     */
    void Core::checkEvent(DataHolder &data, game::IGame **games,
        display::IDisplay **render)
    {
        for (auto &call : _eventsFcts) {
            if (data._events.key == call.first) {
                call.second(data, games);
            }
        }

        if (data._gameIndex != this->_gameIndex) {
            try {
                _libLoader.setGameLib(data._gameIndex);
            } catch (const LibLoader::Error &e) {
                std::cerr << e.what() << std::endl;
                throw Error("Failed to change librairie");
            }
            *games = _libLoader.getGameLib();
            _gameIndex = data._gameIndex;
        }

        if (data._graphicIndex != this->_graphicIndex) {
            try {
                _libLoader.setGraphicLib(data._graphicIndex);
            } catch (const LibLoader::Error &e) {
                std::cerr << e.what() << std::endl;
                throw Error("Failed to change librairie");
            }
            *render = _libLoader.getGraphicLib();
            _graphicIndex = data._graphicIndex;
        }
    }

    /**
     * @brief Stop the currently running graphic lib and set _isRunning to false
     * @param DataHolder&
     * @param game::IGame**
     */
    void Core::eventQuit(__attribute_maybe_unused__ DataHolder &data,
        __attribute_maybe_unused__ game::IGame **games)
    {
        _isRunning = false;
    }

    /**
     * @brief Set the game index to the one with the "Menu" without changing the current game
     * @param DataHolder&
     * @param game::IGame**
     */
    void Core::eventMenu(__attribute_maybe_unused__ DataHolder &data,
        __attribute_maybe_unused__ game::IGame **games)
    {
        if (data._isTyping) {
            return;
        }
        int old = _gameIndex;
        setGameIndex();
        data._gameIndex = _gameIndex;
        _gameIndex = old;
    }

    /**
     * @brief Cycle through the available graphic libraries and update the graphic index
     * @param DataHolder&
     * @param game::IGame**
     */
    void Core::eventLib(__attribute_maybe_unused__ DataHolder &data,
        __attribute_maybe_unused__ game::IGame **games)
    {
        if (data._isTyping) {
            return;
        }
        data._graphicIndex = static_cast<int>(_libLoader.getGraphLibNames().size() - 1) == _graphicIndex ? 0 : _graphicIndex + 1;
    }

    /**
     * @brief Cycle through the available game libraries and update the game index
     * @param DataHolder&
     * @param game::IGame**
     */
    void Core::eventGame(__attribute_maybe_unused__ DataHolder &data,
        __attribute_maybe_unused__ game::IGame **games)
    {
        if (data._isTyping) {
            return;
        }
        data._gameIndex = static_cast<int>(_libLoader.getGameLibNames().size() - 1) == _gameIndex ? 0 : _gameIndex + 1;
    }

    /**
     * @brief Create a file users/users.arc if it doesn't exist the users data will be
     * stored in it with the format username:score
     */
    void Core::createUserData()
    {
        std::filesystem::path usersDir("users");

        if (!std::filesystem::exists(usersDir)) {
            std::filesystem::create_directory(usersDir);
        }

        std::filesystem::path userFile(USER_FILE);
        if (!std::filesystem::exists(userFile)) {
            std::ofstream file(userFile);
            file.close();
        }
    }

    /**
     * @brief Search for the current _username in data and get it's score.
     * It also create the users.arc file if it doesn't exist
     * @param DataHolder data
     */
    void Core::getUserData(DataHolder &data)
    {
        createUserData();

        std::ifstream file(USER_FILE);
        std::string line;

        while (std::getline(file, line)) {
            if (!line.empty()) {
                size_t delimiterPos = line.find(':');
                if (delimiterPos != std::string::npos && data._username == line.substr(0, delimiterPos)) {
                    data._score = std::stoi(line.substr(delimiterPos + 1));
                    file.close();
                    return;
                }
            }
        }

        if (data._username != "" && !data._isTyping) {
            std::ofstream outFile(USER_FILE, std::ios::app);
            outFile << data._username << ":0" << std::endl;
            outFile.close();
        }
        file.close();
    }

    /**
     * @brief Update the current's player score in the users.arc file
     * @param DataHolder data
     */
    void Core::setUserData(DataHolder &data)
    {
        std::vector<std::string> lines;
        std::ifstream inFile(USER_FILE);
        std::string line;

        while (std::getline(inFile, line)) {
            if (!line.empty()) {
                size_t delimiterPos = line.find(':');
                if (delimiterPos != std::string::npos && data._username == line.substr(0, delimiterPos)) {
                    lines.push_back(data._username + ":" + std::to_string(data._score));
                    continue;
                }
                lines.push_back(line);
            }
        }
        inFile.close();

        std::ofstream outFile(USER_FILE);
        for (const auto &l : lines) {
            outFile << l << std::endl;
        }
        outFile.close();
    }
}
