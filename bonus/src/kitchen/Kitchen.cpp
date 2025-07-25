/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Kitchen
*/

#include <unistd.h>
#include <sys/wait.h>

#include <memory>
#include <vector>
#include <csignal>
#include <chrono>
#include <thread>
#include <iostream>
#include <string>

#include "../../common/messages/Order.hpp"
#include "../../common/messages/DoneStatus.hpp"
#include "../../common/messages/CookStatus.hpp"

#include "Kitchen.hpp"



Kitchen::Kitchen(int id, int nbCooks, int cookTime, int restockTime,
    bool debug)
    : _ID(id), _nbCooks(nbCooks), _cookTime(cookTime),
    _restockTime(restockTime), _maxCmd(2 * nbCooks), _currentOrders(0),
    _isRunning(true), _isDebug(debug) {
    _ingridient = std::make_shared<Ingridient>();
    _lastActivity = std::chrono::steady_clock::now();
    _isFull = false;
}

Kitchen::~Kitchen() {
    stopKitchen();
}

/* Setter */

void Kitchen::setCurrentOrders(int currenOrders) {
    std::lock_guard<std::mutex> lock(_orderMutex);
    _currentOrders = currenOrders;
}

void Kitchen::incrementCurrentOrders(int amount) {
    std::lock_guard<std::mutex> lock(_orderMutex);
    _currentOrders += amount;
}

void Kitchen::createCooks() {
    for (int i = 0; i < _nbCooks; ++i) {
        _cooks.push_back(std::make_shared<Cooks>
            (_ingridient, i, _cookTime, _restockTime));
    }
}

void Kitchen::restock() {
    while (_isRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(_restockTime));

        std::lock_guard<std::mutex> lock(_ingMutex);
        auto status = _ingridient->fridgeStatus();

        for (auto &ing : status) {
            if (ing.quantity == 0) {
                while (ing.quantity < 5) {
                    if (this->_isDebug == true) {
                        std::cout << "Restocking ingredient type: " << ing.type
                            << " from " << ing.quantity << " to " <<
                            (ing.quantity + 1) << std::endl;
                    }
                    ing.quantity += 1;
                    std::shared_ptr<Ingridient> newtemp =
                        _ingridient->operator=(status);
                    _ingridient = newtemp;
                    {
                        std::lock_guard<std::mutex> activityLock(_orderMutex);
                        _lastActivity = std::chrono::steady_clock::now();
                    }
                }
                sendRefillMessage();
            }
        }
    }
}

bool Kitchen::isFull() const {
    return _isFull;
}

void Kitchen::setIsFull(bool full) {
    _isFull = full;
}

void Kitchen::startKitchenProcess() {
    if (!_process) {
        throw ErrorKitchen("Process handler not initialized");
    }

    try {
        _pid = _process->create([this]() {
            try {
                startKitchen();
                exit(0);
            } catch (const std::exception &e) {
                std::cerr << "Kitchen " << _ID << " error: " << e.what()
                << std::endl;
                exit(1);
            }
        });
        if (_pid < 0) {
            throw ErrorKitchen("Failed to create kitchen process");
        }
        if (_isDebug) {
            std::cout << "Created kitchen process with PID: " << _pid
                << std::endl;
        }
    } catch (const std::exception &e) {
        throw ErrorKitchen(std::string("Failed to create process: ") +
            e.what());
    }
}

