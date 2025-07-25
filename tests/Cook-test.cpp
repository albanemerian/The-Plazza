/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Cook-test
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/cooks/Cooks.hpp"
#include "../common/Ingridient.hpp"
#include "../common/AException.hpp"
#include "../common/messages/Order.hpp"
#include "../common/IException.hpp"
#include <memory>
#include <thread>
#include <chrono>

void redirect_all_stdout_cook(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

TestSuite(Cooks, .init = redirect_all_stdout_cook);

Test(Cooks, constructor_and_getters)
{
    auto ingredients = std::make_shared<Ingridient>();
    Cooks cook(ingredients, 42, 1000, 500);
    
    cr_assert_eq(cook.getID(), 42);
    cr_assert(!cook.isBusy(), "Cook should not be busy initially");
    cr_assert(!cook.isRestocking(), "Cook should not be restocking initially");
}

Test(Cooks, toString_pizza_types)
{
    auto ingredients = std::make_shared<Ingridient>();
    Cooks cook(ingredients, 1, 1000, 500);
    
    cr_assert_str_eq(cook.toString(Nothing).c_str(), "Nothing");
    cr_assert_str_eq(cook.toString(Regina).c_str(), "Regina");
    cr_assert_str_eq(cook.toString(Margarita).c_str(), "Margarita");
    cr_assert_str_eq(cook.toString(Americana).c_str(), "Americana");
    cr_assert_str_eq(cook.toString(Fantasia).c_str(), "Fantasia");
    cr_assert_str_eq(cook.toString(static_cast<PizzaType>(999)).c_str(), "Unknown");
}

Test(Cooks, hasEnoughIngredients_margarita_sufficient)
{
    auto ingredients = std::make_shared<Ingridient>();
    ingredients->setDough(5);
    ingredients->setTomato(5);
    ingredients->setCheese(5);
    
    Cooks cook(ingredients, 1, 1000, 500);
    
    Order order(Margarita, S, 2);
    std::string orderData = order.pack(order);
    
    bool hasEnough = cook.hasEnoughIngredients(orderData, ingredients);
    cr_assert(hasEnough, "Should have enough ingredients for Margarita");
}

Test(Cooks, hasEnoughIngredients_margarita_insufficient)
{
    auto ingredients = std::make_shared<Ingridient>();
    ingredients->setDough(1);
    ingredients->setTomato(1);
    ingredients->setCheese(1);
    
    Cooks cook(ingredients, 1, 1000, 500);
    
    Order order(Margarita, S, 5);
    std::string orderData = order.pack(order);
    
    bool hasEnough = cook.hasEnoughIngredients(orderData, ingredients);
    cr_assert(!hasEnough, "Should not have enough ingredients for 5 Margaritas");
}

Test(Cooks, hasEnoughIngredients_regina_sufficient)
{
    auto ingredients = std::make_shared<Ingridient>();
    ingredients->setDough(3);
    ingredients->setTomato(3);
    ingredients->setCheese(3);
    ingredients->setHam(3);
    ingredients->setMushroom(3);
    
    Cooks cook(ingredients, 1, 1000, 500);
    
    Order order(Regina, M, 2);
    std::string orderData = order.pack(order);
    
    bool hasEnough = cook.hasEnoughIngredients(orderData, ingredients);
    cr_assert(hasEnough, "Should have enough ingredients for Regina");
}

Test(Cooks, hasEnoughIngredients_americana_sufficient)
{
    auto ingredients = std::make_shared<Ingridient>();
    ingredients->setDough(3);
    ingredients->setTomato(3);
    ingredients->setCheese(3);
    ingredients->setSteak(3);
    
    Cooks cook(ingredients, 1, 1000, 500);
    
    Order order(Americana, L, 1);
    std::string orderData = order.pack(order);
    
    bool hasEnough = cook.hasEnoughIngredients(orderData, ingredients);
    cr_assert(hasEnough, "Should have enough ingredients for Americana");
}

Test(Cooks, hasEnoughIngredients_fantasia_sufficient)
{
    auto ingredients = std::make_shared<Ingridient>();
    ingredients->setDough(2);
    ingredients->setTomato(2);
    ingredients->setEggplant(2);
    ingredients->setCheese(2);
    ingredients->setChefLove(2);
    
    Cooks cook(ingredients, 1, 1000, 500);
    
    Order order(Fantasia, XL, 1);
    std::string orderData = order.pack(order);
    
    bool hasEnough = cook.hasEnoughIngredients(orderData, ingredients);
    cr_assert(hasEnough, "Should have enough ingredients for Fantasia");
}

Test(Cooks, hasEnoughIngredients_unknown_pizza_type)
{
    auto ingredients = std::make_shared<Ingridient>();
    ingredients->setDough(10);
    ingredients->setTomato(10);
    
    Cooks cook(ingredients, 1, 1000, 500);
    
    Order order(Nothing, S, 1);
    std::string orderData = order.pack(order);
    
    bool hasEnough = cook.hasEnoughIngredients(orderData, ingredients);
    cr_assert(!hasEnough, "Should return false for unknown pizza type");
}

Test(Cooks, restock_changes_status)
{
    auto ingridient = std::make_shared<Ingridient>();

    Cooks cook(ingridient, 2, 5, 5000);

    std::string margarit = "0x01|1|1|1";
    cook.hasEnoughIngredients(margarit, ingridient);

    std::string regina = "0x01|2|1|1";
    cook.hasEnoughIngredients(regina, ingridient);

    std::string fantasia = "0x01|8|1|1";
    cook.hasEnoughIngredients(fantasia, ingridient);

    std::string americana = "0x01|4|1|1";
    cook.hasEnoughIngredients(americana, ingridient);
}

Test(Cooks, loadPlugin_invalid_path)
{
    auto ingredients = std::make_shared<Ingridient>();
    Cooks cook(ingredients, 1, 1000, 500);
    
    auto recipe = cook.loadPlugin("/nonexistent/plugin.so", 1);
    cr_assert_null(recipe.get(), "Loading non-existent plugin should return nullptr");
}

Test(Cooks, findAndLoadPlugin_no_plugins_directory)
{
    auto ingredients = std::make_shared<Ingridient>();
    Cooks cook(ingredients, 1, 1000, 500);
    
    auto recipe = cook.findAndLoadPlugin("Margarita", 1);
    // This may return nullptr if plugins directory doesn't exist or no matching plugin found
    // We just test it doesn't crash
}

Test(Cooks, exception_creation)
{
    const std::string errorMsg = "Test cook error";
    bool exceptionThrown = false;
    
    try {
        throw AException("Cook", errorMsg);
    } catch (const IException &e) {
        exceptionThrown = true;
        std::string what = e.what();
    }
}


Test(Cooks, multiple_cooks_different_ids)
{
    auto ingredients1 = std::make_shared<Ingridient>();
    auto ingredients2 = std::make_shared<Ingridient>();
    
    Cooks cook1(ingredients1, 10, 1000, 500);
    Cooks cook2(ingredients2, 20, 1000, 500);
    
    cr_assert_eq(cook1.getID(), 10);
    cr_assert_eq(cook2.getID(), 20);
    cr_assert_neq(cook1.getID(), cook2.getID());
}

Test(Cooks, constructor_with_different_times)
{
    auto ingredients = std::make_shared<Ingridient>();
    
    Cooks fastCook(ingredients, 1, 100, 50);
    Cooks slowCook(ingredients, 2, 2000, 1000);
    
    cr_assert_eq(fastCook.getID(), 1);
    cr_assert_eq(slowCook.getID(), 2);
    
    // Both should start in the same state
    cr_assert(!fastCook.isBusy());
    cr_assert(!fastCook.isRestocking());
    cr_assert(!slowCook.isBusy());
    cr_assert(!slowCook.isRestocking());
}

Test(Cooks, hasEnoughIngredients_edge_case_exact_amount)
{
    auto ingredients = std::make_shared<Ingridient>();
    ingredients->setDough(1);
    ingredients->setTomato(1);
    ingredients->setCheese(1);
    
    Cooks cook(ingredients, 1, 1000, 500);
    
    Order order(Margarita, S, 1);
    std::string orderData = order.pack(order);
    
    bool hasEnough = cook.hasEnoughIngredients(orderData, ingredients);
    cr_assert(hasEnough, "Should have exactly enough ingredients for one Margarita");
}

Test(Cooks, startOrder_empty_orders_vector)
{
    auto ingredients = std::make_shared<Ingridient>();
    ingredients->setDough(10);
    ingredients->setTomato(10);
    ingredients->setCheese(10);
    
    Cooks cook(ingredients, 1, 100, 500);
    
    std::vector<std::string> emptyOrders;
    
    try {
        auto result = cook.startOrder(ingredients, emptyOrders);
        cr_assert_not_null(result.get(), "Should return valid ingredients even with empty orders");
        cr_assert(!cook.isBusy(), "Cook should not be busy after processing empty orders");
    } catch (const std::exception& e) {
        cr_assert_fail("Processing empty orders should not throw exception: %s", e.what());
    }
}

Test(Cooks, hasEnoughIngredients_malformed_order_data)
{
    auto ingredients = std::make_shared<Ingridient>();
    ingredients->setDough(10);
    
    Cooks cook(ingredients, 1, 1000, 500);
    
    std::string malformedData = "invalid_order_format";
    
    // This should not crash, but behavior depends on Order::unpack implementation
    bool hasEnough = cook.hasEnoughIngredients(malformedData, ingredients);
    // Result doesn't matter as much as not crashing
}
