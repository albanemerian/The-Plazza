/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** commonTest
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <memory>
#include <iostream>
#include "../common/APizza.hpp"


class TestPizza : public APizza {
public:
    TestPizza(int number) : APizza(number) {}

    void cook(int cookTime) override {
        std::cout << "Cooking pizza for " << cookTime << " minutes" << std::endl;
    }

    std::shared_ptr<Ingridient> prepare(int number, std::shared_ptr<Ingridient> ingridient) override {
        std::cout << "Preparing " << number << " pizzas with ingredient" << std::endl;
        return ingridient;
    }

    void serve() override {
        std::cout << "Pizza is served" << std::endl;
    }
};

void redirect_all_stdout_pizza(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(Pizza, constructor, .init = redirect_all_stdout_pizza) {
    int number = 1;
    TestPizza pizza(number);

    cr_assert_eq(pizza.getNumber(), number, "Constructor should set the number correctly");
}

Test(Pizza, getNumber, .init = redirect_all_stdout_pizza) {
    TestPizza pizza(5);

    cr_assert_eq(pizza.getNumber(), 5, "getNumber should return the correct number");
}

Test(Pizza, setNumber, .init = redirect_all_stdout_pizza) {
    TestPizza pizza(1);
    pizza.setNumber(10);

    cr_assert_eq(pizza.getNumber(), 10, "setNumber should update the number correctly");
}

Test(Pizza, cook, .init = redirect_all_stdout_pizza) {
    TestPizza pizza(1);
    pizza.cook(15);

    cr_assert_stdout_eq_str("Cooking pizza for 15 minutes\n", "cook method should print the correct message");
}

Test(Pizza, serve, .init = redirect_all_stdout_pizza) {
    TestPizza pizza(1);
    pizza.serve();

    cr_assert_stdout_eq_str("Pizza is served\n", "serve method should print the correct message");
}
