/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** AmericanaClass
*/


#include "../../../common/APizza.hpp"

#ifndef AMERICANA_HPP_
#define AMERICANA_HPP_

class AmericanaClass : public APizza {
    public:
        AmericanaClass(int number);
        ~AmericanaClass() override;

        /* Method */
        void cook(int cookTime) override;
        std::shared_ptr<Ingridient> prepare(int number, std::shared_ptr<Ingridient> ingridient) override;
        void serve() override;

    protected:
    private:
};

#endif /* !AMERICANA_HPP_ */
