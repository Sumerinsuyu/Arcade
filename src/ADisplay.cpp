/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Apr, 2025                                                     *
 * Title           - ARC                                                                *
 * Description     -                                                                    *
 *     ADisplay                                                                         *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                     *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                     *
 *             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                     *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "Display/ADisplay.hpp"

namespace display {

    ADisplay::ADisplay(std::string name)
    : _name(name), _event(NONE, 0) {};

    ADisplay::Error::Error(const std::string &msg)
    : _msg(msg)
    {

    }

    /**
     * Return the error message
     * @return const char *
     */
    const char *ADisplay::Error::what() const noexcept
    {
        return _msg.c_str();
    }

    event ADisplay::getEvent()
    {
        return _event;
    }
}