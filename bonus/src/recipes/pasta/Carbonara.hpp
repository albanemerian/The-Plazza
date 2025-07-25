/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Carbonara
*/

#include "../../../common/APasta.hpp"

#ifndef CARBONARA_HPP_
    #define CARBONARA_HPP_

class CarbonaraClass : public APasta {
    public:
        CarbonaraClass(int number);
        ~CarbonaraClass() override;

        /* Method */
        void cook(int cookTime) override;
        std::shared_ptr<Ingridient> prepare(int number, std::shared_ptr<Ingridient> ingridient) override;
        void serve() override;

    private:
};

#endif /* !CARBONAR_HPP_ */
