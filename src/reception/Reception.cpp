/*
** EPITECH PROJECT, 2025
** The Plazza
** File description:
** Reception
*/

#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <fstream>
#include <map>

#include "../../common/messages/IMesagges.hpp"
#include "../../common/messages/Order.hpp"
#include "../../common/messages/DoneStatus.hpp"
#include "../../common/messages/Inactivity.hpp"
#include "../../common/messages/RefillStatus.hpp"
#include "../../common/DebugLogger.hpp"
#include "Reception.hpp"
#include "../../lib/DLLoader.hpp"


Reception::Reception() : _isRunning(true) {
    this->_nbCooks = 0;
    this->_cookTime = 0;
    this->_restockTime = 0;
    this->_nbKitchens = 1;
    this->_isDebug = false;
    this->_kitchens.clear();
    // this->loader = nullptr;
}

Reception::~Reception() {
    _isRunning = false;
    if (_monitorThread.joinable())
        _monitorThread.join();

    std::lock_guard<std::mutex> lock(_kitchensMutex);
    _kitchens.clear();
    _kitchenSockets.clear();
}

/* Getter */

int Reception::getNbKitchens() const {
    return this->_nbKitchens;
}

std::vector<std::shared_ptr<Kitchen>> Reception::getKitchens() const {
    return this->_kitchens;
}

std::shared_ptr<Kitchen> Reception::getKitchen(int id) const {
    for (auto &kitchen : _kitchens) {
        if (kitchen->getID() == id)
            return kitchen;
    }
    return nullptr;
}

void Reception::killKitchen() {
    std::lock_guard<std::mutex> lock(_kitchensMutex);
    for (auto &kitchen : _kitchens) {
        kitchen->stopKitchen();
    }
    _kitchens.clear();
    _kitchenSockets.clear();
    _nbKitchens = 0;
    if (this->loader.getHandler() != nullptr) {
        this->loader.Close();
    }
    /* Log Output */
    this->_logger->log("All kitchens have been closed.");
    std::cout << "\033[1;31mAll kitchens have been closed.\033[0m" << std::endl;
}

/* Methods */

void Reception::setValues(int nbCooks, int cookTime, int restockTime,
    bool debug, std::shared_ptr<DebugLogger> logger) {
    this->_nbCooks = nbCooks;
    this->_cookTime = cookTime;
    this->_restockTime = restockTime;
    this->_nbKitchens = 1;
    this->_isDebug = debug;
    this->_kitchens.clear();
    this->_logger = logger;
    _monitorThread = std::thread(&Reception::monitorKitchens, this);
}

void Reception::createKitchen(int id, int nbCooks, int cookTime,
    int restockTime) {
    std::shared_ptr<Kitchen> kitchen = std::make_shared<Kitchen>(id, nbCooks,
        cookTime, restockTime, this->_isDebug, this->_logger);

    try {
        /* Load the Process shared librairy */
        std::string processLibPath = "./plugins/plazza_fork.so";

        if (this->_isDebug) {
            std::cout << "Attempting to load process library from: "
                << processLibPath << std::endl;
        }
        /* Log Output */
        this->_logger->log("Attempting to load process library from: "
            + processLibPath);
        if (loader.Open(processLibPath.c_str(), RTLD_LAZY) == nullptr) {
            processLibPath = "./plugins/plazza_fork.so";
            if (this->_isDebug) {
                std::cout << "First path failed, trying: " << processLibPath
                    << std::endl;
            }

            if (loader.Open(processLibPath.c_str(), RTLD_LAZY) == nullptr) {
                throw ErrorReception(std::string("Failed to open library: ") +
                    loader.Error());
            }
        }

        if (this->_isDebug) {
            std::cout << "Process library loaded successfully." << std::endl;
        }
        /* Log Output */
        this->_logger->log("Process library loaded successfully.");
        auto createProcess = loader.getSymbol("createProcess");
        if (!createProcess) {
            loader.Close();
            throw ErrorReception(std::string("Failed to get symbol: ") +
                loader.Error());
        }
        std::shared_ptr<IProcess> process = createProcess();
        if (!process) {
            loader.Close();
            throw ErrorReception("createProcess returned nullptr");
        }
        kitchen->setProcess(process);
        kitchen->startKitchenProcess();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        std::string socketPath = "/tmp/plazza_kitchen_" + std::to_string(id)
            + ".sock";
        std::shared_ptr<Socket> socket = std::make_shared<Socket>();
        socket->connectToServer(socketPath);
        {
            std::lock_guard<std::mutex> lock(_kitchensMutex);
            _kitchens.push_back(kitchen);
            _kitchenSockets[id] = socket;
            _nbKitchens++;
        }

        std::cout << "\033[1;32mCreated Kitchen #" << id << "\033[0m"
            << std::endl;
        /* Log Output */
        this->_logger->log("Created Kitchen #" + std::to_string(id));

        loader.Close();
    } catch (const std::exception &e) {
        throw ErrorReception(std::string("Failed to create kitchen: ")
            + e.what());
    }
}

