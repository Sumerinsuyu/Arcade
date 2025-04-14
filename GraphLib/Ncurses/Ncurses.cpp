/*
** EPITECH PROJECT, 2025
** B-OOP-400-STG-4-1-arcade-paul1.emeriau
** File description:
** Ncurses
*/

#include "Ncurses.hpp"
#include <vector>

/**
 * @brief return a string to be able to know which type of librairie it is.
 * In the case of SDL2 it is "DISPLAY"
 */
extern "C" std::string getType()
{
    return DISPLAY_TYPE;
}

/**
 * @brief return an instance of display::SFML
 */
extern "C" display::IDisplay &getInstance()
{
    static display::Ncurses instance;

    return instance;
}

namespace display {

    Ncurses::Ncurses()
    : ADisplay("Ncurses")
    {
        _eventAssignment = {
            {display::UP,       'z'},
            {display::DOWN,     's'},
            {display::LEFT,     'q'},
            {display::RIGHT,    'd'},
            {display::LIB,      'l'},
            {display::MENU,     'm'},
            {display::GAME,     'g'},
            {display::RESTART,  'r'},
            {display::QUIT,     27},
            {display::SELECT,   10},
        };

        initscr();
        start_color();
        use_default_colors();
        curs_set(0);
        noecho();
        cbreak();
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        init_pair(1, COLOR_RED, -1);
        init_pair(2, COLOR_GREEN, -1);
        init_pair(3, COLOR_BLUE, -1);
        init_pair(4, COLOR_YELLOW, -1);
        init_pair(5, COLOR_WHITE, -1);
        init_pair(6, COLOR_BLACK, COLOR_RED);
        init_pair(7, COLOR_BLACK, COLOR_GREEN);
        init_pair(8, COLOR_BLACK, COLOR_BLUE);
        init_pair(9, COLOR_BLACK, COLOR_YELLOW);
        init_pair(10, COLOR_BLACK, COLOR_WHITE);
    }

    Ncurses::~Ncurses()
    {
        clear();
        refresh();
        endwin();
        resetterm();
        fflush(stdout);
    }

    /**
     * @brief Get an RBG color in argument
     * Return an int between 1 and 5
     * Corresponding to the closest main color we have
     * (Red, Green, Blue, Yellow, White)
    */
    short getColorPairId(const display::Color &color) {
        if (color.r > 200 && color.g < 100 && color.b < 100)
            return 1;
        if (color.r < 100 && color.g > 200 && color.b < 100)
            return 2;
        if (color.r < 100 && color.g < 100 && color.b > 200)
            return 3;
        if (color.r > 200 && color.g > 200 && color.b < 100)
            return 4;
        return 5;
    }

    /**
     * @brief Display all pixel from _pix with his color
    */
   void Ncurses::display(std::vector<Pixel> &pix) {
        erase();
        for (const auto &pixel : pix) {
            short pairId = getColorPairId(pixel.color);
            if (pixel.c == '#'){
                attron(COLOR_PAIR(pairId + 5));
                mvaddch(pixel.pos.y, pixel.pos.x, ' ');
                attroff(COLOR_PAIR(pairId + 5));
            } else {
                attron(COLOR_PAIR(pairId));
                mvaddch(pixel.pos.y, pixel.pos.x, pixel.c);
                attroff(COLOR_PAIR(pairId));
            }
        }
        catchEvent();
        refresh();
    }

    /**
     * loop through the map of assignable keys and return the one if assigned
     * or NONE if not assignable
     * @param int ch pressed key value
     * @return display::Key
     */
    display::Key Ncurses::getKey(int ch)
    {
        // ! those have to be harcoded because comparing it from the tuplpe doesn't work
        if (ch == KEY_UP) {
            return display::UP;
        }
        if (ch == KEY_DOWN) {
            return display::DOWN;
        }
        if (ch == KEY_LEFT) {
            return display::LEFT;
        }
        if (ch == KEY_RIGHT) {
            return display::RIGHT;
        }

        for (auto [key, value] : _eventAssignment) {
            if (value == ch) {
                return key;
            }
        }
        return display::NONE;
    }

    /**
     * @brief Get the pressed keys and set the event strucure accordingly
     */
    void Ncurses::catchEvent()
    {
        int ch = getch();

        _event = display::event(getKey(ch), ch);
    }
}
