/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** APizza
*/

#include "APizza.hpp"

APizza::APizza(int number) : _number(number) {
}


/* Getter */

int APizza::getNumber() const {
    return _number;
}

/* Setter */

void APizza::setNumber(int number) {
    _number = number;
}
