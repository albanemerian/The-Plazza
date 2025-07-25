/*
** EPITECH PROJECT, 2025
** The Plazza
** File description:
** Command Parser
*/

#include <iostream>
#include <sstream>
#include <regex>
#include <vector>
#include <map>
#include <stdexcept>
#include <magic_enum.hpp>
#include <string>
#include <filesystem>
#include <dlfcn.h>

#include "Reception.hpp"
#include "../../common/messages/Order.hpp"


const std::map<Size, std::string> PizzaSizeNames = {
    {S, "S"}, {M, "M"}, {L, "L"}, {XL, "XL"}, {XXL, "XXL"}};

void Reception::printMenu(std::map<RecipyType, std::string> RecipyTypeNames) {

    std::cout << "Welcome to the Plazza!" << std::endl;
    std::cout << "Available orders:" << std::endl;
    for (const auto &pair : RecipyTypeNames) {
        std::cout << "- " << pair.second << " (" << typeToString(pair.first) << ")"
                  << std::endl;
    }
}

std::vector<std::string> Reception::checkCommand(const char *command) {
    try {
        std::string cmd(command);
        std::vector<std::string> orders;
        std::map<RecipyType, std::string> RecipyTypeNames = reloadRecipyTypeNames();

        if (strcmp(cmd.c_str(), "menu") == 0) {
            printMenu(RecipyTypeNames);
            return {};
        }
        std::istringstream ss(cmd);
        std::string token;
        while (std::getline(ss, token, ';')) {
            token = std::regex_replace(token, std::regex("^\\s+|\\s+$"), "");
            if (token.empty()) {
                continue;
            }

            std::regex pattern(
                "([a-zA-Z]+)\\s+(S|M|L|XL|XXL)\\s+x([1-9][0-9]*)");
            std::smatch matches;
            if (!std::regex_match(token, matches, pattern)) {
                throw Reception::ErrorReception(
                    "Invalid order format: " + token);
            }

            RecipyType foundRecipyType = Nothing;
            for (auto &pair : RecipyTypeNames) {
                if (pair.second == matches[1].str()) {
                    foundRecipyType = pair.first;
                    break;
                }
            }
            if (foundRecipyType == Nothing) {
                throw Reception::ErrorReception(
                    "Invalid Recipy type: " + matches[1].str());
            }

            Size size = Zero;
            for (auto &pair : PizzaSizeNames) {
                if (pair.second == matches[2].str()) {
                    size = pair.first;
                    break;
                }
            }

            int number = 0;
            try {
                number = std::stoi(matches[3].str());
            } catch (const std::invalid_argument &) {
                throw Reception::ErrorReception(
                    "Invalid quantity: " + matches[3].str());
            } catch (const std::out_of_range &) {
                throw Reception::ErrorReception(
                    "Quantity out of range: " + matches[3].str());
            }
            Order order(foundRecipyType, size, number);
            orders.push_back(order.pack(order));
        }

        if (orders.empty()) {
            throw Reception::ErrorReception("No valid orders found");
        }
        return orders;
    } catch (const Reception::ErrorReception &e) {
        std::cout << e.getFormattedMessage() << std::endl;
        return {};
    }
}

std::string Reception::typeToString(RecipyType type) {
    auto name = magic_enum::enum_name(type);
    return std::string{name};
}

std::map<RecipyType, std::string> Reception::reloadRecipyTypeNames() {
    std::map<RecipyType, std::string> recipyTypeNames;
    std::string pluginDir = "./plugins/";

    try {
        if (!std::filesystem::exists(pluginDir)) {
            std::cerr << "Plugin directory does not exist: " << pluginDir << std::endl;
            return recipyTypeNames;
        }

        for (const auto &entry : std::filesystem::directory_iterator(pluginDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".so") {
                std::string pluginPath = entry.path().string();

                try {
                    void *handle = dlopen(pluginPath.c_str(), RTLD_LAZY);
                    if (!handle) {
                        std::cerr << "Failed to load plugin: " << dlerror() << std::endl;
                        continue;
                    }

                    void *getTypeSymbol = dlsym(handle, "getType");
                    if (!getTypeSymbol) {
                        dlclose(handle);
                        continue;
                    }

                    std::string (*getType)() = reinterpret_cast<std::string (*)()>(getTypeSymbol);
                    std::string typeString = getType();

                    auto enumValue = magic_enum::enum_cast<RecipyType>(typeString);
                    if (enumValue.has_value()) {
                        std::string lowerTypeString = typeString;
                        std::transform(lowerTypeString.begin(), lowerTypeString.end(),
                                     lowerTypeString.begin(), ::tolower);
                        recipyTypeNames[enumValue.value()] = lowerTypeString;
                    }

                    dlclose(handle);
                } catch (const std::exception &e) {
                    std::cerr << "Error processing plugin " << pluginPath << ": " << e.what() << std::endl;
                    continue;
                }
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error scanning plugin directory: " << e.what() << std::endl;
    }

    return recipyTypeNames;
}
