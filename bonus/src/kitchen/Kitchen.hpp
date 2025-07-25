/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Kitchen
*/

#ifndef KITCHEN_HPP_
#define KITCHEN_HPP_

#include <queue>
#include <mutex>
#include <vector>
#include <thread>
#include <condition_variable>
#include <chrono>

#include "../cooks/Cooks.hpp"
#include "../../common/processes/IProcess.hpp"
#include "../../common/Socket.hpp"

#include "../../common/AException.hpp"

class Kitchen {

    class ErrorKitchen : public AException {
        public:
            ErrorKitchen(const std::string &message);
    };

    public:
        Kitchen(int id, int nbCooks, int cookTime, int restockTime, bool debug);
        ~Kitchen();
        void restock();
        void startKitchenProcess();
        void startKitchen();
        void run();
        void processOrder(const std::string &orderData);
        bool canAcceptOrder(int numPizzas);
        void stopKitchen();
        void sendOrder();
        void createCooks();
        void setIsFull(bool isFull);

        // Process management
        void setProcess(std::shared_ptr<IProcess> process) {
            _process = process;
        }

        /* Setter */
        void setCurrentOrders(int currentOrders);
        void incrementCurrentOrders(int amount);
        /* Getter */
        int getID() const;
        int getNbCooks() const;
        int getCookTime() const;
        int getRestockTime() const;
        int getMaxCmd() const;
        std::shared_ptr<Ingridient> getIngridient() const;
        std::vector<std::shared_ptr<Cooks>> getCooks() const;
        int getCurrentOrders() const;
        bool isFull() const;

        /* Send Messages */
        void sendQueueStatMessage();
        void sendDoneMessage();
        void sendRefillMessage();
        void sendInactive();
        void sendCookStatus();
    protected:
    private:
        int _ID;
        int _nbCooks;
        int _cookTime;
        int _restockTime;
        int _maxCmd;
        int _currentOrders;
        pid_t _pid;  // Add this line to track the process ID
        std::shared_ptr<Ingridient> _ingridient;
        std::vector<std::shared_ptr<Cooks>> _cooks;
        std::queue<std::string> _orderQueue;
        std::mutex _orderMutex;
        std::mutex _ingMutex;
        std::condition_variable _cookCV;
        std::vector<std::thread> _cookThreads;
        std::thread _restockThread;
        Socket _socket;
        std::chrono::steady_clock::time_point _lastActivity;
        std::shared_ptr<IProcess> _process;
        bool _isRunning;
        bool _isDebug;
        bool _isFull;
};

std::ostream& operator<<(std::ostream& os, const Kitchen& kitchen);

#endif /* !KITCHEN_HPP_ */
