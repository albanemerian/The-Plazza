/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** APasta
*/

#include "APasta.hpp"

APasta::APasta(int number) : _number(number) {
}

/* Getter */
int APasta::getNumber() const {
    return _number;
}

/* Setter */

void APasta::setNumber(int number) {
    _number = number;
}
