/*
** EPITECH PROJECT, 2025
** The Plazza
** File description:
** Bolognese
*/

#include <chrono>
#include <thread>
#include <memory>
#include <string>

#include "Bolognese.hpp"

BologneseClass::BologneseClass(int number) : APasta(number) {
}

BologneseClass::~BologneseClass() {
}

/* Method */

void BologneseClass::cook(int cookTime) {
    int baseCookTime = 2;
    double waitTime = baseCookTime * cookTime;
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>
        (waitTime * 1000)));
}

std::shared_ptr<Ingridient> BologneseClass::prepare(int number,
    std::shared_ptr<Ingridient> ingridient) {
    for (int i = 0; i < number; i++) {
        ingridient->setDough(ingridient->getDough() - 1);
        ingridient->setTomato(ingridient->getTomato() - 1);
        ingridient->setSteak(ingridient->getSteak() - 1);
        ingridient->setCheese(ingridient->getCheese() - 1);
    }
    return ingridient;
}

void BologneseClass::serve() {
}

/* Extern */
extern "C" {
    std::shared_ptr<IRecipe>createFood(int number) {
        return std::make_shared<BologneseClass>(number);
    }
    std::string getType() {
        return "Bolognese";
    }
}
