/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Americana
*/

#include <chrono>
#include <thread>
#include <memory>
#include <string>

#include "Americana.hpp"

AmericanaClass::AmericanaClass(int number) :
    APizza(number) {
    // Constructor logic for AmericanaClass pizza
}

AmericanaClass::~AmericanaClass() {
}

/* Mehtod */

void AmericanaClass::cook(int cookTime) {
    /* Cook time = 2 second * multiplier */
    int baseCookTime = 2;
    double waitTime = baseCookTime * cookTime;
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>
        (waitTime * 1000)));
}

std::shared_ptr<Ingridient> AmericanaClass::prepare(int number,
    std::shared_ptr<Ingridient> ingridient) {
    /* Ingridient = dough, tomato, gruyere, steak */
    for (int i = 0; i < number; i++) {
        ingridient->setDough(ingridient->getDough() - 1);
        ingridient->setTomato(ingridient->getTomato() - 1);
        ingridient->setCheese(ingridient->getCheese() - 1);
        ingridient->setSteak(ingridient->getSteak() - 1);
    }
    return ingridient;
}

void AmericanaClass::serve() {
    // Serving logic for Americana pizza
}


/* Extern */

extern "C" {
    std::shared_ptr<IRecipe>createFood(int number) {
        return std::make_shared<AmericanaClass>(number);
    }
    std::string getType() {
        return "Americana";
    }
}
