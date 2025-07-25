/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Cooks
*/

#include <iostream>
#include <memory>
#include <mutex>
#include <vector>

#include "../../common/AException.hpp"
#include "../../common/Ingridient.hpp"
#include "../../common/IRecipe.hpp"
#include "../../lib/DLLoader.hpp"

#ifndef COOKS_HPP_
    #define COOKS_HPP_

class Cooks {

    class ErrorCooks : public AException {
        public:
            ErrorCooks(const std::string &message);
    };

    public:
        Cooks(std::shared_ptr<Ingridient> ingridient, int id,
            int cookTime, int restockTime);
        ~Cooks() = default;

        /* Method */
        std::shared_ptr<Ingridient> startOrder(std::shared_ptr<Ingridient> ingridient, std::vector<std::string> order);
        bool hasEnoughIngredients(const std::string &orderData, std::shared_ptr<Ingridient> ingridient);
        void waitForTheOven(Size size);
        std::shared_ptr<IRecipe> loadPlugin(const std::string &path, int number);
        std::string getType(const std::string& path, DLLoader<IRecipe> loader);
        /* Getter */
        int getID() const;
        bool isBusy() const;
        bool isRestocking() const;

        /* Setter */
        void setIsBusy(bool isBusy);
        /* Loader Pluggins */
        std::string toString(RecipyType type);
        std::shared_ptr<IRecipe> findAndLoadPlugin(const std::string &pizzaType, int number);
    private:
        int _ID;
        int _cookTime;
        int _restockTime;
        std::shared_ptr<Ingridient> _ingridient;
        std::mutex _statusMutex;
        bool _isBusy;
        bool _isRestocking;
};

#endif /* !COOKS_HPP_ */
