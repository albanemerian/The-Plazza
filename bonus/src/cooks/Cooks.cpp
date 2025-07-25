/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Cooks
*/

#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include <regex>
#include <magic_enum.hpp>
#include <thread>
#include <filesystem>
#include <iostream>

#include "../recipes/pizza/Americana.hpp"
#include "../recipes/pizza/Regina.hpp"
#include "../recipes/pizza/Margarita.hpp"
#include "../recipes/pizza/Fantasia.hpp"
#include "../../common/messages/Order.hpp"
#include "Cooks.hpp"


Cooks::Cooks(std::shared_ptr<Ingridient> ingridient, int id,
            int cookTime, int restockTime) {
    this->_ID = id;
    this->_cookTime = cookTime;
    this->_restockTime = restockTime;
    this->_ingridient = ingridient;
    this->_isBusy = false;
    this->_isRestocking = false;
}


/* Getter */
int Cooks::getID() const {
    return this->_ID;
}

bool Cooks::isBusy() const {
    return this->_isBusy;
}

bool Cooks::isRestocking() const {
    return this->_isRestocking;
}


/* Method */

void Cooks::waitForTheOven(Size size) {
    std::string sizeStr;
    int ovenTime = 0;
    switch (size) {
        case S:
            sizeStr = "S";
            ovenTime = 1000;
            break;
        case M:
            sizeStr = "M";
            ovenTime = 2000;
            break;
        case L:
            sizeStr = "L";
            ovenTime = 3000;
            break;
        case XL:
            sizeStr = "XL";
            ovenTime = 4000;
            break;
        case XXL:
            sizeStr = "XXL";
            ovenTime = 5000;
            break;
        default:
            sizeStr = "Unknown Size";
    }
    std::cout << "\033[1;93mCook #" << _ID << " is waiting for the oven for "
              << sizeStr << " pizza\033[0m\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(ovenTime));
}

bool Cooks::hasEnoughIngredients(const std::string &orderData,
    std::shared_ptr<Ingridient> ingridient) {
    Order order(Nothing, Zero, 0);
    order.unpack(orderData);
    int number = order.number;

    switch (order.type) {
        case Regina:
            /* Check Regina ingredients: dough tomato gruyere ham mushrooms */
            return ingridient->getDough() >= number &&
                   ingridient->getTomato() >= number &&
                   ingridient->getCheese() >= number &&
                   ingridient->getHam() >= number &&
                   ingridient->getMushroom() >= number;
        case Margarita:
            /* Check Margarita ingredients: dough, tomato, gruyere */
            return ingridient->getDough() >= number &&
                   ingridient->getTomato() >= number &&
                   ingridient->getCheese() >= number;
        case Americana:
            /* Check Americana ingredients: dough, tomato, gruyere, steak */
            return ingridient->getDough() >= number &&
                   ingridient->getTomato() >= number &&
                   ingridient->getCheese() >= number &&
                   ingridient->getSteak() >= number;
        case Fantasia:
            /* Check Fantasia dough tomato eggplant goat cheese chef love */
            return ingridient->getDough() >= number &&
                   ingridient->getTomato() >= number &&
                   ingridient->getEggplant() >= number &&
                   ingridient->getCheese() >= number &&
                   ingridient->getChefLove() >= number;
        case Carbonara:
            /* Check Carbonara ingredients: dough, tomato, gruyere, egg, bacon */
            return ingridient->getDough() >= number &&
                   ingridient->getTomato() >= number &&
                   ingridient->getCheese() >= number &&
                   ingridient->getEgg() >= number &&
                   ingridient->getBacon() >= number;
        case Pesto:
            /* Check Pesto ingredients: dough, tomato, gruyere, basil */
            return ingridient->getDough() >= number &&
                   ingridient->getTomato() >= number &&
                   ingridient->getCheese() >= number &&
                   ingridient->getBasil() >= number;
        case Bolognese:
            /* Check Bolognese ingredients: dough, tomato, gruyere, ham */
            return ingridient->getDough() >= number &&
                   ingridient->getTomato() >= number &&
                   ingridient->getCheese() >= number &&
                   ingridient->getHam() >= number;

        case Arrabiata:
            /* Check Arrabiata ingredients: dough, tomato, pepper, steak */
            return ingridient->getDough() >= number &&
                   ingridient->getTomato() >= number &&
                   ingridient->getPepper() >= number &&
                   ingridient->getSteak() >= number;

        case Paffo:
            /* Check Paffo ingredients: dough, tomato, gruyere, eggplant, goat cheese */
            return ingridient->getDough() >= number &&
                   ingridient->getTomato() >= number &&
                   ingridient->getCheese() >= number &&
                   ingridient->getEggplant() >= number &&
                   ingridient->getGoatCheese() >= number;

        case Lasagna:
            /* Check Lasagna ingredients: dough, tomato, gruyere, ham, eggplant */
            return ingridient->getDough() >= number &&
                   ingridient->getTomato() >= number &&
                   ingridient->getCheese() >= number &&
                   ingridient->getHam() >= number &&
                   ingridient->getEggplant() >= number;
        default:
            return false;
    }
}

