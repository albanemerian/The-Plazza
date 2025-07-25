/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** *
*/

#include <iostream>

#include "Plazza.hpp"
#include "../common/IException.hpp"

int main(int ac, char **av) {
    try {
        Plazza plazza;
        plazza.parseCmd(av, ac);
        plazza.orderingLoop();
    } catch (const IException &e) {
        std::cerr << e.getFormattedMessage() << std::endl;
        return 84;
    }
    return 0;
}
