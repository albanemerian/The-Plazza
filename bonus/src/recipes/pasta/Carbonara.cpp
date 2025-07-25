/*
** EPITECH PROJECT, 2025
** The Plazza
** File description:
** Carbonara pasta
*/

#include <chrono>
#include <thread>
#include <memory>
#include <string>

#include "Carbonara.hpp"

CarbonaraClass::CarbonaraClass(int number) : APasta(number) {
}

CarbonaraClass::~CarbonaraClass() {
}

/* Method */

void CarbonaraClass::cook(int cookTime) {
    int baseCookTime = 1;
    double waitTime = baseCookTime * cookTime;
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>
        (waitTime * 1000)));
}

std::shared_ptr<Ingridient> CarbonaraClass::prepare(int number,
    std::shared_ptr<Ingridient> ingridient) {
    for (int i = 0; i < number; i++) {
        ingridient->setDough(ingridient->getDough() - 1);
        ingridient->setEgg(ingridient->getEgg() - 1);
        ingridient->setBacon(ingridient->getBacon() - 1);
    }
    return ingridient;
}

void CarbonaraClass::serve() {
}


/* Extern */
extern "C" {
    std::shared_ptr<IRecipe>createFood(int number) {
        return std::make_shared<CarbonaraClass>(number);
    }
    std::string getType() {
        return "Carbonara";
    }
}
