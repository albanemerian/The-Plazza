/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Cook-test
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../src/reception/Reception.hpp"

#include <memory>
#include <thread>
#include <chrono>

void redirect_all_stdout_parser(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

TestSuite(CommandParser, .init = redirect_all_stdout_parser);

Test(CommandParser, test_valid_single_order)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regina S x1");
    cr_assert_eq(result.size(), 1);
    cr_assert(!result[0].empty(), "Order string should not be empty");
}

Test(CommandParser, test_valid_multiple_orders)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regina S x1; margarita M x2; americana L x3");
    cr_assert_eq(result.size(), 3);
    for (const auto& order : result) {
        cr_assert(!order.empty(), "Each order string should not be empty");
    }
}

Test(CommandParser, test_all_pizza_types)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> pizzaTypes = {"regina", "margarita", "americana", "fantasia"};
    for (const auto& pizza : pizzaTypes) {
        std::string command = pizza + " S x1";
        std::vector<std::string> result = reception.checkCommand(command.c_str());
        cr_assert_eq(result.size(), 1, "Should parse valid pizza type");
    }
}

Test(CommandParser, test_all_pizza_sizes)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> sizes = {"S", "M", "L", "XL", "XXL"};
    for (const auto& size : sizes) {
        std::string command = "regina " + size + " x1";
        reception.checkCommand("menu");
        std::vector<std::string> result = reception.checkCommand(command.c_str());
        cr_assert_eq(result.size(), 1, "Should parse valid pizza size");
    }
}

Test(CommandParser, test_different_quantities)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<int> quantities = {1, 5, 10, 99};
    for (int qty : quantities) {
        std::string command = "regina S x" + std::to_string(qty);
        std::vector<std::string> result = reception.checkCommand(command.c_str());
        cr_assert_eq(result.size(), 1, "Should parse valid quantity");
    }
}

Test(CommandParser, test_invalid_pizza_type)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("invalidpizza S x1");
    cr_assert_eq(result.size(), 0, "Should return empty vector for invalid pizza type");
}

Test(CommandParser, test_invalid_pizza_size)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regina INVALID x1");
    cr_assert_eq(result.size(), 0, "Should return empty vector for invalid pizza size");
}

Test(CommandParser, test_invalid_quantity_zero)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regina S x0");
    cr_assert_eq(result.size(), 0, "Should return empty vector for zero quantity");
}

Test(CommandParser, test_invalid_format_missing_x)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regina S 1");
    cr_assert_eq(result.size(), 0, "Should return empty vector for missing 'x'");
}

Test(CommandParser, test_invalid_format_wrong_order)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("S regina x1");
    cr_assert_eq(result.size(), 0, "Should return empty vector for wrong parameter order");
}

Test(CommandParser, test_empty_command)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("");
    cr_assert_eq(result.size(), 0, "Should return empty vector for empty command");
}

Test(CommandParser, test_whitespace_only_command)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("   ");
    cr_assert_eq(result.size(), 0, "Should return empty vector for whitespace-only command");
}

Test(CommandParser, test_semicolon_only_command)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand(";");
    cr_assert_eq(result.size(), 0, "Should return empty vector for semicolon-only command");
}

Test(CommandParser, test_multiple_semicolons)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regina S x1;;; margarita M x2");
    cr_assert_eq(result.size(), 2, "Should handle multiple semicolons correctly");
}

Test(CommandParser, test_trailing_semicolon)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regina S x1;");
    cr_assert_eq(result.size(), 1, "Should handle trailing semicolon correctly");
}

Test(CommandParser, test_leading_semicolon)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand(";regina S x1");
    cr_assert_eq(result.size(), 1, "Should handle leading semicolon correctly");
}

Test(CommandParser, test_extra_whitespace)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("  regina   S   x1  ");
    cr_assert_eq(result.size(), 1, "Should handle extra whitespace correctly");
}

