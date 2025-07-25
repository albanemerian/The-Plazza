/*
** EPITECH PROJECT, 2025
** The Plazza
** File description:
** Error Parsing
*/

#include <string>

#include "Plazza.hpp"

Plazza::ErrorParsing::ErrorParsing(const std::string &message)
    : AException("Parsing", message) {
}
