/*
** EPITECH PROJECT, 2025
** The Plazza
** File description:
** Error Cooks
*/

#include <string>

#include "Cooks.hpp"

Cooks::ErrorCooks::ErrorCooks(const std::string &message)
    : AException("Cooks", message) {
}
