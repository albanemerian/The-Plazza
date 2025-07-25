/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Inactivity-test
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../common/messages/Inactivity.hpp"

#include <string>
#include <iostream>

void redirect_all_stdout_inactivity(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(Inactivity, test_constructor)
{
    Inactivity inactivity(42);
    cr_assert_eq(inactivity.id, 42);
}

Test(Inactivity, test_pack)
{
    Inactivity inactivity(24);
    std::string packed = inactivity.pack(inactivity);
    std::string expected = "0x03|24;";
    cr_assert_str_eq(packed.c_str(), expected.c_str());
}

Test(Inactivity, test_unpack_valid_data)
{
    Inactivity inactivity(0);
    std::string data = "0x03|24;";

    auto result = inactivity.unpack(data);
    auto unpackedInactivity = std::dynamic_pointer_cast<Inactivity>(result);

    cr_assert_not_null(unpackedInactivity.get());
    cr_assert_eq(unpackedInactivity->id, 24);
}

Test(Inactivity, test_unpack_invalid_data)
{
    Inactivity inactivity(0);
    std::string invalidData = "malformed_string";

    auto result = inactivity.unpack(invalidData);
    auto unpackedInactivity = std::dynamic_pointer_cast<Inactivity>(result);

    cr_assert_not_null(unpackedInactivity.get());
    cr_assert_eq(unpackedInactivity->id, 0);
}

Test(Inactivity, test_getType)
{
    Inactivity inactivity(1);
    cr_assert_eq(inactivity.getType(), MessageType::Inactivity);
}

Test(Inactivity, test_typeToString)
{
    Inactivity inactivity(1);
    std::string typeStr = inactivity.typeToString(MessageType::Inactivity);
    cr_assert_str_eq(typeStr.c_str(), "0x03");
}

