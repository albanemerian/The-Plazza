/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Margarita
*/

#include <chrono>
#include <thread>
#include <memory>
#include <string>

#include "Margarita.hpp"

MargaritaClass::MargaritaClass(int number) :
    APizza(number) {
    // Constructor logic for MargaritaClass pizza
}

MargaritaClass::~MargaritaClass() {
}

/* Method */

void MargaritaClass::cook(int cookTime) {
    /* Cook time = 1 * multipler */
    int baseCookTime = 1;
    double waitTime = baseCookTime * cookTime;
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>
        (waitTime * 1000)));
}

std::shared_ptr<Ingridient> MargaritaClass::prepare(int number,
    std::shared_ptr<Ingridient> ingridient) {
    /* Ingridient = dough, tomato and gruyere*/
    for (int i = 0; i < number; i++) {
         ingridient->setDough(ingridient->getDough() - 1);
         ingridient->setTomato(ingridient->getTomato() - 1);
         ingridient->setCheese(ingridient->getCheese() - 1);
    }
    return ingridient;
}

void MargaritaClass::serve() {
    // Serving logic for Margarita pizza
}

/* Extern */

extern "C" {
    std::shared_ptr<IRecipe>createFood(int number) {
        return std::make_shared<MargaritaClass>(number);
    }
    std::string getType() {
        return "Margarita";
    }
}
