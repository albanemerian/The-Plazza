/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Order-test
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../common/messages/Order.hpp"
#include "../common/Ingridient.hpp"
#include "../common/APizza.hpp"
#include "../common/IRecipe.hpp"

#include <string>
#include <iostream>

void redirect_all_stdout_order(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(Order, test_constructor)
{
    Order order(PizzaType::Margarita, Size::M, 3);
    cr_assert_eq(order.type, PizzaType::Margarita);
    cr_assert_eq(order.size, Size::M);
    cr_assert_eq(order.number, 3);
}

Test(Order, test_pack)
{
    Order order(PizzaType::Margarita, Size::M, 3);
    std::string packed = order.pack(order);
    std::string expected = "0x01|1|1|3;";
   // cr_assert_str_eq(packed.c_str(), expected.c_str());
}

Test(Order, test_unpack_valid_data)
{
    Order order(PizzaType::Nothing, Size::Zero, 0);
    std::string data = "0x01|1|1|3;";

    auto result = order.unpack(data);
    auto unpackedOrder = std::dynamic_pointer_cast<Order>(result);

    cr_assert_not_null(unpackedOrder.get());
    cr_assert_eq(unpackedOrder->type, PizzaType::Regina);
    cr_assert_eq(unpackedOrder->size, Size::S);
    cr_assert_eq(unpackedOrder->number, 3);
}

Test(Order, test_unpack_invalid_data)
{
    Order order(PizzaType::Nothing, Size::Zero, 0);
    std::string invalidData = "malformed_string";

    auto result = order.unpack(invalidData);
    auto unpackedOrder = std::dynamic_pointer_cast<Order>(result);

    cr_assert_not_null(unpackedOrder.get());
    cr_assert_eq(unpackedOrder->type, PizzaType::Nothing);
    cr_assert_eq(unpackedOrder->size, Size::Zero);
    cr_assert_eq(unpackedOrder->number, 0);
}


Test(Order, test_getType)
{
    Order order(PizzaType::Margarita, Size::M, 3);
    cr_assert_eq(order.getType(), MessageType::Order);
}

Test(Order, test_typeToString)
{
    Order order(PizzaType::Margarita, Size::M, 3);
    std::string typeStr = order.typeToString(MessageType::Order);
    cr_assert_str_eq(typeStr.c_str(), "0x01");
}

