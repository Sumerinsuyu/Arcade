/*
** EPITECH PROJECT, 2025
** BSARC
** File description:
** main
*/

#include "Display/IDisplay.hpp"
#include "Core.hpp"

#include <string.h>
#include <unistd.h>
#include <csignal>

void handleSigInt(int signal)
{
    if (signal == SIGINT) {
        std::cout << "SIGINT received. Please press esc to close the program." << std::endl;
    }
}

void print_usage()
{
    std::cout << "Usage: ./arcade <graphic_library>" << std::endl;
    std::cout << "Example: ./arcade ./lib/arcade_ncurses.so" << std::endl;
    std::cout << "A Menu is required for the arcade to work." << std::endl;
}

bool isLibValid(int ac, char **av)
{
    if (ac < 2) {
        print_usage();
        std::cerr << std::endl << "ERROR: You need to specify a graphic librairie." << std::endl;
        return false;
    }

    if (strcmp(av[1], "-h") == 0)
        return true;

    if (access(av[1], F_OK) == -1) {
        std::cerr << "ERROR: " << av[1] << " no such file" << std::endl;
        return false;
    }

    char *selectedLibName = strstr(av[1], "/");

    if (!selectedLibName) {
        std::cerr << "The libraire needs to be in ./lib" << std::endl;
        return false;
    }
    return true;
}

int main(int ac, char **av)
{
    if (!isLibValid(ac, av))
        return 84;

    if (strcmp("-h", av[1]) == 0) {
        print_usage();
        return 0;
    }
    std::signal(SIGINT, handleSigInt);
    try {
        LibLoader check(av[1]);
        if (check.getLibType(av[1]) != DISPLAY_TYPE) {
            std::cerr << "ERROR: '"<< av[1] << "' is not a graphic lib." << std::endl;
            return 84;
        }

        try {
            core::Core::getInstance(check).run();
        } catch (const core::Core::Error &e) {
            std::cerr << e.what() << std::endl;
            return 84;
        }

    } catch (const LibLoader::Error &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }

    return 0;
}
