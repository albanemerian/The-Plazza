/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Helper-test
*/


#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/utils/Utils.hpp"

void redirect_all_stdout(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(Utils, helper_output, .init = redirect_all_stdout) {
    Utils utils;
    utils.helper();
    cr_assert_stdout_eq_str("Usage: ./plazza <multiplier Cooking time> [-d] <number of cooks> <Restock time milli-second>\n"
                           "Options:\n"
                           "\t in the shell you can use : \n"
                           "\t\t - 'exit' to quit the program\n"
                           "\t\t - 'clear' to clear the screen\n"
                           "\t\t - 'menu' display the available order\n"
                           "\t\t - 'status' | 'S' to display the state of the restaurant\n");
}