void Cooks::setIsBusy(bool busy) {
    std::lock_guard<std::mutex> lock(_statusMutex);
    _isBusy = busy;
}


std::shared_ptr<Ingridient> Cooks::startOrder(std::shared_ptr<Ingridient>
    ingridient, std::vector<std::string> orders) {
    std::shared_ptr<Ingridient> workingIngredients = ingridient;
    auto startTime = std::chrono::high_resolution_clock::now();
    {
        std::lock_guard<std::mutex> lock(_statusMutex);
        _isBusy = true;
    }

    try {
        for (auto &orderData : orders) {
            try {
                Order order(Nothing, Zero, 0);
                order.unpack(orderData);

                std::string pizzaType = toString(order.type);
                std::cout << "\033[1;93mReceived order for " << order.number
                    << " " << pizzaType << " to Cook #" << _ID << "\033[0m\n";

                std::shared_ptr<IRecipe> pizza = findAndLoadPlugin(pizzaType,
                    order.number);
                if (!pizza) {
                    std::cerr << "Cook " << _ID << " error: Failed to load  "
                        << pizzaType << " - plugin not available or removed"
                        << std::endl;
                    {
                        std::lock_guard<std::mutex> lock(_statusMutex);
                        _isBusy = false;
                    }
                    return workingIngredients;
                }
                if (!hasEnoughIngredients(orderData, workingIngredients)) {
                    std::cout << "\033[1;31mCook #" << _ID <<
                        "doesn't have enough ingredients for order "
                        << orderData << "\033[0m\n";
                        return nullptr;
                }
                workingIngredients = pizza->prepare(order.number,
                    workingIngredients);
                pizza->cook(_cookTime);
                waitForTheOven(order.size);
                pizza->serve();
            } catch (const ErrorCooks &e) {
                std::cerr << e.getFormattedMessage() << std::endl;
                {
                    std::lock_guard<std::mutex> lock(_statusMutex);
                    _isBusy = false;
                }
                return workingIngredients;
            }
        }
    } catch (...) {
        std::cerr << "Unexpected error in Cook " << _ID << std::endl;
        {
            std::lock_guard<std::mutex> lock(_statusMutex);
            _isBusy = false;
        }
        return workingIngredients;
    }

    {
        std::lock_guard<std::mutex> lock(_statusMutex);
        _isBusy = false;
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(endTime - startTime);

    {
        std::lock_guard<std::mutex> lock(_statusMutex);
        _isBusy = false;
    }

    std::cout << "\033[1;36mCook #" << _ID << " completed order in "
              << duration.count() << " seconds\033[0m\n";
    return workingIngredients;
}


std::string Cooks::getType(const std::string& path, DLLoader<IRecipe> loader) {
    void *symbol = loader.Symbol("getType");
    if (!symbol) {
        throw ErrorCooks("Failed to get symbol from plugin");
    }
    return reinterpret_cast<std::string (*)()>(symbol)();
}

std::shared_ptr<IRecipe> Cooks::loadPlugin(const std::string &path,
    int number) {
    try {
        DLLoader<IRecipe> loader;
        loader.Open(path.c_str(), RTLD_LAZY);
        using FoodFunc = std::shared_ptr<IRecipe> (*)(int);
        void *createSymbol = loader.Symbol("createFood");
        if (!createSymbol) {
            throw ErrorCooks("Failed to get createFood symbol from plugin");
        }
        FoodFunc createFood = reinterpret_cast<FoodFunc>(createSymbol);
        std::shared_ptr<IRecipe> recipe = createFood(number);
        return recipe;
    } catch (const std::exception &e) {
        std::cerr << "Error loading plugin: " << e.what() << std::endl;
        return nullptr;
    }
}


/* Load pluggins */

std::string Cooks::toString(RecipyType type) {
    auto name = magic_enum::enum_name(type);
    return std::string{name};
}


std::shared_ptr<IRecipe> Cooks::findAndLoadPlugin(const std::string &pizzaType, int number) {
    std::string pluginDir = "./plugins/";

    try {
        if (!std::filesystem::exists(pluginDir)) {
            std::cerr << "Plugin directory does not exist: " << pluginDir << std::endl;
            return nullptr;
        }

        for (const auto &entry : std::filesystem::directory_iterator(pluginDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".so") {
                std::string pluginPath = entry.path().string();

                try {
                    DLLoader<IRecipe> loader;
                    loader.Open(pluginPath.c_str(), RTLD_LAZY);

                    std::string pluginType = getType(pluginPath, loader);

                    if (pluginType == pizzaType) {
                        using FoodFunc = std::shared_ptr<IRecipe> (*)(int);
                        void *createSymbol = loader.Symbol("createFood");
                        if (!createSymbol) {
                            continue;
                        }
                        FoodFunc createFood = reinterpret_cast<FoodFunc>(createSymbol);
                        return createFood(number);
                    }
                } catch (const std::exception &e) {
                    std::cerr << "Failed to load plugin " << pluginPath << ": " << e.what() << std::endl;
                    continue;
                }
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error scanning plugin directory: " << e.what() << std::endl;
    }

    std::cerr << "Plugin for " << pizzaType << " not found in " << pluginDir << std::endl;
    return nullptr;
}
