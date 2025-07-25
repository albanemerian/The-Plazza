/*
** EPITECH PROJECT, 2025
** The Plazza
** File description:
** Plazza
*/

#include <cstring>

#include <string>
#include <memory>
#include <iostream>

#include "Plazza.hpp"
#include "../common/DebugLogger.hpp"
#include "utils/Utils.hpp"

Plazza::Plazza() {
    this->_nbCooks = 0;
    this->_timerCooker = 0;
    this->_timerRestock = 0;
    this->_debug = false;
}

Plazza::~Plazza() {
}

void Plazza::parseCmd(char **av, int ac) {
    Utils utils;
    for (int i = 1; i < ac; i++) {
        if (strcmp(av[i], "-h") == 0 || strcmp(av[i], "--help") == 0) {
            utils.helper();
            exit(0);
        }
        if (strcmp(av[i], "-d") == 0) {
            this->_debug = true;
        }
    }
    if (ac != 4 && this->_debug == false) {
        utils.helper();
        exit(84);
    }

    for (int i = 1; i < 4; i++) {
        std::string arg = av[i];
        if (arg == "-d")
            continue;
        bool hasDot = false;
        for (char c : arg) {
            if (!std::isdigit(c) && c != '-' && c != '.') {
                throw Plazza::ErrorParsing("Arguments must be numerical");
            }
            if (c == '.') {
                if (hasDot) {
                    throw Plazza::ErrorParsing("Arguments must be numerical");
                }
                hasDot = true;
            }
        }
        double value = std::atof(av[i]);
        if (value < 0) {
            throw Plazza::ErrorParsing("Arguments cannot be negative");
        }
    }
    this->_timerCooker = std::atof(av[1]);
    this->_nbCooks = std::atoi(av[2]);
    this->_timerRestock = std::atoi(av[3]);
    this->_logger = std::make_shared<DebugLogger>("plazza.log");
    if (this->_debug == true) {
        std::cout << "Number of cooks: " << this->_nbCooks << std::endl;
        std::cout << "Timer cooker: " << this->_timerCooker << std::endl;
        std::cout << "Timer restock: " << this->_timerRestock << std::endl;
    }
    this->_logger->log("Plazza initialized with parameters: " +
        std::to_string(this->_nbCooks) + ", " +
        std::to_string(this->_timerCooker) + ", " +
        std::to_string(this->_timerRestock) + ", debug: " +
        (this->_debug ? "true" : "false"));
    this->_reception.setValues(this->_nbCooks,
        this->_timerCooker, this->_timerRestock, this->_debug, this->_logger);
}

void Plazza::orderingLoop() {
    this->_reception.orderingLoop();
}
