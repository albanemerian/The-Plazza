/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Ingridient-test
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../common/Ingridient.hpp"

void redirect_all_stdout_ingridient(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(Ingridient, constructor, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;

    cr_assert_eq(ingridient.getDough(), 5, "Constructor should set the dough correctly");
    cr_assert_eq(ingridient.getTomato(), 5, "Constructor should set the tomato correctly");
    cr_assert_eq(ingridient.getCheese(), 5, "Constructor should set the cheese correctly");
    cr_assert_eq(ingridient.getHam(), 5, "Constructor should set the ham correctly");
    cr_assert_eq(ingridient.getMushroom(), 5, "Constructor should set the mushroom correctly");
    cr_assert_eq(ingridient.getSteak(), 5, "Constructor should set the steak correctly");
    cr_assert_eq(ingridient.getEggplant(), 5, "Constructor should set the eggplant correctly");
    cr_assert_eq(ingridient.getGoatCheese(), 5, "Constructor should set the goat cheese correctly");
    cr_assert_eq(ingridient.getChefLove(), 5, "Constructor should set the chef love correctly");
    cr_assert_eq(ingridient.getEgg(), 5, "Constructor should set the egg correctly");
    cr_assert_eq(ingridient.getBacon(), 5, "Constructor should set the bacon correctly");
    cr_assert_eq(ingridient.getBasil(), 5, "Constructor should set the basil correctly");
    cr_assert_eq(ingridient.getPepper(), 5, "Constructor should set the pepper correctly");
}

Test(Ingridient, setDough, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;
    ingridient.setDough(10);

    cr_assert_eq(ingridient.getDough(), 10, "setDough should update the dough correctly");
}

Test(Ingridient, setTomato, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;
    ingridient.setTomato(10);

    cr_assert_eq(ingridient.getTomato(), 10, "setTomato should update the tomato correctly");
}

Test(Ingridient, setCheese, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;
    ingridient.setCheese(10);

    cr_assert_eq(ingridient.getCheese(), 10, "setCheese should update the cheese correctly");
}

Test(Ingridient, setHam, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;
    ingridient.setHam(10);

    cr_assert_eq(ingridient.getHam(), 10, "setHam should update the ham correctly");
}

Test(Ingridient, setMushroom, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;
    ingridient.setMushroom(10);

    cr_assert_eq(ingridient.getMushroom(), 10, "setMushroom should update the mushroom correctly");
}

Test(Ingridient, setSteak, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;
    ingridient.setSteak(10);

    cr_assert_eq(ingridient.getSteak(), 10, "setSteak should update the steak correctly");
}

Test(Ingridient, setEggplant, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;
    ingridient.setEggplant(10);

    cr_assert_eq(ingridient.getEggplant(), 10, "setEggplant should update the eggplant correctly");
}

Test(Ingridient, setGoatCheese, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;
    ingridient.setGoatCheese(10);

    cr_assert_eq(ingridient.getGoatCheese(), 10, "setGoatCheese should update the goat cheese correctly");
}

Test(Ingridient, setChefLove, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;
    ingridient.setChefLove(10);

    cr_assert_eq(ingridient.getChefLove(), 10, "setChefLove should update the chef love correctly");
}

Test(Ingridient, setEgg, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;
    ingridient.setEgg(10);

    cr_assert_eq(ingridient.getEgg(), 10, "setEgg should update the egg correctly");
}

Test(Ingridient, setBacon, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;
    ingridient.setBacon(10);

    cr_assert_eq(ingridient.getBacon(), 10, "setBacon should update the bacon correctly");
}

Test(Ingridient, setBasil, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;
    ingridient.setBasil(10);

    cr_assert_eq(ingridient.getBasil(), 10, "setBasil should update the basil correctly");
}

Test(Ingridient, setPepper, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;
    ingridient.setPepper(10);

    cr_assert_eq(ingridient.getPepper(), 10, "setPepper should update the pepper correctly");
}


Test(Ingridient, fridgeStatus, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;
    std::vector<ingStat> status = ingridient.fridgeStatus();

    cr_assert_eq(status.size(), 13, "fridgeStatus should return the correct size");
    cr_assert_eq(status[0].quantity, 5, "fridgeStatus should return the correct dough quantity");
    cr_assert_eq(status[1].quantity, 5, "fridgeStatus should return the correct tomato quantity");
    cr_assert_eq(status[2].quantity, 5, "fridgeStatus should return the correct cheese quantity");
    cr_assert_eq(status[3].quantity, 5, "fridgeStatus should return the correct ham quantity");
    cr_assert_eq(status[4].quantity, 5, "fridgeStatus should return the correct mushroom quantity");
    cr_assert_eq(status[5].quantity, 5, "fridgeStatus should return the correct steak quantity");
    cr_assert_eq(status[6].quantity, 5, "fridgeStatus should return the correct eggplant quantity");
    cr_assert_eq(status[7].quantity, 5, "fridgeStatus should return the correct goat cheese quantity");
    cr_assert_eq(status[8].quantity, 5, "fridgeStatus should return the correct chef love quantity");
    cr_assert_eq(status[9].quantity, 5, "fridgeStatus should return the correct egg quantity");
    cr_assert_eq(status[10].quantity, 5, "fridgeStatus should return the correct bacon quantity");
    cr_assert_eq(status[11].quantity, 5, "fridgeStatus should return the correct basil quantity");
    cr_assert_eq(status[12].quantity, 5, "fridgeStatus should return the correct pepper quantity");
}

Test(Ingridient, packIngredients, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;
    std::string packedData = ingridient.packIngredients();

    // cr_assert_str_eq(packedData.c_str(), "0:5|1:5|2:5|3:5|4:5|5:5|6:5|7:5|8:5|9:5|10:5|11:5|12:5|", "packIngredients should return the correct packed data");
}

Test(Ingridient, unpackIngredients, .init = redirect_all_stdout_ingridient) {
    std::string packedData = "0:5|1:5|2:5|3:5|4:5|5:5|6:5|7:5|8:5|9:5|10:5|11:5|12:5|";
    std::map<IngridientType, int> unpackedData = Ingridient::unpackIngredients(packedData);

    cr_assert_eq(unpackedData[DOUGH], 5, "unpackIngredients should return the correct dough quantity");
    cr_assert_eq(unpackedData[TOMATO], 5, "unpackIngredients should return the correct tomato quantity");
    cr_assert_eq(unpackedData[CHEESE], 5, "unpackIngredients should return the correct cheese quantity");
    cr_assert_eq(unpackedData[HAM], 5, "unpackIngredients should return the correct ham quantity");
    cr_assert_eq(unpackedData[MUSHROOM], 5, "unpackIngredients should return the correct mushroom quantity");
    cr_assert_eq(unpackedData[STEAK], 5, "unpackIngredients should return the correct steak quantity");
    cr_assert_eq(unpackedData[EGGPLANT], 5, "unpackIngredients should return the correct eggplant quantity");
    cr_assert_eq(unpackedData[GOAT_CHEESE], 5, "unpackIngredients should return the correct goat cheese quantity");
    cr_assert_eq(unpackedData[CHEF_LOVE], 5, "unpackIngredients should return the correct chef love quantity");
    cr_assert_eq(unpackedData[EGG], 5, "unpackIngredients should return the correct egg quantity");
    cr_assert_eq(unpackedData[BACON], 5, "unpackIngredients should return the correct bacon quantity");
    cr_assert_eq(unpackedData[BASIL], 5, "unpackIngredients should return the correct basil quantity");
    cr_assert_eq(unpackedData[PEPPER], 5, "unpackIngredients should return the correct pepper quantity");
}

Test(Ingridient, operator_assignment, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient1;
    std::vector<ingStat> ingStat = {{DOUGH, 10}, {TOMATO, 20}, {CHEESE, 30},
    {HAM, 5}, {MUSHROOM, 5}, {STEAK, 5}, {EGGPLANT, 5}, {GOAT_CHEESE, 5},
    {CHEF_LOVE, 5}, {EGG, 5}, {BACON, 5}, {BASIL, 5}, {PEPPER, 5}};

    std::shared_ptr<Ingridient> result = ingridient1.operator=(ingStat);
    cr_assert_not_null(result, "operator= should return a valid shared_ptr");
    cr_assert_eq(result->getDough(), 10, "operator= should set dough correctly");
    cr_assert_eq(result->getTomato(), 20, "operator= should set tomato correctly");
    cr_assert_eq(result->getCheese(), 30, "operator= should set cheese correctly");
}

// ===== NEW COMPREHENSIVE TESTS =====

Test(Ingridient, test_all_getters_initial_values, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;

    cr_assert_eq(ingridient.getDough(), 5, "Initial dough should be 5");
    cr_assert_eq(ingridient.getTomato(), 5, "Initial tomato should be 5");
    cr_assert_eq(ingridient.getCheese(), 5, "Initial cheese should be 5");
    cr_assert_eq(ingridient.getHam(), 5, "Initial ham should be 5");
    cr_assert_eq(ingridient.getMushroom(), 5, "Initial mushroom should be 5");
    cr_assert_eq(ingridient.getSteak(), 5, "Initial steak should be 5");
    cr_assert_eq(ingridient.getEggplant(), 5, "Initial eggplant should be 5");
    cr_assert_eq(ingridient.getGoatCheese(), 5, "Initial goat cheese should be 5");
    cr_assert_eq(ingridient.getChefLove(), 5, "Initial chef love should be 5");
    cr_assert_eq(ingridient.getEgg(), 5, "Initial egg should be 5");
    cr_assert_eq(ingridient.getBacon(), 5, "Initial bacon should be 5");
    cr_assert_eq(ingridient.getBasil(), 5, "Initial basil should be 5");
    cr_assert_eq(ingridient.getPepper(), 5, "Initial pepper should be 5");
}

Test(Ingridient, test_all_setters_boundary_values, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;

    ingridient.setDough(0);
    ingridient.setTomato(0);
    ingridient.setCheese(0);
    cr_assert_eq(ingridient.getDough(), 0, "Dough should be set to 0");
    cr_assert_eq(ingridient.getTomato(), 0, "Tomato should be set to 0");
    cr_assert_eq(ingridient.getCheese(), 0, "Cheese should be set to 0");

    ingridient.setHam(-1);
    ingridient.setMushroom(-10);
    cr_assert_eq(ingridient.getHam(), -1, "Ham should accept negative values");
    cr_assert_eq(ingridient.getMushroom(), -10, "Mushroom should accept negative values");

    ingridient.setSteak(1000);
    ingridient.setEggplant(999999);
    cr_assert_eq(ingridient.getSteak(), 1000, "Steak should accept large values");
    cr_assert_eq(ingridient.getEggplant(), 999999, "Eggplant should accept very large values");
}

Test(Ingridient, test_fridgeStatus_after_modifications, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;

    ingridient.setDough(15);
    ingridient.setTomato(3);
    ingridient.setCheese(0);
    ingridient.setHam(25);

    std::vector<ingStat> status = ingridient.fridgeStatus();
    cr_assert_eq(status.size(), 13, "fridgeStatus should return 13 ingredients");

    cr_assert_eq(status[0].type, DOUGH, "First element should be DOUGH");
    cr_assert_eq(status[0].quantity, 15, "Dough quantity should be 15");
    cr_assert_eq(status[1].type, TOMATO, "Second element should be TOMATO");
    cr_assert_eq(status[1].quantity, 3, "Tomato quantity should be 3");
    cr_assert_eq(status[2].type, CHEESE, "Third element should be CHEESE");
    cr_assert_eq(status[2].quantity, 0, "Cheese quantity should be 0");
    cr_assert_eq(status[3].type, HAM, "Fourth element should be HAM");
    cr_assert_eq(status[3].quantity, 25, "Ham quantity should be 25");
}

Test(Ingridient, test_packIngredients_with_default_values, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;
    std::string packed = ingridient.packIngredients();

    cr_assert(!packed.empty(), "Packed ingredients should not be empty");

    cr_assert(packed.find("0:5") != std::string::npos, "Should contain DOUGH:5");
    cr_assert(packed.find("1:5") != std::string::npos, "Should contain TOMATO:5");
    cr_assert(packed.find("2:5") != std::string::npos, "Should contain CHEESE:5");
    cr_assert(packed.find("3:5") != std::string::npos, "Should contain HAM:5");
    cr_assert(packed.find("4:5") != std::string::npos, "Should contain MUSHROOM:5");

    cr_assert(packed.back() != '|', "Packed string should not end with |");
}

Test(Ingridient, test_packIngredients_with_zero_values, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;

    ingridient.setDough(0);
    ingridient.setTomato(0);
    ingridient.setCheese(0);

    std::string packed = ingridient.packIngredients();
    cr_assert(packed.find("0:0") == std::string::npos, "Zero dough should not be packed");
    cr_assert(packed.find("1:0") == std::string::npos, "Zero tomato should not be packed");
    cr_assert(packed.find("2:0") == std::string::npos, "Zero cheese should not be packed");

    cr_assert(packed.find("3:5") != std::string::npos, "Non-zero ham should be packed");
}

Test(Ingridient, test_packIngredients_mixed_values, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;

    ingridient.setDough(10);
    ingridient.setTomato(0);
    ingridient.setCheese(-5);
    ingridient.setHam(100);

    std::string packed = ingridient.packIngredients();
    cr_assert(packed.find("0:10") != std::string::npos, "Should pack positive dough");
    cr_assert(packed.find("1:0") == std::string::npos, "Should not pack zero tomato");
    cr_assert(packed.find("2:-5") == std::string::npos, "Should not pack negative cheese");
    cr_assert(packed.find("3:100") != std::string::npos, "Should pack large ham value");
}

Test(Ingridient, test_unpackIngredients_valid_data, .init = redirect_all_stdout_ingridient) {
    std::string packedData = "0:10|1:20|2:30|3:5|4:15";
    std::map<IngridientType, int> unpacked = Ingridient::unpackIngredients(packedData);

    cr_assert_eq(unpacked[DOUGH], 10, "Unpacked dough should be 10");
    cr_assert_eq(unpacked[TOMATO], 20, "Unpacked tomato should be 20");
    cr_assert_eq(unpacked[CHEESE], 30, "Unpacked cheese should be 30");
    cr_assert_eq(unpacked[HAM], 5, "Unpacked ham should be 5");
    cr_assert_eq(unpacked[MUSHROOM], 15, "Unpacked mushroom should be 15");

    cr_assert(unpacked.find(STEAK) == unpacked.end(), "STEAK should not be in unpacked map");
}

Test(Ingridient, test_unpackIngredients_empty_data, .init = redirect_all_stdout_ingridient) {
    std::string emptyData = "";
    std::map<IngridientType, int> unpacked = Ingridient::unpackIngredients(emptyData);

    cr_assert_eq(unpacked.size(), 0, "Empty data should result in empty map");
}

Test(Ingridient, test_pack_unpack_roundtrip, .init = redirect_all_stdout_ingridient) {
    Ingridient original;

    original.setDough(15);
    original.setTomato(8);
    original.setCheese(0);
    original.setHam(25);
    original.setMushroom(3);

    std::string packed = original.packIngredients();
    std::map<IngridientType, int> unpacked = Ingridient::unpackIngredients(packed);
    cr_assert_eq(unpacked[DOUGH], 15, "Roundtrip should preserve dough");
    cr_assert_eq(unpacked[TOMATO], 8, "Roundtrip should preserve tomato");
    cr_assert_eq(unpacked[HAM], 25, "Roundtrip should preserve ham");
    cr_assert_eq(unpacked[MUSHROOM], 3, "Roundtrip should preserve mushroom");

    cr_assert(unpacked.find(CHEESE) == unpacked.end(), "Zero cheese should not be in unpacked map");
}

Test(Ingridient, test_operator_assignment_complete, .init = redirect_all_stdout_ingridient) {
    Ingridient original;

    std::vector<ingStat> stats = {
        {DOUGH, 100}, {TOMATO, 90}, {CHEESE, 80}, {HAM, 70},
        {MUSHROOM, 60}, {STEAK, 50}, {EGGPLANT, 40}, {GOAT_CHEESE, 30},
        {CHEF_LOVE, 20}, {EGG, 10}, {BACON, 5}, {BASIL, 3}, {PEPPER, 1}
    };

    std::shared_ptr<Ingridient> result = original.operator=(stats);
    cr_assert_not_null(result, "Operator= should return valid pointer");

    cr_assert_eq(result->getDough(), 100, "Dough should be 100");
    cr_assert_eq(result->getTomato(), 90, "Tomato should be 90");
    cr_assert_eq(result->getCheese(), 80, "Cheese should be 80");
    cr_assert_eq(result->getHam(), 70, "Ham should be 70");
    cr_assert_eq(result->getMushroom(), 60, "Mushroom should be 60");
    cr_assert_eq(result->getSteak(), 50, "Steak should be 50");
    cr_assert_eq(result->getEggplant(), 40, "Eggplant should be 40");
    cr_assert_eq(result->getGoatCheese(), 30, "Goat cheese should be 30");
    cr_assert_eq(result->getChefLove(), 20, "Chef love should be 20");
    cr_assert_eq(result->getEgg(), 10, "Egg should be 10");
    cr_assert_eq(result->getBacon(), 5, "Bacon should be 5");
    cr_assert_eq(result->getBasil(), 3, "Basil should be 3");
    cr_assert_eq(result->getPepper(), 1, "Pepper should be 1");
}

Test(Ingridient, test_operator_assignment_partial, .init = redirect_all_stdout_ingridient) {
    Ingridient original;

    std::vector<ingStat> partialStats = {
        {DOUGH, 25}, {CHEESE, 35}, {STEAK, 45}
    };

    std::shared_ptr<Ingridient> result = original.operator=(partialStats);
    cr_assert_not_null(result, "Operator= should handle partial stats");

    cr_assert_eq(result->getDough(), 25, "Specified dough should be set");
    cr_assert_eq(result->getCheese(), 35, "Specified cheese should be set");
    cr_assert_eq(result->getSteak(), 45, "Specified steak should be set");

    cr_assert_eq(result->getTomato(), 5, "Unspecified tomato should remain default");
    cr_assert_eq(result->getHam(), 5, "Unspecified ham should remain default");
}

Test(Ingridient, test_operator_assignment_empty_vector, .init = redirect_all_stdout_ingridient) {
    Ingridient original;
    std::vector<ingStat> emptyStats;

    std::shared_ptr<Ingridient> result = original.operator=(emptyStats);
    cr_assert_not_null(result, "Operator= should handle empty vector");

    cr_assert_eq(result->getDough(), 5, "Default dough should be preserved");
    cr_assert_eq(result->getTomato(), 5, "Default tomato should be preserved");
    cr_assert_eq(result->getCheese(), 5, "Default cheese should be preserved");
}

Test(Ingridient, test_operator_assignment_negative_values, .init = redirect_all_stdout_ingridient) {
    Ingridient original;

    std::vector<ingStat> negativeStats = {
        {DOUGH, -10}, {TOMATO, -5}, {CHEESE, 0}, {HAM, 15}
    };

    std::shared_ptr<Ingridient> result = original.operator=(negativeStats);
    cr_assert_not_null(result, "Operator= should handle negative values");
    cr_assert_eq(result->getDough(), -10, "Should accept negative dough");
    cr_assert_eq(result->getTomato(), -5, "Should accept negative tomato");
    cr_assert_eq(result->getCheese(), 0, "Should accept zero cheese");
    cr_assert_eq(result->getHam(), 15, "Should accept positive ham");
}

Test(Ingridient, test_fridgeStatus_consistency, .init = redirect_all_stdout_ingridient) {
    Ingridient ingridient;

    std::vector<ingStat> status1 = ingridient.fridgeStatus();

    ingridient.setDough(100);
    ingridient.setTomato(200);

    std::vector<ingStat> status2 = ingridient.fridgeStatus();
    cr_assert_eq(status1.size(), status2.size(), "Status size should remain consistent");
    cr_assert_eq(status2[0].quantity, 100, "Modified dough should be reflected");
    cr_assert_eq(status2[1].quantity, 200, "Modified tomato should be reflected");

    cr_assert_eq(status2[2].quantity, 5, "Unmodified cheese should remain 5");
}

Test(Ingridient, test_memory_management, .init = redirect_all_stdout_ingridient) {
    Ingridient original;

    for (int i = 0; i < 10; ++i) {
        std::vector<ingStat> stats = {{DOUGH, i * 10}, {TOMATO, i * 5}};
        std::shared_ptr<Ingridient> result = original.operator=(stats);

        cr_assert_not_null(result, "Each operator= call should return valid pointer");
        cr_assert_eq(result->getDough(), i * 10, "Dough should be set correctly in iteration");
        cr_assert_eq(result->getTomato(), i * 5, "Tomato should be set correctly in iteration");
    }
}

Test(Ingridient, test_ingredient_type_enum_values, .init = redirect_all_stdout_ingridient) {
    cr_assert_eq(DOUGH, 0, "DOUGH should be 0");
    cr_assert_eq(TOMATO, 1, "TOMATO should be 1");
    cr_assert_eq(CHEESE, 2, "CHEESE should be 2");
    cr_assert_eq(HAM, 3, "HAM should be 3");
    cr_assert_eq(MUSHROOM, 4, "MUSHROOM should be 4");
    cr_assert_eq(STEAK, 5, "STEAK should be 5");
    cr_assert_eq(EGGPLANT, 6, "EGGPLANT should be 6");
    cr_assert_eq(GOAT_CHEESE, 7, "GOAT_CHEESE should be 7");
    cr_assert_eq(CHEF_LOVE, 8, "CHEF_LOVE should be 8");
    cr_assert_eq(EGG, 9, "EGG should be 9");
    cr_assert_eq(BACON, 10, "BACON should be 10");
    cr_assert_eq(BASIL, 11, "BASIL should be 11");
    cr_assert_eq(PEPPER, 12, "PEPPER should be 12");
}

Test(Ingridient, test_ingStat_structure, .init = redirect_all_stdout_ingridient) {
    ingStat stat1 = {DOUGH, 25};
    ingStat stat2 = {TOMATO, 50};

    cr_assert_eq(stat1.type, DOUGH, "ingStat type should be set correctly");
    cr_assert_eq(stat1.quantity, 25, "ingStat quantity should be set correctly");
    cr_assert_eq(stat2.type, TOMATO, "Second ingStat type should be set correctly");
    cr_assert_eq(stat2.quantity, 50, "Second ingStat quantity should be set correctly");

    std::vector<ingStat> stats = {stat1, stat2};
    cr_assert_eq(stats.size(), 2, "Vector should contain 2 stats");
    cr_assert_eq(stats[0].type, DOUGH, "First stat in vector should be DOUGH");
    cr_assert_eq(stats[1].quantity, 50, "Second stat quantity should be 50");
}
