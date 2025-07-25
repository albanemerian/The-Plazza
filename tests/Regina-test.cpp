/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Regina-test
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <memory>
#include <chrono>
#include <iostream>
#include "../src/recipes/pizza/Regina.hpp"

extern "C" {
    std::shared_ptr<IRecipe> createFood(int number);
    std::string getType();
}

void redirect_all_stdout_regina(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(Regina, constructor, .init = redirect_all_stdout_regina)
{
    int number = 1;
    ReginaClass regina(number);

    cr_assert_eq(regina.getNumber(), number, "Constructor should set the number correctly");
}

Test(Regina, cook_method, .init = redirect_all_stdout_regina)
{
    int number = 1;
    ReginaClass regina(number);

    auto startTime = std::chrono::high_resolution_clock::now();

    regina.cook(1);

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

    cr_assert_geq(duration, 1, "Cook time should be at least 2 seconds");
    cr_assert_leq(duration, 3, "Cook time should be at most 2 seconds (with small margin)");
}

Test(Regina, prepare_method, .init = redirect_all_stdout_regina)
{
    int number = 2;
    ReginaClass regina(number);

    std::shared_ptr<Ingridient> ingredients = std::make_shared<Ingridient>();
    ingredients->setDough(10);
    ingredients->setTomato(10);
    ingredients->setCheese(10);
    ingredients->setHam(10);
    ingredients->setMushroom(10);

    auto updatedIngredients = regina.prepare(number, ingredients);

    cr_assert_eq(updatedIngredients->getDough(), 8, "Should use 2 units of dough");
    cr_assert_eq(updatedIngredients->getTomato(), 8, "Should use 2 units of tomato");
    cr_assert_eq(updatedIngredients->getCheese(), 8, "Should use 2 units of cheese");
    cr_assert_eq(updatedIngredients->getHam(), 8, "Should use 2 units of ham");
    cr_assert_eq(updatedIngredients->getMushroom(), 8, "Should use 2 units of mushroom");
}

Test(Regina, serve_method, .init = redirect_all_stdout_regina)
{
    int number = 1;
    ReginaClass regina(number);

    regina.serve();

    cr_assert(true);
}

Test(Regina, external_functions, .init = redirect_all_stdout_regina)
{
    int number = 3;
    auto pizzaPtr = createFood(number);

    cr_assert_not_null(pizzaPtr, "createFood should return a valid pointer");

    cr_assert_eq(pizzaPtr->getNumber(), number, "Created pizza should have the correct number");

    std::string type = getType();
    cr_assert_str_eq(type.c_str(), "Regina", "getType should return 'Regina'");
}