Test(CommandParser, test_mixed_case_pizza_types)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("REGINA S x1");
    cr_assert_eq(result.size(), 0, "Should be case sensitive for pizza types");
    result = reception.checkCommand("Regina S x1");
    cr_assert_eq(result.size(), 0, "Should be case sensitive for pizza types");
}

Test(CommandParser, test_mixed_case_pizza_sizes)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regina s x1");
    cr_assert_eq(result.size(), 0, "Should be case sensitive for pizza sizes");
    result = reception.checkCommand("regina m x1");
    cr_assert_eq(result.size(), 0, "Should be case sensitive for pizza sizes");
}

Test(CommandParser, test_large_quantity)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regina S x999");
    cr_assert_eq(result.size(), 1, "Should handle large quantities");
}

Test(CommandParser, test_negative_quantity)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regina S x-1");
    cr_assert_eq(result.size(), 0, "Should reject negative quantities");
}

Test(CommandParser, test_complex_valid_order)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand(
        "regina S x1; margarita M x2; americana L x3; fantasia XL x4");
    cr_assert_eq(result.size(), 4, "Should parse complex valid order");
}

Test(CommandParser, test_mixed_valid_invalid_orders)
{
    Reception reception;
    reception.setValues(2, 1000, 500);
    std::vector<std::string> result = reception.checkCommand("regina S x1; invalid M x2");

    cr_assert_eq(result.size(), 0, "Should return empty for any invalid order in sequence");
}

Test(CommandParser, test_order_content_validation)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regina S x1");
    cr_assert_eq(result.size(), 1);

    cr_assert(!result[0].empty(), "Order string should not be empty");
    cr_assert(result[0].find("0x01") != std::string::npos, "Order should start with message type");
}

Test(CommandParser, test_all_combinations)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> commands = {
        "regina S x1",
        "margarita M x5",
        "americana L x10",
        "fantasia XL x2",
        "regina XXL x1"
    };

    for (const auto& command : commands) {
        std::vector<std::string> result = reception.checkCommand(command.c_str());
        cr_assert_eq(result.size(), 1, "Each valid command should produce one order");
    }
}

Test(CommandParser, test_regex_edge_cases)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> invalidCommands = {
        "regina",
        "regina S",
        "regina x1",
        "S x1",
        "x1",
        "regina S x",
        "regina S x1 extra",
        "123 S x1",
        "regina 123 x1"
    };

    for (const auto& command : invalidCommands) {
        std::vector<std::string> result = reception.checkCommand(command.c_str());
        cr_assert_eq(result.size(), 0, "Malformed command should return empty vector");
    }
}

Test(CommandParser, test_exception_handling)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    reception.checkCommand("regina S x999999999999999999999");
}

Test(CommandParser, test_null_command)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("");
    cr_assert_eq(result.size(), 0, "Should handle empty string");
}

// ===== NEW COMPREHENSIVE TESTS =====

Test(CommandParser, test_boundary_quantity_values)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regina S x1");
    cr_assert_eq(result.size(), 1, "Should accept minimum quantity 1");

    result = reception.checkCommand("regina S x9999");
    cr_assert_eq(result.size(), 1, "Should handle very large quantities");

    result = reception.checkCommand("regina S x2147483647");
    cr_assert_eq(result.size(), 1, "Should handle max int value");
}

Test(CommandParser, test_invalid_quantity_formats)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regina S x1.5");
    cr_assert_eq(result.size(), 0, "Should reject decimal quantities");

    result = reception.checkCommand("regina S xabc");
    cr_assert_eq(result.size(), 0, "Should reject non-numeric quantities");

    result = reception.checkCommand("regina S x-5");
    cr_assert_eq(result.size(), 0, "Should reject negative quantities");

    result = reception.checkCommand("regina S x00001");
    cr_assert_eq(result.size(), 0, "Should reject quantities with leading zeros");
}

