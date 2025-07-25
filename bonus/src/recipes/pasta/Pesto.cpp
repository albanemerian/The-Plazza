/*
** EPITECH PROJECT, 2025
** The Plazza
** File description:
** Pesto
*/

#include <chrono>
#include <thread>
#include <memory>
#include <string>

#include "Pesto.hpp"

PestoClass::PestoClass(int number) : APasta(number) {
}

PestoClass::~PestoClass() {
}

/* Method */

void PestoClass::cook(int cookTime) {
    int baseCookTime = 1;
    double waitTime = baseCookTime * cookTime;
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>
        (waitTime * 1000)));
}

std::shared_ptr<Ingridient> PestoClass::prepare(int number,
    std::shared_ptr<Ingridient> ingridient) {
    for (int i = 0; i < number; i++) {
        ingridient->setDough(ingridient->getDough() - 1);
        ingridient->setBasil(ingridient->getBasil() - 1);
        ingridient->setCheese(ingridient->getCheese() - 1);
    }
    return ingridient;
}

void PestoClass::serve() {
}

/* Extern */
extern "C" {
    std::shared_ptr<IRecipe>createFood(int number) {
        return std::make_shared<PestoClass>(number);
    }
    std::string getType() {
        return "Pesto";
    }
}
