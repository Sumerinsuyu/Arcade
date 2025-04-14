/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Mar, 2025                                                     *
 * Title           - ARC                                                                *
 * Description     -                                                                    *
 *     GraphicLibLoader                                                                 *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░             *
 *         ▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓             *
 *         █  ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "LibLoader.hpp"

/**
 * @brief Return the type of the libraire eithe "GAME" or "DISPLAY"
 * @param libPath path to the lib
 * @return std::string type of the librairie
 */
std::string LibLoader::getLibType(const char *libPath)
{
    void *handle;
    type_func_t my_entry_point;
    std::string type;

        handle = dlopen(libPath, RTLD_LAZY);
        if (!handle) {
            throw Error("LibLoader: no such file: " + std::string(libPath));
        }

        my_entry_point = (type_func_t) dlsym(handle, "getType");
        if (!my_entry_point) {
            dlclose(handle);
            throw Error("Failed to open " + std::string(libPath));
        }
        type = my_entry_point();
        dlclose(handle);
        return type;
}

/**
 * @brief Load a graphical library and return an instance of it
 * @param libPath
 * @return display::IDisplay
 */
display::IDisplay &LibLoader::getGraphicalLibInstance(const char *libPath, void **graphHandle)
{
    void *handle;
    func_t my_entry_point;

        handle = dlopen(libPath, RTLD_LAZY);
        if (!handle) {
            throw Error("LibLoader: no such file: " + std::string(libPath));
        }

        my_entry_point = (func_t) dlsym(handle, "getInstance");
        if (!my_entry_point) {
            dlclose(handle);
            throw Error("Failed to open " + std::string(libPath));
        }
        *graphHandle = handle;
        return my_entry_point();
}

/**
 * @brief Load a graphical library and return an instance of it
 * @param libPath
 * @return display::IGame
 */
game::IGame &LibLoader::getGameLibInstance(const char *libPath, void **gameHandle)
{
    void *handle;
    gfunc_t my_entry_point;

        handle = dlopen(libPath, RTLD_LAZY);
        if (!handle) {
            throw Error("LibLoader: no such file: " + std::string(libPath));
        }

        my_entry_point = (gfunc_t) dlsym(handle, "getInstance");
        if (!my_entry_point) {
            dlclose(handle);
            throw Error("Failed to open " + std::string(libPath));
        }
        *gameHandle = handle;
        return my_entry_point();
}

LibLoader::LibLoader(std::string grLibName, std::string gaLibName)
: _graphicLib(&getGraphicalLibInstance(grLibName.c_str(), &_graphicHandle)),
_gameLib(&getGameLibInstance(gaLibName.c_str(), &_gameHandle))
{
    refreshNames();
}

LibLoader::Error::Error(const std::string &msg)
: _msg(msg)
{

}

/**
 * Return the error message
 * @return const char *
 */
const char *LibLoader::Error::what() const noexcept
{
    return _msg.c_str();
}

/**
 * @brief Return the loaded graphical library
 * @return display::IDisplay
 */
display::IDisplay *LibLoader::getGraphicLib()
{
    return _graphicLib;
}

/**
 * @brief Return the loaded graphical library
 * @return game::IGame
 */
game::IGame *LibLoader::getGameLib()
{
    return _gameLib;
}

/**
 * @brief return a vector of string of the game libs names
 * and check their types beforehand
 * @return std::vector<std::string>
 */
void LibLoader::refreshNames()
{
    std::vector<std::string> libNames;
    std::string libPath = "./lib";
    std::string libName;

    for (const auto &entry : std::filesystem::directory_iterator(libPath)) {
        if (entry.path().extension() == ".so") {
            if (getLibType(entry.path().c_str()) == GAME_TYPE) {
                _gameLibsNames.push_back(entry.path().filename().string());
            }
            if (getLibType(entry.path().c_str()) == DISPLAY_TYPE) {
                _graphLibsNames.push_back(entry.path().filename().string());
            }
        }
    }
}

/**
 * @brief get the vector of names of the lib in ./lib
 * @return std::vector<std::string>
 */
std::vector<std::string> LibLoader::getGameLibNames() const
{
    return _gameLibsNames;
}

/**
 * @brief get the vector of names of the lib in ./lib
 * @return std::vector<std::string>
 */
std::vector<std::string> LibLoader::getGraphLibNames() const
{
    return _graphLibsNames;
}

/**
 * @brief Change the game Instance to the game at index of gameLibName Vector
 * @param int index
 */
void LibLoader::setGameLib(int index)
{
    void* newHandle = nullptr;
    void* oldHandle = _gameHandle;
    std::string fullPath = "./lib/" + _gameLibsNames[index];
    game::IGame& newGame = getGameLibInstance(fullPath.c_str(), &newHandle);

    _gameHandle = newHandle;
    _gameLib = &newGame;

    if (oldHandle) {
        dlclose(oldHandle);
    }
}

/**
 * @brief Change the display Instance to the display at index of displayLibName Vector
 * @param int index
 */
void LibLoader::setGraphicLib(int index)
{
    std::string fullPath = "./lib/" + _graphLibsNames[index];

    if (_graphicHandle)
        dlclose(_graphicHandle);
    _graphicLib = &getGraphicalLibInstance(fullPath.c_str(), &_graphicHandle);
}
