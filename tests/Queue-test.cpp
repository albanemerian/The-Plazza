/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Queue-test
*/
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../common/messages/Queue.hpp"

#include <string>
#include <iostream>

void redirect_all_stdout_queue(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(Queue, test_constructor)
{
    Queue queue(42, 5);
    cr_assert_eq(queue.kitchenId, 42);
    cr_assert_eq(queue.nbCurrentOrders, 5);
}

Test(Queue, test_pack)
{
    Queue queue(24, 3);
    std::string packed = queue.pack(queue);
    std::string expected = "0x05|24|3;";
    cr_assert_str_eq(packed.c_str(), expected.c_str());
}

Test(Queue, test_unpack_valid_data)
{
    Queue queue(0, 0);
    std::string data = "0x05|24|3;";

    auto result = queue.unpack(data);
    auto unpackedQueue = std::dynamic_pointer_cast<Queue>(result);

    cr_assert_not_null(unpackedQueue.get());
    cr_assert_eq(unpackedQueue->kitchenId, 24);
    cr_assert_eq(unpackedQueue->nbCurrentOrders, 3);
}

Test(Queue, test_unpack_invalid_data)
{
    Queue queue(0, 0);
    std::string invalidData = "malformed_string";

    auto result = queue.unpack(invalidData);
    auto unpackedQueue = std::dynamic_pointer_cast<Queue>(result);

    cr_assert_not_null(unpackedQueue.get());
    cr_assert_eq(unpackedQueue->kitchenId, 0);
    cr_assert_eq(unpackedQueue->nbCurrentOrders, 0);
}

Test(Queue, test_getType)
{
    Queue queue(1, 0);
    cr_assert_eq(queue.getType(), MessageType::Queue);
}

Test(Queue, test_typeToString)
{
    Queue queue(1, 0);
    std::string typeStr = queue.typeToString(MessageType::Queue);
    cr_assert_str_eq(typeStr.c_str(), "0x05");
}

