/*
** EPITECH PROJECT, 2025
** Plazza
** File description:
** Arrab
*/

#include <chrono>
#include <thread>
#include <memory>
#include <string>

#include "Arrabiata.hpp"

ArrabiataClass::ArrabiataClass(int number) : APasta(number) {
}

ArrabiataClass::~ArrabiataClass() {
}

/* Method */

void ArrabiataClass::cook(int cookTime) {
    int baseCookTime = 2;
    double waitTime = baseCookTime * cookTime;
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>
        (waitTime * 1000)));
}

std::shared_ptr<Ingridient> ArrabiataClass::prepare(int number,
    std::shared_ptr<Ingridient> ingridient) {
    for (int i = 0; i < number; i++) {
        ingridient->setDough(ingridient->getDough() - 1);
        ingridient->setTomato(ingridient->getTomato() - 1);
        ingridient->setPepper(ingridient->getPepper() - 1);
        ingridient->setSteak(ingridient->getSteak() - 1);
    }
    return ingridient;
}

void ArrabiataClass::serve() {
}


/* Extern */
extern "C" {
    std::shared_ptr<IRecipe>createFood(int number) {
        return std::make_shared<ArrabiataClass>(number);
    }
    std::string getType() {
        return "Arrabiata";
    }
}