void Kitchen::startKitchen() {
    std::string socketPath = "/tmp/plazza_kitchen_" +
        std::to_string(_ID) + ".sock";

    /* Create the "client" side socket that will with the "Server"(Reception)*/
    try {
        _socket.createServer(socketPath);
        _socket.acceptClient();
    } catch (const Socket::SocketException &e) {
        throw ErrorKitchen(std::string("Socket error: ") + e.what());
    }
    createCooks();
    _restockThread = std::thread(&Kitchen::restock, this);

    for (int i = 0; i < _nbCooks; ++i) {
        _cookThreads.emplace_back([this, i]() {
        while (_isRunning) {
            std::unique_lock<std::mutex> lock(_orderMutex);
            _cookCV.wait(lock, [this]() {
                return !_orderQueue.empty() || !_isRunning;
            });

            if (!_isRunning)
                break;
            if (!_orderQueue.empty()) {
                std::string order = _orderQueue.front();
                _orderQueue.pop();
                lock.unlock();

            try {
                std::shared_ptr<Ingridient> updatedIngredients;
                bool orderProcessed = false;
                bool hasIngredients = false;

                {
                    std::lock_guard<std::mutex> ingredientLock(_ingMutex);
                    hasIngredients = _cooks[i]->hasEnoughIngredients(order,
                        _ingridient);
                }

                if (hasIngredients) {
                    _cooks[i]->setIsBusy(true);
                    sendCookStatus();
                    updatedIngredients = _cooks[i]->startOrder(_ingridient,
                        {order});
                    if (updatedIngredients == nullptr) {
                        std::lock_guard<std::mutex> requeueLock(
                        _orderMutex);
                        _orderQueue.push(order);
                        continue;
                    }
                    {
                        std::lock_guard<std::mutex> ingredientLock(_ingMutex);
                        _ingridient = updatedIngredients;
                    }
                    orderProcessed = true;
                }

                if (orderProcessed) {
                    {
                    std::lock_guard<std::mutex> activityLock(
                        _orderMutex);
                    _lastActivity = std::chrono::steady_clock::now();
                    _currentOrders--;
                    sendCookStatus();
                    sendQueueStatMessage();
                    }
                    sendDoneMessage();

                    if (_currentOrders < _maxCmd) {
                        _isFull = false;
                    }
                } else {
                    std::lock_guard<std::mutex> requeueLock(
                        _orderMutex);
                    _orderQueue.push(order);
                }
            } catch (const std::exception &e) {
                std::cerr << "Cook " << i << " error: " << e.what()
                    << std::endl;
                std::lock_guard<std::mutex> requeueLock(_orderMutex);
                _orderQueue.push(order);
            } catch (...) {
                std::cerr << "Cook " << i << " unknown error\n";
                std::lock_guard<std::mutex> requeueLock(_orderMutex);
                _orderQueue.push(order);
            }
        } else {
            lock.unlock();
            std::this_thread::yield();
        }
        }
        });
    }
    run();
}


