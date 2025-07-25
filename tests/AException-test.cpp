/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** AException-test
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../common/AException.hpp"
#include <string>
#include <iostream>

void redirect_all_stdout_aexception(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(AException, constructor)
{
    const std::string type = "TestExceptionType";
    const std::string message = "Test exception message";

    AException exception(type, message);

}

Test(AException, get_type)
{
    const std::string type = "TestExceptionType";
    const std::string message = "Test exception message";

    AException exception(type, message);

}

Test(AException, get_message)
{
    const std::string type = "TestExceptionType";
    const std::string message = "Test exception message";

    AException exception(type, message);

}

Test(AException, get_formatted_message)
{
    const std::string type = "TestExceptionType";
    const std::string message = "Test exception message";
    const std::string expected = "\033[1;31m[TestExceptionType]\033[0m Test exception message";

    AException exception(type, message);

}

Test(AException, what_method)
{
    const std::string type = "TestExceptionType";
    const std::string message = "Test exception message";
    const std::string expected = "\033[1;31m[TestExceptionType]\033[0m Test exception message";

    AException exception(type, message);

}

Test(AException, inheritance)
{
    const std::string type = "TestExceptionType";
    const std::string message = "Test exception message";

    AException exception(type, message);
    IException* basePtr = &exception;

}

Test(AException, different_instances)
{
    const std::string type1 = "ErrorType1";
    const std::string message1 = "First error message";
    const std::string type2 = "ErrorType2";
    const std::string message2 = "Second error message";

    AException exception1(type1, message1);
    AException exception2(type2, message2);

}