void Reception::destroyKitchen(int id) {
    std::lock_guard<std::mutex> lock(_kitchensMutex);

    for (auto it = _kitchens.begin(); it != _kitchens.end(); ++it) {
        if ((*it)->getID() == id) {
            auto socketIt = _kitchenSockets.find(id);
            if (socketIt != _kitchenSockets.end()) {
                _kitchenSockets.erase(socketIt);
            }
            if (it->get() != nullptr)
                _kitchens.erase(it);
            _nbKitchens--;
            std::cout << "\033[1;31mKitchen #" << id <<
                " has closed\033[0m" << std::endl;
            return;
        }
    }
}

void Reception::updateKitchenStat(std::map<IngridientType, int> ingredients,
    std::shared_ptr<Kitchen> kitchens) {
    kitchens->getIngridient()->setDough(ingredients[DOUGH]);
    kitchens->getIngridient()->setTomato(ingredients[TOMATO]);
    kitchens->getIngridient()->setCheese(ingredients[CHEESE]);
    kitchens->getIngridient()->setHam(ingredients[HAM]);
    kitchens->getIngridient()->setMushroom(ingredients[MUSHROOM]);
    kitchens->getIngridient()->setSteak(ingredients[STEAK]);
    kitchens->getIngridient()->setEggplant(ingredients[EGGPLANT]);
    kitchens->getIngridient()->setGoatCheese(ingredients[GOAT_CHEESE]);
    kitchens->getIngridient()->setChefLove(ingredients[CHEF_LOVE]);
    kitchens->getIngridient()->setEgg(ingredients[EGG]);
    kitchens->getIngridient()->setBacon(ingredients[BACON]);
    kitchens->getIngridient()->setBasil(ingredients[BASIL]);
    kitchens->getIngridient()->setPepper(ingredients[PEPPER]);
}

void Reception::monitorKitchens() {
    while (_isRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        std::vector<int> kitchenIds;
        {
            std::lock_guard<std::mutex> lock(_kitchensMutex);
            for (const auto &kitchen : _kitchens) {
                kitchenIds.push_back(kitchen->getID());
            }
        }

        for (int id : kitchenIds) {
            std::shared_ptr<Socket> socket;
            {
                std::lock_guard<std::mutex> lock(_kitchensMutex);
                auto it = _kitchenSockets.find(id);
                if (it == _kitchenSockets.end()) continue;
                socket = it->second;
            }

            try {
                interMessaege(socket, id);
            } catch (const std::exception &e) {
                std::cerr << "Error monitoring kitchen " << id << ": "
                << e.what() << std::endl;
            }
        }

        int status;
        pid_t pid;
        while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
            if (this->_isDebug == true) {
                std::cout << "Child process " << pid << " terminated" <<
                    std::endl;
            }
            /* Log Output */
            this->_logger->log("Child process " + std::to_string(pid) +
                " terminated");
        }
    }
}

