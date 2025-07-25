/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Reception
*/

#ifndef RECEPTION_HPP_
#define RECEPTION_HPP_

#include <thread>
#include <atomic>
#include <mutex>
#include <unordered_map>


#include "../../common/AException.hpp"
#include "../../common/APizza.hpp"
#include "../../common/Socket.hpp"
#include "../kitchen/Kitchen.hpp"


class Reception {

    class ErrorReception : public AException {
        public:
            ErrorReception(const std::string &message);
    };

    public:
        Reception();
        ~Reception();

        /* Getter */
        int getNbKitchens() const;
        std::vector<std::shared_ptr<Kitchen>> getKitchens() const;
        std::shared_ptr<Kitchen> getKitchen(int id) const;

        /* Setter */
        void setValues(int nbCooks, int cookTime, int restockTime,
            bool debug = false);

        /* Methods */
        void createKitchen(int id, int nbCooks, int cookTime, int restockTime);
        void destroyKitchen(int id);
        void processOrders(const std::vector<std::string> &orders);
        void orderingLoop();
        bool sendOrderToKitchen(const std::string &orderData);
        void monitorKitchens();
        void updateKitchenStat(std::map<IngridientType, int> ingredients,
            std::shared_ptr<Kitchen> kitchens);

        /* Parser Methods */
        std::map<RecipyType, std::string> reloadRecipyTypeNames();
        std::vector<std::string> checkCommand(const char *command);
        std::string typeToString(RecipyType type);
        void printMenu(std::map<RecipyType, std::string> RecipyTypeNames);

        /* Messefe func handler */
        void interMessaege(std::shared_ptr<Socket> socket, int id);
        void inactivityMessage(std::string message);
        void orderCompletionMessage(std::string message);
        void refillMessage(std::string message);
        void queueMessage(std::string message);
        void cookStatusMessage(std::string message);
    protected:
    private:
        int _nbCooks;
        int _cookTime;
        int _restockTime;
        int _nbKitchens;
        bool _isDebug;
        std::vector<std::shared_ptr<Kitchen>> _kitchens;
        std::unordered_map<int, std::shared_ptr<Socket>> _kitchenSockets;
        std::mutex _kitchensMutex;
        std::atomic<bool> _isRunning;
        std::thread _monitorThread;
};

#endif /* !RECEPTION_HPP_ */
