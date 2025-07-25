/*
** EPITECH PROJECT, 2025
** The Plazza
** File description:
** Error Kitchen
*/

#include <string>

#include "Kitchen.hpp"

Kitchen::ErrorKitchen::ErrorKitchen(const std::string &message)
    : AException("Kitchen", message) {
}