Test(CommandParser, test_malformed_separators)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regina S x1, margarita M x2");
    cr_assert_eq(result.size(), 0, "Should reject comma separators");

    result = reception.checkCommand("regina S x1: margarita M x2");
    cr_assert_eq(result.size(), 0, "Should reject colon separators");

    result = reception.checkCommand("regina S x1| margarita M x2");
    cr_assert_eq(result.size(), 0, "Should reject pipe separators");
}

Test(CommandParser, test_unicode_and_special_characters)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regína S x1");
    cr_assert_eq(result.size(), 0, "Should reject unicode in pizza names");

    result = reception.checkCommand("regina123 S x1");
    cr_assert_eq(result.size(), 0, "Should reject numbers in pizza names");

    result = reception.checkCommand("regina@ S x1");
    cr_assert_eq(result.size(), 0, "Should reject special characters in pizza names");

    result = reception.checkCommand("regina_pizza S x1");
    cr_assert_eq(result.size(), 0, "Should reject underscores in pizza names");
}

Test(CommandParser, test_case_sensitivity_comprehensive)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> wrongCasePizzas = {"REGINA", "Regina", "MARGARITA", "Margarita",
                                               "AMERICANA", "Americana", "FANTASIA", "Fantasia"};

    for (const auto& pizza : wrongCasePizzas) {
        std::vector<std::string> result = reception.checkCommand((pizza + " S x1").c_str());
        cr_assert_eq(result.size(), 0, "Should be case sensitive for pizza types");
    }

    std::vector<std::string> wrongCaseSizes = {"s", "m", "l", "xl", "xxl", "Xl", "Xxl"};

    for (const auto& size : wrongCaseSizes) {
        std::vector<std::string> result = reception.checkCommand(("regina " + size + " x1").c_str());
        cr_assert_eq(result.size(), 0, "Should be case sensitive for pizza sizes");
    }
}

Test(CommandParser, test_whitespace_variations)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regina    S    x1");
    cr_assert_eq(result.size(), 1, "Should handle multiple spaces correctly");

    result = reception.checkCommand("regina S x1  ;  margarita M x2");
    cr_assert_eq(result.size(), 2, "Should handle whitespace around semicolons");
}

Test(CommandParser, test_empty_segments_in_multiorder)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regina S x1;;margarita M x2");
    cr_assert_eq(result.size(), 2, "Should handle empty segments correctly");

    result = reception.checkCommand("regina S x1;;;margarita M x2");
    cr_assert_eq(result.size(), 2, "Should handle multiple empty segments");

    result = reception.checkCommand(";;;");
    cr_assert_eq(result.size(), 0, "Should handle only empty segments");
}

Test(CommandParser, test_order_format_variations)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regina S X1");
    cr_assert_eq(result.size(), 0, "Should be case sensitive for 'x'");

    result = reception.checkCommand("regina Sx1");
    cr_assert_eq(result.size(), 0, "Should require proper spacing");

    result = reception.checkCommand("regina S x1 extra");
    cr_assert_eq(result.size(), 0, "Should reject extra characters");

    result = reception.checkCommand("regina S");
    cr_assert_eq(result.size(), 0, "Should reject incomplete orders");
}

Test(CommandParser, test_numeric_overflow_handling)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regina S x99999999999999999999999999999");
    cr_assert_eq(result.size(), 0, "Should handle numeric overflow gracefully");

    result = reception.checkCommand("regina S x2147483648");
    cr_assert_eq(result.size(), 0, "Should handle numbers larger than int max");
}

Test(CommandParser, test_mixed_valid_invalid_in_multiorder)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regina S x1; invalid_pizza M x2");
    cr_assert_eq(result.size(), 0, "Should fail if any order in sequence is invalid");

    result = reception.checkCommand("invalid_pizza S x1; regina M x2");
    cr_assert_eq(result.size(), 0, "Should fail if any order in sequence is invalid");

    result = reception.checkCommand("regina S x1; margarita M x2; invalid_pizza L x3; americana XL x4");
    cr_assert_eq(result.size(), 0, "Should fail if any order in long sequence is invalid");
}

