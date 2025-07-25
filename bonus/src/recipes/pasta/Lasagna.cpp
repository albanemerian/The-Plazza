/*
** EPITECH PROJECT, 2025
** The Plazza
** File description:
** Pasta
*/

#include <chrono>
#include <thread>
#include <memory>
#include <string>

#include "Lasagna.hpp"

LasagnaClass::LasagnaClass(int number) : APasta(number) {
}

LasagnaClass::~LasagnaClass() {
}

/* Method */

void LasagnaClass::cook(int cookTime) {
    int baseCookTime = 5;
    double waitTime = baseCookTime * cookTime;
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>
        (waitTime * 1000)));
}

std::shared_ptr<Ingridient> LasagnaClass::prepare(int number,
    std::shared_ptr<Ingridient> ingridient) {
    for (int i = 0; i < number; i++) {
        ingridient->setDough(ingridient->getDough() - 1);
        ingridient->setTomato(ingridient->getTomato() - 1);
        ingridient->setCheese(ingridient->getCheese() - 1);
        ingridient->setSteak(ingridient->getSteak() - 1);
        ingridient->setEggplant(ingridient->getEggplant() - 1);
    }
    return ingridient;
}

void LasagnaClass::serve() {
}


/* Extern */

extern "C" {
    std::shared_ptr<IRecipe>createFood(int number) {
        return std::make_shared<LasagnaClass>(number);
    }
    std::string getType() {
        return "Lasagna";
    }
}
