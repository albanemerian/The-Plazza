/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Pasta-test
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <memory>
#include <iostream>
#include "../common/APasta.hpp"

// A concrete implementation of APizza for testing
class TestPasta : public APasta {
public:
    TestPasta(int number) : APasta(number) {}

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

void redirect_all_stdout_pasta(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(Pasta, constructor, .init = redirect_all_stdout_pasta) {
    int number = 1;
    TestPasta pasta(number);

    cr_assert_eq(pasta.getNumber(), number, "Constructor should set the number correctly");
}

Test(Pasta, getNumber, .init = redirect_all_stdout_pasta) {
    TestPasta pasta(5);

    cr_assert_eq(pasta.getNumber(), 5, "getNumber should return the correct number");
}

Test(Pasta, setNumber, .init = redirect_all_stdout_pasta) {
    TestPasta pasta(1);
    pasta.setNumber(10);

    cr_assert_eq(pasta.getNumber(), 10, "setNumber should update the number correctly");
}

Test(Pasta, cook, .init = redirect_all_stdout_pasta) {
    TestPasta pasta(1);
    int cookTime = 10;

    pasta.cook(cookTime);
    cr_assert_stdout_eq_str("Cooking pizza for 10 minutes\n", "cook should output the correct message");
}


Test(Pasta, serve, .init = redirect_all_stdout_pasta) {
    TestPasta pasta(1);

    pasta.serve();
    cr_assert_stdout_eq_str("Pizza is served\n", "serve should output the correct message");
}
