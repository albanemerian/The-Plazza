/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** ReceiveMessageKitchen
*/

#include <string>
#include <map>
#include <iostream>
#include <vector>

#include "Reception.hpp"
#include "../../common/messages/DoneStatus.hpp"
#include "../../common/messages/Inactivity.hpp"
#include "../../common/messages/Queue.hpp"
#include "../../common/messages/RefillStatus.hpp"
#include "../../common/messages/CookStatus.hpp"
#include "../../common/DebugLogger.hpp"

/* 0X03 Message */
void Reception::inactivityMessage(std::string message) {
    Inactivity inactivityMsg(0);
    if (this->_isDebug == true) {
        std::cout << "Inactivity Message received: " << message << std::endl;
    }
    this->_logger->log("Inactivity Message received: " + message);
    auto unpackedMsg = inactivityMsg.unpack(message);

    if (auto inactivity = std::dynamic_pointer_cast<Inactivity>(unpackedMsg)) {
        destroyKitchen(inactivity->id);
    }
    return;
}
/* 0x02 Message */
void Reception::orderCompletionMessage(std::string message) {
    DoneStatus doneMsg(0, {});
    if (this->_isDebug == true) {
        std::cout << "Order Completion Message received: " <<
            message << std::endl;
    }
    /* Log output */
    this->_logger->log("Order Completion Message received: " + message);

    auto unpackedMsg = doneMsg.unpack(message);
    if (auto status = std::dynamic_pointer_cast<DoneStatus>(unpackedMsg)) {
        std::map<IngridientType, int> ingredients;
        for (const auto& stat : status->status) {
            ingredients[stat.type] = stat.quantity;
        }

        for (const auto &kitchen : _kitchens) {
            if (kitchen->getID() == status->kitchenId) {
                updateKitchenStat(ingredients, kitchen);
                int currentOrders = kitchen->getCurrentOrders();
                if (currentOrders > 0) {
                    kitchen->setCurrentOrders(currentOrders - 1);
                    if (currentOrders - 1 < kitchen->getMaxCmd()) {
                        kitchen->setIsFull(false);
                    }
                }
                std::cout << "\033[1;32mOrder completed by Kitchen #" <<
                    status->kitchenId << "\033[0m" << std::endl;
                /* Log Output */
                this->_logger->log("Order completed by Kitchen #" +
                    std::to_string(status->kitchenId));
            }
        }
    }
}
/* 0x04 Message*/
void Reception::refillMessage(std::string message) {
    RefillStatus refillMsg(0, {});

    if (this->_isDebug == true) {
        std::cout << "Refill Message received: " << message << std::endl;
    }
    /* Log Output */
    this->_logger->log("Refill Message received: " + message);

    auto unpackedMsg = refillMsg.unpack(message);
    if (auto refill = std::dynamic_pointer_cast<RefillStatus>(unpackedMsg)) {
        std::map<IngridientType, int> ingredients;
        for (const auto& stat : refill->status) {
            ingredients[stat.type] = stat.quantity;
        }

        std::cout << "\033[1;95mRefill received by Kitchen #" <<
            refill->kitchenId << "\033[0m" << std::endl;
        /* Log Output */
        this->_logger->log("Refill received by Kitchen #" +
            std::to_string(refill->kitchenId));

        for (const auto &kitchen : _kitchens) {
            if (kitchen->getID() == refill->kitchenId) {
                updateKitchenStat(ingredients, kitchen);
            }
        }
    }
}