void Kitchen::run() {
    std::string messageBuffer;

    while (_isRunning) {
        try {
            {
            std::lock_guard<std::mutex> lock(_orderMutex);
            auto now = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(
                now - _lastActivity).count();
            if (this->_isDebug == true)
                std::cout << "Duration: " << duration << " CurrentOrders "
                << _currentOrders << " QueueSize: " << _orderQueue.size()
                << std::endl;
            if (duration > 5 && _currentOrders == 0 && _orderQueue.empty()) {
                sendInactive();
                stopKitchen();
                break;
            }
            }

            if (_socket.isConnected()) {
                std::string message;
                try {
                    _socket >> message;

                    if (!message.empty()) {
                        messageBuffer += message;

                        size_t pos = 0;
                        while ((pos = messageBuffer.find("\r\n")) !=
                            std::string::npos) {
                            std::string completeMessage =
                                messageBuffer.substr(0, pos);
                            messageBuffer.erase(0, pos + 2);

                            if (!completeMessage.empty()) {
                                processOrder(completeMessage);
                                _lastActivity = std::chrono::
                                    steady_clock::now();
                            }
                        }
                    }
                } catch (const Socket::SocketException &e) {
                    std::cerr << "Socket error in kitchen " << _ID << ": " <<
                    e.what() << std::endl;
                    stopKitchen();
                    break;
                }
            } else {
                stopKitchen();
                break;
            }
        } catch (const std::exception &e) {
            std::cerr << "Kitchen " << _ID << " error: " << e.what()
            << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}


void Kitchen::processOrder(const std::string &orderData) {
    if (orderData.substr(0, 4) == "0x01") {
        try {
            Order orderMsg(Nothing, Zero, 0);
            auto unpackedMsg = orderMsg.unpack(orderData);
            if (auto order = std::dynamic_pointer_cast<Order>(unpackedMsg)) {
                int numPizzas = order->number;
                {
                    std::lock_guard<std::mutex> lock(_orderMutex);
                    for (int i = 0; i < numPizzas; i++) {
                        Order singleOrder(order->type, order->size, 1);
                        std::string singlePizzaOrder =
                            singleOrder.pack(singleOrder);
                        _orderQueue.push(singlePizzaOrder);
                    }
                    _currentOrders += numPizzas;
                    if (_currentOrders >= _maxCmd) {
                        _isFull = true;
                    }
                    sendQueueStatMessage();
                }
                _cookCV.notify_all();
                if (this->_isDebug) {
                    std::cout << "Added " << numPizzas << ", notifying cooks"
                        << std::endl;
                }
            }
        } catch (const std::exception &e) {
            std::cerr << "Error parsing order: " << e.what() << std::endl;
        }
    }
}



bool Kitchen::canAcceptOrder(int numPizzas) {
    std::lock_guard<std::mutex> lock(_orderMutex);
    int totalCurrentLoad = _currentOrders +
        static_cast<int>(_orderQueue.size());
    bool canAccept = (totalCurrentLoad + numPizzas) <= _maxCmd;
    if (canAccept && (totalCurrentLoad + numPizzas) == _maxCmd) {
        _isFull = true;
    }
    return canAccept;
}

void Kitchen::stopKitchen() {
    if (_isRunning) {
        _isRunning = false;
        _cookCV.notify_all();
        if (_restockThread.joinable())
            _restockThread.join();

        for (auto &thread : _cookThreads) {
            if (thread.joinable())
                thread.join();
        }

        try {
            if (_socket.isConnected())
                _socket.closeServer();
        } catch (...) {
            // Ignore socket errors during shutdown
        }
        if (_process) {
            _process->close();
        }
    }
}

void Kitchen::sendOrder() {
    // This is now handled by the Socket communication
}

/* Getter */

int Kitchen::getID() const {
    return _ID;
}

int Kitchen::getNbCooks() const {
    return _nbCooks;
}

int Kitchen::getCookTime() const {
    return _cookTime;
}

int Kitchen::getRestockTime() const {
    return _restockTime;
}

int Kitchen::getMaxCmd() const {
    return _maxCmd;
}

std::shared_ptr<Ingridient> Kitchen::getIngridient() const {
    return _ingridient;
}

std::vector<std::shared_ptr<Cooks>> Kitchen::getCooks() const {
    return _cooks;
}

int Kitchen::getCurrentOrders() const {
    return _currentOrders;
}

/* Overload */
std::ostream& operator<<(std::ostream& os, const Kitchen &kitchen) {
    /* Printing kitchen status*/
    os << "Kitchen N° " << kitchen.getID() << " | Cooks: " <<
        kitchen.getNbCooks() << " | Cook time: " << kitchen.getCookTime() <<
        " | Restock time: " << kitchen.getRestockTime() << " | Current orders "
        << kitchen.getCurrentOrders() << "/" << kitchen.getMaxCmd();

    /* Printing cooks status */
    for (const auto cook : kitchen.getCooks()) {
        os << "\n\tCook ID: " << cook->getID() << " | Busy: " << cook->isBusy();
    }

    /* Printing ingredient status */
    auto ingridient = kitchen.getIngridient();
    ingridient->fridgeStatus();
    os << "\n\tIngredients: "
       << "Dough: " << ingridient->getDough()
       << " Tomato: " << ingridient->getTomato()
       << " Cheese: " << ingridient->getCheese()
       << " Ham: " << ingridient->getHam()
       << " Mushroom: " << ingridient->getMushroom()
       << " Steak: " << ingridient->getSteak()
       << " Eggplant: " << ingridient->getEggplant()
       << " GoatCheese: " << ingridient->getGoatCheese()
       << " Love: " << ingridient->getChefLove();

    return os;
}
