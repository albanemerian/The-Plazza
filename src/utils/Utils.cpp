/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** utils
*/

#include "Utils.hpp"
#include <iostream>

void Utils::helper() {
    std::cout << "Usage: ./plazza <multiplier Cooking time> [-d] ";
    std::cout << "<number of cooks> <Restock time milli-second>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "\t in the shell you can use : \n";
    std::cout << "\t\t - 'exit' to quit the program" << std::endl;
    std::cout << "\t\t - 'clear' to clear the screen" << std::endl;
    std::cout << "\t\t - 'menu' display the available order" << std::endl;
    std::cout << "\t\t - 'status' | 'S' to display the state of the restaurant"
        << std::endl;
}