bool Reception::sendOrderToKitchen(std::string &orderData) {
    std::lock_guard<std::mutex> lock(_kitchensMutex);

    int pizzaQuantity = 1;
    try {
        Order orderMsg(Nothing, Zero, 0);
        auto unpackedOrder = orderMsg.unpack(orderData);
        if (auto order = std::dynamic_pointer_cast<Order>(unpackedOrder)) {
            pizzaQuantity = order->number;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error parsing order quantity: "
        << e.what() << std::endl;
    }

    for (auto &kitchen : _kitchens) {
        int id = kitchen->getID();
        auto socketIt = _kitchenSockets.find(id);

        if (socketIt != _kitchenSockets.end() &&
            kitchen->canAcceptOrder(pizzaQuantity)) {
            try {
                orderData += "\r\n";
                socketIt->second->send(orderData);
                std::cout << "\033[1;34mSent order to Kitchen #" << id <<
                "\033[0m" << std::endl;
                /* Log Output */
                this->_logger->log("Sent order to Kitchen #" +
                    std::to_string(id) + ": " + orderData);
                return true;
            } catch (const std::exception &e) {
                std::cerr << "Error sending to kitchen " << id << ": " <<
                e.what() << std::endl;
            }
        }
    }

    return false;
}

void Reception::processOrders(const std::vector<std::string> &orders) {
    for (const auto &orderStr : orders) {
        Order originalOrder(Nothing, Zero, 0);
        try {
            auto unpackedMsg = originalOrder.unpack(orderStr);
            if (auto order = std::dynamic_pointer_cast<Order>(unpackedMsg)) {
                originalOrder = *order;
            }
        } catch (const std::exception &e) {
            std::cerr << "Error parsing order: " << e.what() << std::endl;
            continue;
        }

        int totalPizzas = originalOrder.number;
        int remainingPizzas = totalPizzas;

        if (this->_isDebug == true) {
            std::cout << "Processing order for " << totalPizzas <<
            " pizzas of type " << originalOrder.type << " size " <<
            originalOrder.size << std::endl;
        }
        /* Log Output */
        this->_logger->log("Processing order for " +
            std::to_string(totalPizzas) + " pizzas of type " +
            typeToString(originalOrder.type));

        int kitchenCapacity = 2 * _nbCooks;
        int kitchensNeeded = (totalPizzas + kitchenCapacity - 1) /
            kitchenCapacity;
        if (this->_isDebug == true) {
            std::cout << "Order may require up to " << kitchensNeeded
            << " kitchens" << std::endl;
        }
        /* Log Output */
        this->_logger->log("Order may require up to " +
            std::to_string(kitchensNeeded) + " kitchens");
        {
            std::lock_guard<std::mutex> lock(_kitchensMutex);
            for (auto &kitchen : _kitchens) {
                if (remainingPizzas <= 0)
                    break;

                int id = kitchen->getID();
                auto socketIt = _kitchenSockets.find(id);
                if (socketIt == _kitchenSockets.end())
                    continue;

                int currentLoad = kitchen->getCurrentOrders();
                std::cout << "Current load for Kitchen #" << id << ": "
                    << currentLoad << std::endl;
                /* Log Output */
                this->_logger->log("Current load for Kitchen #" +
                    std::to_string(id) + ": " + std::to_string(currentLoad));

                int maxCapacity = kitchen->getMaxCmd();
                int availableCapacity = maxCapacity - currentLoad;

                if (availableCapacity <= 0) {
                    kitchen->setIsFull(true);
                    continue;
                }

                int pizzasToAssign = std::min(remainingPizzas,
                    availableCapacity);
                if (pizzasToAssign > 0) {
                    Order subOrder(originalOrder.type, originalOrder.size,
                        pizzasToAssign);
                    std::string subOrderStr = subOrder.pack(subOrder);
                    try {
                        subOrderStr += "\r\n";
                        socketIt->second->send(subOrderStr);
                        std::cout << "\033[1;34mSent order for " <<
                            pizzasToAssign << " pizzas to Kitchen #"
                            << id << "\033[0m\n";
                        /* Log Output */
                        this->_logger->log("Sent order for " +
                            std::to_string(pizzasToAssign) +
                            " pizzas to Kitchen #" + std::to_string(id));
                        remainingPizzas -= pizzasToAssign;

                        kitchen->setCurrentOrders(currentLoad + pizzasToAssign);
                        if ((currentLoad + pizzasToAssign) >= maxCapacity) {
                            kitchen->setIsFull(true);
                        }
                    } catch (const std::exception &e) {
                        std::cerr << "Error sending to kitchen " << id << ": "
                        << e.what() << std::endl;
                    }
                }
            }
        }

        while (remainingPizzas > 0) {
            int newId = 0;
            {
                std::lock_guard<std::mutex> lock(_kitchensMutex);
                std::vector<int> existingIds;

                for (const auto& kitchen : _kitchens) {
                    existingIds.push_back(kitchen->getID());
                }

                while (std::find(existingIds.begin(), existingIds.end(),
                    newId) != existingIds.end()) {
                    newId++;
                }
            }

            try {
                createKitchen(newId, _nbCooks, _cookTime, _restockTime);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));

                int kitchenCapacity = 2 * _nbCooks;
                int pizzasForThisKitchen = std::min(remainingPizzas,
                    kitchenCapacity);

                Order subOrder(originalOrder.type, originalOrder.size,
                    pizzasForThisKitchen);
                std::string subOrderStr = subOrder.pack(subOrder);
                {
                    std::lock_guard<std::mutex> lock(_kitchensMutex);
                    auto socketIt = _kitchenSockets.find(newId);
                    if (socketIt != _kitchenSockets.end()) {
                        try {
                            subOrderStr += "\r\n";
                            socketIt->second->send(subOrderStr);
                            std::cout << "\033[1;34mSent order for " <<
                                pizzasForThisKitchen <<
                                " pizzas to new Kitchen #" << newId
                                << "\033[0m" << std::endl;
                            /* Log Output */
                            this->_logger->log("Sent order for " +
                                std::to_string(pizzasForThisKitchen) +
                                " pizzas to new Kitchen #" +
                                std::to_string(newId));
                            remainingPizzas -= pizzasForThisKitchen;

                            auto kit = getKitchen(newId);
                            if (kit) {
                                kit->setCurrentOrders(pizzasForThisKitchen);
                                if (pizzasForThisKitchen >= kitchenCapacity) {
                                    kit->setIsFull(true);
                                }
                            }
                        } catch (const std::exception &e) {
                            std::cerr << "Error sending to new kitchen " <<
                            newId << ": " << e.what() << std::endl;
                        }
                    }
                }
            } catch (const std::exception &e) {
                std::cerr << "Failed to create kitchen #" << newId <<
                ": " << e.what() << std::endl;
                break;
            }
        }

        if (remainingPizzas > 0) {
            std::cerr << "\033[1;31mWarning: " << remainingPizzas <<
            " pizzas from the order could not be processed!\033[0m\n";
            /* Log Output */
            this->_logger->log("Warning: " + std::to_string(remainingPizzas) +
                " pizzas from the order could not be processed!");
        } else {
            std::cout << "\033[1;32mOrder fully distributed " <<
                "across kitchens\033[0m" << std::endl;
            /* Log Output */
            this->_logger->log("Order fully distributed across kitchens");
        }
    }
}