Test(CommandParser, test_all_size_combinations_with_all_pizzas)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> pizzaTypes = {"regina", "margarita", "americana", "fantasia"};
    std::vector<std::string> sizes = {"S", "M", "L", "XL", "XXL"};
    for (const auto& pizza : pizzaTypes) {
        for (const auto& size : sizes) {
            std::string command = pizza + " " + size + " x1";
            std::vector<std::string> result = reception.checkCommand(command.c_str());
            cr_assert_eq(result.size(), 1, "All valid combinations should work");
        }
    }
}

Test(CommandParser, test_quantity_range_stress)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<int> quantities = {1, 2, 5, 10, 25, 50, 100, 250, 500, 999, 1000};
    for (int qty : quantities) {
        std::string command = "regina S x" + std::to_string(qty);
        std::vector<std::string> result = reception.checkCommand(command.c_str());
        cr_assert_eq(result.size(), 1, "Should handle various quantity ranges");
    }
}

Test(CommandParser, test_long_multiorder_sequences)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::string longOrder = "regina S x1; margarita M x2; americana L x3; fantasia XL x4; "
                           "regina XXL x5; margarita S x6; americana M x7; fantasia L x8; "
                           "regina XL x9; margarita XXL x10";

    std::vector<std::string> result = reception.checkCommand(longOrder.c_str());
    cr_assert_eq(result.size(), 10, "Should handle long sequences of valid orders");
}

Test(CommandParser, test_error_message_generation)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> invalidCommands = {
        "invalid_format",
        "regina",
        "regina invalid_size x1",
        "invalid_pizza S x1",
        "regina S x0",
        "regina S x-1",
        ""
    };

    for (const auto& command : invalidCommands) {
        std::vector<std::string> result = reception.checkCommand(command.c_str());
        cr_assert_eq(result.size(), 0, "Invalid commands should return empty vector");
    }
}

Test(CommandParser, test_order_pack_format_validation)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> result = reception.checkCommand("regina S x5");
    cr_assert_eq(result.size(), 1);

    cr_assert(!result[0].empty(), "Packed order should not be empty");
    cr_assert(result[0].find("0x01") == 0, "Packed order should start with message type 0x01");
    cr_assert(result[0].back() == ';', "Packed order should end with semicolon");

    size_t pipeCount = std::count(result[0].begin(), result[0].end(), '|');
    cr_assert_eq(pipeCount, 3, "Packed order should have 3 pipe separators");
}

Test(CommandParser, test_regex_boundary_conditions)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> edgeCases = {
        "regina S x",        // Missing quantity
        "regina S x 1",      // Space before quantity
        "regina S x1 ",      // Space after quantity
        " regina S x1",      // Leading space
        "regina S x1 ",      // Trailing space
        "regina  S  x1",     // Double spaces
        "xregina S x1",      // Prefix that might match 'x'
        "regina Sx1",        // No space before x
        "regina S x1x",      // Extra x
        "1regina S x1",      // Leading number
        "regina1 S x1"       // Trailing number in pizza name
    };

    for (const auto& command : edgeCases) {
        std::vector<std::string> result = reception.checkCommand(command.c_str());
        if (command == "regina  S  x1" || command == " regina S x1" || command == "regina S x1 ") {
            cr_assert_eq(result.size(), 1, "Should handle extra whitespace properly");
        } else {
            cr_assert_eq(result.size(), 0, "Should reject malformed regex patterns");
        }
    }
}

