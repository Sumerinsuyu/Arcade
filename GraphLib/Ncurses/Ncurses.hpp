/*
** EPITECH PROJECT, 2025
** B-OOP-400-STG-4-1-arcade-paul1.emeriau
** File description:
** NcursesModule
*/

#ifndef NCURSESMODULE_HPP_
    #define NCURSESMODULE_HPP_

    #include "Display/ADisplay.hpp"

    #include <ncurses.h>
    #include<map>

namespace display {
    class Ncurses : public ADisplay {

        public:

            Ncurses();
            ~Ncurses();
            void display(std::vector<Pixel> &pixels) override;
            void catchEvent();

        private:
            display::Key getKey(int ch);
            std::map<enum display::Key, int> _eventAssignment;
    };
}

#endif /* !NCURSESMODULE_HPP_ */
