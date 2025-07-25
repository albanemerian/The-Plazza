/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Fantasia
*/

#include <chrono>
#include <thread>
#include <memory>
#include <string>

#include "Fantasia.hpp"

FantasiaClass::FantasiaClass(int number) :
    APizza(number) {
    // Constructor logic for FantasiaClass pizza
}

FantasiaClass::~FantasiaClass() {
}

/* Method */

void FantasiaClass::cook(int cookTime) {
    /* Cook time = 4 * multipler */
    int baseCookTime = 4;
    double waitTime = baseCookTime * cookTime;
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>
        (waitTime * 1000)));
}

std::shared_ptr<Ingridient> FantasiaClass::prepare(int number,
    std::shared_ptr<Ingridient> ingridient) {
    /* Ingridient = dough, tomato, eggplant, goat cheese and chef love*/
    for (int i = 0; i < number; i++) {
        ingridient->setDough(ingridient->getDough() - 1);
        ingridient->setTomato(ingridient->getTomato() - 1);
        ingridient->setEggplant(ingridient->getEggplant() - 1);
        ingridient->setCheese(ingridient->getCheese() - 1);
        ingridient->setChefLove(ingridient->getChefLove() - 1);
    }
    return ingridient;
}

void FantasiaClass::serve() {
    // Serving logic for Fantasia pizza
}

/* Extern */
extern "C" {
    std::shared_ptr<IRecipe>createFood(int number) {
        return std::make_shared<FantasiaClass>(number);
    }
    std::string getType() {
        return "Fantasia";
    }
}
