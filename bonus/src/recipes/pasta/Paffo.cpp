/*
** EPITECH PROJECT, 2025
** La pasta o formaggio de bitta
** File description:
** Paffo
*/

#include <chrono>
#include <thread>
#include <memory>
#include <string>

#include "Paffo.hpp"

PaffoClass::PaffoClass(int number) : APasta(number) {
}

PaffoClass::~PaffoClass() {
}

/* Method */

void PaffoClass::cook(int cookTime) {
    int baseCookTime = 4;
    double waitTime = baseCookTime * cookTime;
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>
        (waitTime * 1000)));
}

std::shared_ptr<Ingridient> PaffoClass::prepare(int number,
    std::shared_ptr<Ingridient> ingridient) {
    for (int i = 0; i < number; i++) {
        ingridient->setDough(ingridient->getDough() - 1);
        ingridient->setEgg(ingridient->getEgg() - 1);
        ingridient->setCheese(ingridient->getCheese() - 1);
        ingridient->setGoatCheese(ingridient->getGoatCheese() - 1);
        ingridient->setBasil(ingridient->getBasil() - 1);
        ingridient->setMushroom(ingridient->getMushroom() - 1);
        ingridient->setChefLove(ingridient->getChefLove() - 1);
    }
    return ingridient;
}

void PaffoClass::serve() {
}

/* Extern */
extern "C" {
    std::shared_ptr<IRecipe>createFood(int number) {
        return std::make_shared<PaffoClass>(number);
    }
    std::string getType() {
        return "Paffo";
    }
}
