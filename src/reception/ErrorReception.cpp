/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** ReceptionExeption
*/

#include <string>

#include "Reception.hpp"

Reception::ErrorReception::ErrorReception(const std::string &message)
    : AException("Reception", message) {
}
