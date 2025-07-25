/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** SendingMessageKitchen
*/

#include <string>
#include <iostream>

#include "../../common/messages/Queue.hpp"
#include "../../common/messages/Order.hpp"
#include "../../common/messages/DoneStatus.hpp"
#include "../../common/messages/Inactivity.hpp"
#include "../../common/messages/RefillStatus.hpp"
#include "../../common/messages/CookStatus.hpp"

#include "Kitchen.hpp"

void Kitchen::sendQueueStatMessage() {
    Queue queue(this->_ID, this->_currentOrders);
    std::string queueMsg = queue.pack(queue);
    if (this->_isDebug) {
        std::cout << "Queue Message: " << queueMsg << std::endl;
    }
    queueMsg += "\r\n";
    _socket << queueMsg;
}

void Kitchen::sendDoneMessage() {
    DoneStatus doneMsg(_ID, _ingridient->fridgeStatus());
    std::string Msg = doneMsg.pack(doneMsg);
    if (this->_isDebug)
        std::cout << "Done Message: " << Msg << std::endl;
    Msg += "\r\n";
    _socket << Msg;
}

void Kitchen::sendRefillMessage() {
    RefillStatus refillMsg(_ID, _ingridient->fridgeStatus());
    std::string doneMsg = refillMsg.pack(refillMsg);
    if (this->_isDebug == true)
        std::cout << "Refill Message : " << doneMsg << std::endl;
    doneMsg += "\r\n";
    _socket << doneMsg;
}

void Kitchen::sendInactive() {
    Inactivity inactivityMsg(_ID);
    std::string inactiveMsg = inactivityMsg.pack(inactivityMsg);
    inactiveMsg += "\r\n";
    _socket << inactiveMsg;
}

void Kitchen::sendCookStatus() {
    std::vector<CookStatusData> temp;
    for (const auto &cook : _cooks) {
        CookStatusData cookData;
        cookData.cookId = cook->getID();
        cookData.isBusy = cook->isBusy();
        temp.push_back(cookData);
    }
    CookStatus cookStat(this->_ID, temp);
    std::string cookStatMsg = cookStat.pack(cookStat);
    cookStatMsg += "\r\n";
    _socket << cookStatMsg;
}
