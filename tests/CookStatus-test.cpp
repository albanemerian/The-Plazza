/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** CookStatus-test
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../common/messages/CookStatus.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <memory>

void redirect_all_stdout_cookstatus(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

TestSuite(CookStatus, .init = redirect_all_stdout_cookstatus);

Test(CookStatus, test_constructor_empty)
{
    std::vector<CookStatusData> cooks = {};
    CookStatus status(42, cooks);

    cr_assert_eq(status._kitchenId, 42);
    cr_assert_eq(status._cooksStatus.size(), 0);
}

Test(CookStatus, test_constructor_single_cook)
{
    std::vector<CookStatusData> cooks = {
        {1, true, false}
    };
    CookStatus status(10, cooks);

    cr_assert_eq(status._kitchenId, 10);
    cr_assert_eq(status._cooksStatus.size(), 1);
    cr_assert_eq(status._cooksStatus[0].cookId, 1);
    cr_assert_eq(status._cooksStatus[0].isBusy, true);
    cr_assert_eq(status._cooksStatus[0].isRestocking, false);
}

Test(CookStatus, test_constructor_multiple_cooks)
{
    std::vector<CookStatusData> cooks = {
        {1, true, false},
        {2, false, true},
        {3, false, false},
        {4, true, true}
    };
    CookStatus status(25, cooks);

    cr_assert_eq(status._kitchenId, 25);
    cr_assert_eq(status._cooksStatus.size(), 4);
    
    cr_assert_eq(status._cooksStatus[0].cookId, 1);
    cr_assert_eq(status._cooksStatus[0].isBusy, true);
    cr_assert_eq(status._cooksStatus[0].isRestocking, false);
    
    cr_assert_eq(status._cooksStatus[1].cookId, 2);
    cr_assert_eq(status._cooksStatus[1].isBusy, false);
    cr_assert_eq(status._cooksStatus[1].isRestocking, true);
    
    cr_assert_eq(status._cooksStatus[2].cookId, 3);
    cr_assert_eq(status._cooksStatus[2].isBusy, false);
    cr_assert_eq(status._cooksStatus[2].isRestocking, false);
    
    cr_assert_eq(status._cooksStatus[3].cookId, 4);
    cr_assert_eq(status._cooksStatus[3].isBusy, true);
    cr_assert_eq(status._cooksStatus[3].isRestocking, true);
}

Test(CookStatus, test_getType)
{
    CookStatus status(1, {});
    cr_assert_eq(status.getType(), MessageType::CookStatus);
}

Test(CookStatus, test_typeToString)
{
    CookStatus status(1, {});
    std::string typeStr = status.typeToString(MessageType::CookStatus);
    cr_assert_str_eq(typeStr.c_str(), "0x06");
}

Test(CookStatus, test_pack_empty_cooks)
{
    CookStatus status(42, {});
    
    std::string packed = status.pack(status);
    std::string expected = "0x06|42|;";
    cr_assert_str_eq(packed.c_str(), expected.c_str());
}

Test(CookStatus, test_pack_single_cook_busy_not_restocking)
{
    std::vector<CookStatusData> cooks = {
        {5, true, false}
    };
    CookStatus status(10, cooks);
    
    std::string packed = status.pack(status);
    std::string expected = "0x06|10|5:1:0|;";
    cr_assert_str_eq(packed.c_str(), expected.c_str());
}

Test(CookStatus, test_pack_single_cook_not_busy_restocking)
{
    std::vector<CookStatusData> cooks = {
        {3, false, true}
    };
    CookStatus status(15, cooks);
    
    std::string packed = status.pack(status);
    std::string expected = "0x06|15|3:0:1|;";
    cr_assert_str_eq(packed.c_str(), expected.c_str());
}

Test(CookStatus, test_pack_single_cook_busy_and_restocking)
{
    std::vector<CookStatusData> cooks = {
        {7, true, true}
    };
    CookStatus status(20, cooks);
    
    std::string packed = status.pack(status);
    std::string expected = "0x06|20|7:1:1|;";
    cr_assert_str_eq(packed.c_str(), expected.c_str());
}

Test(CookStatus, test_pack_single_cook_not_busy_not_restocking)
{
    std::vector<CookStatusData> cooks = {
        {9, false, false}
    };
    CookStatus status(30, cooks);
    
    std::string packed = status.pack(status);
    std::string expected = "0x06|30|9:0:0|;";
    cr_assert_str_eq(packed.c_str(), expected.c_str());
}

Test(CookStatus, test_pack_multiple_cooks)
{
    std::vector<CookStatusData> cooks = {
        {1, true, false},
        {2, false, true},
        {3, true, true},
        {4, false, false}
    };
    CookStatus status(50, cooks);
    
    std::string packed = status.pack(status);
    std::string expected = "0x06|50|1:1:0|2:0:1|3:1:1|4:0:0|;";
    cr_assert_str_eq(packed.c_str(), expected.c_str());
}

Test(CookStatus, test_unpack_valid_data_empty_cooks)
{
    CookStatus status(0, {});
    std::string data = "0x06|42|;";
    
    auto result = status.unpack(data);
    auto cookStatus = std::dynamic_pointer_cast<CookStatus>(result);
    
    cr_assert_not_null(cookStatus.get());
    cr_assert_eq(cookStatus->_kitchenId, 42);
    cr_assert_eq(cookStatus->_cooksStatus.size(), 0);
}

Test(CookStatus, test_unpack_valid_data_single_cook)
{
    CookStatus status(0, {});
    std::string data = "0x06|10|5:1:0|;";
    
    auto result = status.unpack(data);
    auto cookStatus = std::dynamic_pointer_cast<CookStatus>(result);
    
    cr_assert_not_null(cookStatus.get());
    cr_assert_eq(cookStatus->_kitchenId, 10);
    cr_assert_eq(cookStatus->_cooksStatus.size(), 1);
    cr_assert_eq(cookStatus->_cooksStatus[0].cookId, 5);
    cr_assert_eq(cookStatus->_cooksStatus[0].isBusy, true);
    cr_assert_eq(cookStatus->_cooksStatus[0].isRestocking, false);
}

Test(CookStatus, test_unpack_valid_data_multiple_cooks)
{
    CookStatus status(0, {});
    std::string data = "0x06|25|1:1:0|2:0:1|3:1:1|4:0:0|;";
    
    auto result = status.unpack(data);
    auto cookStatus = std::dynamic_pointer_cast<CookStatus>(result);
    
    cr_assert_not_null(cookStatus.get());
    cr_assert_eq(cookStatus->_kitchenId, 25);
    cr_assert_eq(cookStatus->_cooksStatus.size(), 4);
    
    cr_assert_eq(cookStatus->_cooksStatus[0].cookId, 1);
    cr_assert_eq(cookStatus->_cooksStatus[0].isBusy, true);
    cr_assert_eq(cookStatus->_cooksStatus[0].isRestocking, false);
    
    cr_assert_eq(cookStatus->_cooksStatus[1].cookId, 2);
    cr_assert_eq(cookStatus->_cooksStatus[1].isBusy, false);
    cr_assert_eq(cookStatus->_cooksStatus[1].isRestocking, true);
    
    cr_assert_eq(cookStatus->_cooksStatus[2].cookId, 3);
    cr_assert_eq(cookStatus->_cooksStatus[2].isBusy, true);
    cr_assert_eq(cookStatus->_cooksStatus[2].isRestocking, true);
    
    cr_assert_eq(cookStatus->_cooksStatus[3].cookId, 4);
    cr_assert_eq(cookStatus->_cooksStatus[3].isBusy, false);
    cr_assert_eq(cookStatus->_cooksStatus[3].isRestocking, false);
}

Test(CookStatus, test_unpack_without_semicolon)
{
    CookStatus status(0, {});
    std::string data = "0x06|15|3:0:1|";
    
    auto result = status.unpack(data);
    auto cookStatus = std::dynamic_pointer_cast<CookStatus>(result);
    
    cr_assert_not_null(cookStatus.get());
    cr_assert_eq(cookStatus->_kitchenId, 15);
    cr_assert_eq(cookStatus->_cooksStatus.size(), 1);
    cr_assert_eq(cookStatus->_cooksStatus[0].cookId, 3);
    cr_assert_eq(cookStatus->_cooksStatus[0].isBusy, false);
    cr_assert_eq(cookStatus->_cooksStatus[0].isRestocking, true);
}

Test(CookStatus, test_unpack_invalid_format)
{
    CookStatus status(0, {});
    std::string invalidData = "invalid_format";
    
    auto result = status.unpack(invalidData);
    auto cookStatus = std::dynamic_pointer_cast<CookStatus>(result);
    
    cr_assert_not_null(cookStatus.get());
    cr_assert_eq(cookStatus->_kitchenId, 0);
    cr_assert_eq(cookStatus->_cooksStatus.size(), 0);
}

Test(CookStatus, test_unpack_wrong_message_type)
{
    CookStatus status(0, {});
    std::string data = "0x02|42|1:1:0|;";
    
    auto result = status.unpack(data);
    auto cookStatus = std::dynamic_pointer_cast<CookStatus>(result);
    
    cr_assert_not_null(cookStatus.get());
    cr_assert_eq(cookStatus->_kitchenId, 0);
    cr_assert_eq(cookStatus->_cooksStatus.size(), 0);
}

Test(CookStatus, test_unpack_malformed_cook_data)
{
    CookStatus status(0, {});
    std::string data = "0x06|30|1:1|2:0:1:extra|3:x:y|;";
    
    auto result = status.unpack(data);
    auto cookStatus = std::dynamic_pointer_cast<CookStatus>(result);
    
    cr_assert_not_null(cookStatus.get());
    cr_assert_eq(cookStatus->_kitchenId, 30);
    // Only valid cook entries should be parsed
    cr_assert_eq(cookStatus->_cooksStatus.size(), 1);
    cr_assert_eq(cookStatus->_cooksStatus[0].cookId, 2);
    cr_assert_eq(cookStatus->_cooksStatus[0].isBusy, false);
    cr_assert_eq(cookStatus->_cooksStatus[0].isRestocking, true);
}

Test(CookStatus, test_unpack_large_cook_ids)
{
    CookStatus status(0, {});
    std::string data = "0x06|999|123:1:0|456:0:1|789:1:1|;";
    
    auto result = status.unpack(data);
    auto cookStatus = std::dynamic_pointer_cast<CookStatus>(result);
    
    cr_assert_not_null(cookStatus.get());
    cr_assert_eq(cookStatus->_kitchenId, 999);
    cr_assert_eq(cookStatus->_cooksStatus.size(), 3);
    cr_assert_eq(cookStatus->_cooksStatus[0].cookId, 123);
    cr_assert_eq(cookStatus->_cooksStatus[1].cookId, 456);
    cr_assert_eq(cookStatus->_cooksStatus[2].cookId, 789);
}

Test(CookStatus, test_pack_unpack_roundtrip_empty)
{
    std::vector<CookStatusData> original = {};
    CookStatus originalStatus(100, original);
    
    std::string packed = originalStatus.pack(originalStatus);
    auto unpacked = originalStatus.unpack(packed);
    auto unpackedStatus = std::dynamic_pointer_cast<CookStatus>(unpacked);
    
    cr_assert_not_null(unpackedStatus.get());
    cr_assert_eq(unpackedStatus->_kitchenId, 100);
    cr_assert_eq(unpackedStatus->_cooksStatus.size(), 0);
}

Test(CookStatus, test_pack_unpack_roundtrip_multiple)
{
    std::vector<CookStatusData> original = {
        {10, true, false},
        {20, false, true},
        {30, true, true},
        {40, false, false}
    };
    CookStatus originalStatus(200, original);
    
    std::string packed = originalStatus.pack(originalStatus);
    auto unpacked = originalStatus.unpack(packed);
    auto unpackedStatus = std::dynamic_pointer_cast<CookStatus>(unpacked);
    
    cr_assert_not_null(unpackedStatus.get());
    cr_assert_eq(unpackedStatus->_kitchenId, 200);
    cr_assert_eq(unpackedStatus->_cooksStatus.size(), 4);
    
    for (size_t i = 0; i < original.size(); ++i) {
        cr_assert_eq(unpackedStatus->_cooksStatus[i].cookId, original[i].cookId);
        cr_assert_eq(unpackedStatus->_cooksStatus[i].isBusy, original[i].isBusy);
        cr_assert_eq(unpackedStatus->_cooksStatus[i].isRestocking, original[i].isRestocking);
    }
}

Test(CookStatus, test_pack_unpack_roundtrip_edge_cases)
{
    std::vector<CookStatusData> original = {
        {0, false, false},
        {999999, true, true}
    };
    CookStatus originalStatus(0, original);
    
    std::string packed = originalStatus.pack(originalStatus);
    auto unpacked = originalStatus.unpack(packed);
    auto unpackedStatus = std::dynamic_pointer_cast<CookStatus>(unpacked);
    
    cr_assert_not_null(unpackedStatus.get());
    cr_assert_eq(unpackedStatus->_kitchenId, 0);
    cr_assert_eq(unpackedStatus->_cooksStatus.size(), 2);
    cr_assert_eq(unpackedStatus->_cooksStatus[0].cookId, 0);
    cr_assert_eq(unpackedStatus->_cooksStatus[1].cookId, 999999);
}

Test(CookStatus, test_typeToString_other_types)
{
    CookStatus status(1, {});
    cr_assert_str_eq(status.typeToString(MessageType::Order).c_str(), "0x01");
    cr_assert_str_eq(status.typeToString(MessageType::Status).c_str(), "0x02");
    cr_assert_str_eq(status.typeToString(MessageType::Inactivity).c_str(), "0x03");
    cr_assert_str_eq(status.typeToString(MessageType::Refill).c_str(), "0x04");
    cr_assert_str_eq(status.typeToString(MessageType::Queue).c_str(), "0x05");
}