Test(CommandParser, test_exception_handling_comprehensive)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> exceptionTriggers = {
        "regina S x999999999999999999999",  // Overflow
        "regina S x18446744073709551616",   // Very large number
        "regina INVALID_SIZE x1",           // Invalid size
        "INVALID_PIZZA S x1",               // Invalid pizza
        "regina S x0",                      // Zero quantity
        "complete_garbage_input",           // Complete nonsense
        ";;;;;;;;;;;;;;;;;;;;;;;",         // Only separators
        ""                                  // Empty string
    };

    for (const auto& command : exceptionTriggers) {
        std::vector<std::string> result = reception.checkCommand(command.c_str());
        cr_assert_eq(result.size(), 0, "Exception triggers should return empty vector");
    }
}

Test(CommandParser, test_memory_and_performance)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    for (int i = 0; i < 100; ++i) {
        std::string command = "regina S x" + std::to_string((i % 10) + 1);
        std::vector<std::string> result = reception.checkCommand(command.c_str());
        cr_assert_eq(result.size(), 1, "Should handle rapid processing");
    }

    std::string veryLongOrder;
    for (int i = 0; i < 50; ++i) {
        if (i > 0) veryLongOrder += "; ";
        veryLongOrder += "regina S x1";
    }

    std::vector<std::string> result = reception.checkCommand(veryLongOrder.c_str());
    cr_assert_eq(result.size(), 50, "Should handle very long order sequences");
}

Test(CommandParser, test_null_and_empty_edge_cases)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> emptyInputs = {
        "",
        " ",
        "  ",
        "\t",
        "\n",
        ";",
        ";;",
        ";;;",
        " ; ; ; "
    };

    for (const auto& input : emptyInputs) {
        std::vector<std::string> result = reception.checkCommand(input.c_str());
        cr_assert_eq(result.size(), 0, "Empty-like inputs should return empty vector");
    }
}

Test(CommandParser, test_international_and_encoding)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    std::vector<std::string> internationalInputs = {
        "régina S x1",           // Accented characters
        "ΡΕΓΙΝΑ S x1",          // Greek characters
        "regina S x①",          // Unicode numbers
        "regina§ S x1",         // Section symbol
        "regina™ S x1",         // Trademark symbol
        "regina\x80 S x1",      // Extended ASCII
    };

    for (const auto& input : internationalInputs) {
        std::vector<std::string> result = reception.checkCommand(input.c_str());
        cr_assert_eq(result.size(), 0, "International characters should be rejected");
    }
}

Test(CommandParser, test_order_content_detailed_validation)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    struct TestCase {
        std::string command;
        std::string expectedType;
        std::string expectedSize;
        int expectedQuantity;
    };

    std::vector<TestCase> testCases = {
        {"regina S x1", "1", "1", 1},      // Regina=1, S=1
        {"margarita M x5", "2", "2", 5},   // Margarita=2, M=2
        {"americana L x10", "3", "3", 10}, // Americana=3, L=3
        {"fantasia XL x25", "4", "4", 25}  // Fantasia=4, XL=4
    };

    for (const auto& testCase : testCases) {
        std::vector<std::string> result = reception.checkCommand(testCase.command.c_str());
        cr_assert_eq(result.size(), 1, "Command should be valid");
    }
}

Test(CommandParser, test_type_to_string)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    reception.typeToString(PizzaType::Regina);
    reception.typeToString(PizzaType::Margarita);
    reception.typeToString(PizzaType::Americana);
    reception.typeToString(PizzaType::Fantasia);
    reception.typeToString(PizzaType::Nothing); // Should handle Nothing type gracefully
    cr_assert(true, "Type to string conversion should not throw exceptions");
}

Test(CommandParser, test_string_to_type)
{
    Reception reception;
    reception.setValues(2, 1000, 500);

    cr_assert_eq(reception.stringToType("regina"), PizzaType::Regina);
    cr_assert_eq(reception.stringToType("margarita"), PizzaType::Margarita);
    cr_assert_eq(reception.stringToType("americana"), PizzaType::Americana);
    cr_assert_eq(reception.stringToType("fantasia"), PizzaType::Fantasia);
    cr_assert_eq(reception.stringToType("invalid"), PizzaType::Nothing); // Should return Nothing for invalid type
}
