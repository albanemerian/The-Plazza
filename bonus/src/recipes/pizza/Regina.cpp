/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Regina
*/

#include <chrono>
#include <thread>
#include <memory>
#include <iostream>
#include <string>

#include "Regina.hpp"

ReginaClass::ReginaClass(int number) :
    APizza(number) {
    // Constructor logic for ReginaClass pizza
}

ReginaClass::~ReginaClass() {
}

/* Method */
void ReginaClass::cook(int cookTime) {
    /* Cook time = 2 * multiplier */
    int baseCookTime = 2;
    double waitTime = baseCookTime * cookTime;
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>
        (waitTime * 1000)));
}

std::shared_ptr<Ingridient> ReginaClass::prepare(int number,
    std::shared_ptr<Ingridient> ingridient) {
    /* dough, tomato, gruyere, ham, mushrooms */
    for (int i = 0; i < number; i++) {
        ingridient->setDough(ingridient->getDough() - 1);
        ingridient->setTomato(ingridient->getTomato() - 1);
        ingridient->setCheese(ingridient->getCheese() - 1);
        ingridient->setHam(ingridient->getHam() - 1);
        ingridient->setMushroom(ingridient->getMushroom() - 1);
        ingridient->fridgeStatus();
    }
    return ingridient;
}

void ReginaClass::serve() {
    // Serving logic for Regina pizza
}


/* Extern */

extern "C" {
    std::shared_ptr<IRecipe>createFood(int number) {
        return std::make_shared<ReginaClass>(number);
    }
    std::string getType() {
        return "Regina";
    }
}