void Reception::orderingLoop() {
    std::string input;
    std::vector<std::string> orders;

    std::cout <<
        "\033[1;33mWelcome to the Plazza! Type 'exit' to quit.\033[0m" <<
        std::endl;
    while (_isRunning) {
        std::cout << "\033[1;34mPlazza $> \033[0m";
        std::getline(std::cin, input);
        if (std::cin.eof()) {
            std::cout << "\n\033[1;31m[(Ctrl+D pressed). Exiting...]\033[0m\n";
            break;
        }

        if (input == "exit") {
            killKitchen();
            break;
        }
        if (input == "clear") {
            system("clear");
            continue;
        }
        if (input == "status" || input == "S") {
            std::lock_guard<std::mutex> lock(_kitchensMutex);
            if (_kitchens.empty()) {
                std::cout << "No Kitchen running." << std::endl;
            } else {
                for (auto &kitchen : _kitchens) {
                    std::cout << *(kitchen) << std::endl;
                }
            }
            continue;
        }

        orders = checkCommand(input.c_str());
        if (!orders.empty()) {
            for (auto &order : orders) {
                std::cout << "\033[1;32mOrder received:\033[0m " << order
                    << std::endl;
                /* Log Output */
                this->_logger->log("Order received: " + order);
            }
            processOrders(orders);
        }
    }

    std::cout << "\033[1;33mArrivederci! See you soon!\033[0m" << std::endl;
}