/* 0x05 Message */
void Reception::queueMessage(std::string message) {
    Queue queueMessage(0, 0);
    if (this->_isDebug == true) {
        std::cout << "Queue Message received: " << message << std::endl;
    }
    /* Log Output */
    this->_logger->log("Queue Message received: " + message);

    auto unpackedMsg = queueMessage.unpack(message);
    if (auto queue = std::dynamic_pointer_cast<Queue>(unpackedMsg)) {
        std::cout << "\033[1;34mQueue status from Kitchen #" <<
            queue->kitchenId << ": " << queue->nbCurrentOrders
            << " current orders\033[0m" << std::endl;
        /* Log Output */
        this->_logger->log("Queue status from Kitchen #" +
            std::to_string(queue->kitchenId) + ": " +
            std::to_string(queue->nbCurrentOrders) + " current orders");

        for (const auto &kitchen : _kitchens) {
            if (kitchen->getID() == queue->kitchenId) {
                kitchen->setCurrentOrders(queue->nbCurrentOrders);
            }
        }
    }
}


/* 0x06 Messages */

void Reception::cookStatusMessage(std::string message) {
    CookStatus cookStatusMsg(0, {});
    if (this->_isDebug == true) {
        std::cout << "Cook Status Message received: " << message << std::endl;
    }
    /* Log Output */
    this->_logger->log("Cook Status Message received: " + message);

    auto unpackedMsg = cookStatusMsg.unpack(message);
    if (auto cookStatus = std::dynamic_pointer_cast<CookStatus>(unpackedMsg)) {
        if (this->_isDebug == true) {
            std::cout << "\033[1;36mCook Status from Kitchen #" <<
                cookStatus->_kitchenId << ":\033[0m" << std::endl;
        }
        /* Log Output */
        this->_logger->log("Cook Status from Kitchen #" +
            std::to_string(cookStatus->_kitchenId) + ":");

        for (const auto &kitchen : this->_kitchens) {
            if (kitchen->getID() == cookStatus->_kitchenId) {
                std::vector<std::shared_ptr<Cooks>> cooks = kitchen->getCooks();

                if (cooks.empty()) {
                    kitchen->createCooks();
                    cooks = kitchen->getCooks();
                }

                for (const auto &cookData : cookStatus->_cooksStatus) {
                    for (const auto &cook : cooks) {
                        if (cook->getID() == cookData.cookId) {
                            cook->setIsBusy(cookData.isBusy);
                        }
                    }
                }
                break;
            }
        }
    }
}

void Reception::interMessaege(std::shared_ptr<Socket> socket, int id) {
    if (socket && socket->isConnected()) {
        std::string message = socket->receive();
        if (!message.empty()) {
            if (this->_isDebug == true)
                std::cout << "Message : " << message << std::endl;

            /* Log Output */
            this->_logger->log("Message received from Kitchen #" +
                std::to_string(id) + ": " + message);

            std::string delimiter = "\r\n";
            size_t pos = 0;
            std::string token;

            while ((pos = message.find(delimiter)) != std::string::npos) {
                token = message.substr(0, pos);
                if (!token.empty()) {
                    if (token.substr(0, 4) == "0x03") {
                        inactivityMessage(token);
                    } else if (token.substr(0, 4) == "0x02") {
                        orderCompletionMessage(token);
                    } else if (token.substr(0, 4) == "0x04") {
                        refillMessage(token);
                    } else if (token.substr(0, 4) == "0x05") {
                        queueMessage(token);
                    } else if (token.substr(0, 4) == "0x06") {
                        cookStatusMessage(token);
                    } else {
                        if (this->_isDebug == true) {
                            std::cerr << "Unknown message type: " << token
                                << std::endl;
                        }
                    }
                }
                message.erase(0, pos + delimiter.length());
            }

            // Handle the last message if it doesn't end with delimiter
            if (!message.empty()) {
                if (message.substr(0, 4) == "0x03") {
                    inactivityMessage(message);
                } else if (message.substr(0, 4) == "0x02") {
                    orderCompletionMessage(message);
                } else if (message.substr(0, 4) == "0x04") {
                    refillMessage(message);
                } else if (message.substr(0, 4) == "0x05") {
                    queueMessage(message);
                } else if (message.substr(0, 4) == "0x06") {
                    cookStatusMessage(message);
                } else {
                    if (this->_isDebug == true) {
                        std::cerr << "Unknown message type: " << message
                            << std::endl;
                    }
                }
            }
        }
    }
}
