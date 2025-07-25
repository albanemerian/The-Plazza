/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** AStatus-test
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../common/messages/AStatus.hpp"
#include "../common/messages/DoneStatus.hpp"
#include "../common/messages/RefillStatus.hpp"
#include "../common/Ingridient.hpp"
#include <string>
#include <iostream>

void redirect_all_stdout_astatus(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

TestSuite(DoneStatus, .init = redirect_all_stdout_astatus);

Test(DoneStatus, test_constructor)
{
    std::vector<ingStat> ingredients = {
        {IngridientType::DOUGH, 5},
        {IngridientType::TOMATO, 3}
    };
    DoneStatus status(42, ingredients);

    cr_assert_eq(status.kitchenId, 42);
    cr_assert_eq(status.status.size(), 2);
    cr_assert_eq(status.status[0].type, IngridientType::DOUGH);
    cr_assert_eq(status.status[0].quantity, 5);
}

Test(DoneStatus, test_getType)
{
    DoneStatus status(1, {});
    cr_assert_eq(status.getType(), MessageType::Status);
}

Test(DoneStatus, test_pack)
{
    std::vector<ingStat> ingredients = {
        {IngridientType::DOUGH, 5},
        {IngridientType::TOMATO, 3}
    };
    DoneStatus status(42, ingredients);

    std::string packed = status.pack(status);
    std::string expected = "0x02|42|0:5,1:3;";
    cr_assert_str_eq(packed.c_str(), expected.c_str());
}

Test(DoneStatus, test_pack_empty_ingredients)
{
    DoneStatus status(10, {});

    std::string packed = status.pack(status);
    std::string expected = "0x02|10|;";
    //cr_assert_str_eq(packed.c_str(), expected.c_str());
}

Test(DoneStatus, test_unpack_valid_data)
{
    DoneStatus status(0, {});
    std::string data = "0x02|42|0:5,1:3;";

    auto result = status.unpack(data);
    auto doneStatus = std::dynamic_pointer_cast<DoneStatus>(result);

    cr_assert_not_null(doneStatus.get());
    cr_assert_eq(doneStatus->kitchenId, 42);
    cr_assert_eq(doneStatus->status.size(), 2);
    cr_assert_eq(doneStatus->status[0].type, IngridientType::DOUGH);
    cr_assert_eq(doneStatus->status[0].quantity, 5);
    cr_assert_eq(doneStatus->status[1].type, IngridientType::TOMATO);
    cr_assert_eq(doneStatus->status[1].quantity, 3);
}

Test(DoneStatus, test_unpack_single_ingredient)
{
    DoneStatus status(0, {});
    std::string data = "0x02|15|2:10;";

    auto result = status.unpack(data);
    auto doneStatus = std::dynamic_pointer_cast<DoneStatus>(result);

    cr_assert_not_null(doneStatus.get());
    cr_assert_eq(doneStatus->kitchenId, 15);
    cr_assert_eq(doneStatus->status.size(), 1);
    cr_assert_eq(doneStatus->status[0].type, static_cast<IngridientType>(2));
    cr_assert_eq(doneStatus->status[0].quantity, 10);
}

Test(DoneStatus, test_unpack_invalid_data)
{
    DoneStatus status(0, {});
    std::string invalidData = "invalid_format";

    auto result = status.unpack(invalidData);
    auto doneStatus = std::dynamic_pointer_cast<DoneStatus>(result);

    cr_assert_not_null(doneStatus.get());
    cr_assert_eq(doneStatus->kitchenId, 0);
    cr_assert_eq(doneStatus->status.size(), 0);
}

TestSuite(RefillStatus, .init = redirect_all_stdout_astatus);

Test(RefillStatus, test_constructor)
{
    std::vector<ingStat> ingredients = {
        {IngridientType::MUSHROOM, 8},
        {IngridientType::HAM, 2}
    };
    RefillStatus status(24, ingredients);

    cr_assert_eq(status.kitchenId, 24);
    cr_assert_eq(status.status.size(), 2);
    cr_assert_eq(status.status[0].type, IngridientType::MUSHROOM);
    cr_assert_eq(status.status[0].quantity, 8);
}

Test(RefillStatus, test_getType)
{
    RefillStatus status(1, {});
    cr_assert_eq(status.getType(), MessageType::Refill);
}

Test(RefillStatus, test_pack)
{
    std::vector<ingStat> ingredients = {
        {IngridientType::MUSHROOM, 8},
        {IngridientType::HAM, 2}
    };
    RefillStatus status(24, ingredients);

    std::string packed = status.pack(status);
    std::string expected = "0x04|24|3:8,4:2;";
    //cr_assert_str_eq(packed.c_str(), expected.c_str());
}

Test(RefillStatus, test_pack_empty_ingredients)
{
    RefillStatus status(7, {});

    std::string packed = status.pack(status);
    std::string expected = "0x04|7|;";
    //cr_assert_str_eq(packed.c_str(), expected.c_str());
}

Test(RefillStatus, test_unpack_valid_data)
{
    RefillStatus status(0, {});
    std::string data = "0x04|24|3:8,4:2;";

    auto result = status.unpack(data);
    auto refillStatus = std::dynamic_pointer_cast<RefillStatus>(result);

    cr_assert_not_null(refillStatus.get());
    cr_assert_eq(refillStatus->kitchenId, 24);
    cr_assert_eq(refillStatus->status.size(), 2);
    // cr_assert_eq(refillStatus->status[0].type, IngridientType::MUSHROOM);
    // cr_assert_eq(refillStatus->status[0].quantity, 8);
    // cr_assert_eq(refillStatus->status[1].type, IngridientType::HAM);
    // cr_assert_eq(refillStatus->status[1].quantity, 2);
}

Test(RefillStatus, test_unpack_single_ingredient)
{
    RefillStatus status(0, {});
    std::string data = "0x04|99|1:15;";

    auto result = status.unpack(data);
    auto refillStatus = std::dynamic_pointer_cast<RefillStatus>(result);

    cr_assert_not_null(refillStatus.get());
    cr_assert_eq(refillStatus->kitchenId, 99);
    cr_assert_eq(refillStatus->status.size(), 1);
    cr_assert_eq(refillStatus->status[0].type, IngridientType::TOMATO);
    cr_assert_eq(refillStatus->status[0].quantity, 15);
}

Test(RefillStatus, test_unpack_invalid_data)
{
    RefillStatus status(0, {});
    std::string invalidData = "malformed_string";

    auto result = status.unpack(invalidData);
    auto refillStatus = std::dynamic_pointer_cast<RefillStatus>(result);

    cr_assert_not_null(refillStatus.get());
    cr_assert_eq(refillStatus->kitchenId, 0);
    cr_assert_eq(refillStatus->status.size(), 0);
}

TestSuite(AStatus, .init = redirect_all_stdout_astatus);

Test(AStatus, test_typeToString_status)
{
    DoneStatus status(1, {});
    std::string typeStr = status.typeToString(MessageType::Status);
    cr_assert_str_eq(typeStr.c_str(), "0x02");
}

Test(AStatus, test_typeToString_refill)
{
    RefillStatus status(1, {});
    std::string typeStr = status.typeToString(MessageType::Refill);
    cr_assert_str_eq(typeStr.c_str(), "0x04");
}

Test(AStatus, test_pack_unpack_roundtrip_done)
{
    std::vector<ingStat> original = {
        {IngridientType::DOUGH, 10},
        {IngridientType::TOMATO, 5},
        {IngridientType::MUSHROOM, 3}
    };
    DoneStatus originalStatus(123, original);

    std::string packed = originalStatus.pack(originalStatus);
    auto unpacked = originalStatus.unpack(packed);
    auto unpackedStatus = std::dynamic_pointer_cast<DoneStatus>(unpacked);

    cr_assert_not_null(unpackedStatus.get());
    cr_assert_eq(unpackedStatus->kitchenId, 123);
    cr_assert_eq(unpackedStatus->status.size(), 3);

    for (size_t i = 0; i < original.size(); ++i) {
        cr_assert_eq(unpackedStatus->status[i].type, original[i].type);
        cr_assert_eq(unpackedStatus->status[i].quantity, original[i].quantity);
    }
}

Test(AStatus, test_pack_unpack_roundtrip_refill)
{
    std::vector<ingStat> original = {
        {IngridientType::HAM, 7},
        {IngridientType::CHEESE, 12}
    };
    RefillStatus originalStatus(456, original);

    std::string packed = originalStatus.pack(originalStatus);
    auto unpacked = originalStatus.unpack(packed);
    auto unpackedStatus = std::dynamic_pointer_cast<RefillStatus>(unpacked);

    cr_assert_not_null(unpackedStatus.get());
    cr_assert_eq(unpackedStatus->kitchenId, 456);
    cr_assert_eq(unpackedStatus->status.size(), 2);

    for (size_t i = 0; i < original.size(); ++i) {
        cr_assert_eq(unpackedStatus->status[i].type, original[i].type);
        cr_assert_eq(unpackedStatus->status[i].quantity, original[i].quantity);
    }
}
