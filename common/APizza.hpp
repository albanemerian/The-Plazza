/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** APizza
*/

#ifndef APIZZA_HPP_
#define APIZZA_HPP_

#include "IRecipe.hpp"

enum PizzaType
{
    Nothing = 0,
    Regina = 1,
    Margarita = 2,
    Americana = 4,
    Fantasia = 8
};


class APizza : public IRecipe {
    public:
        APizza(int number);
        virtual ~APizza() override = default;
        virtual void cook(int cookTime) override = 0;
        virtual std::shared_ptr<Ingridient> prepare(int number, std::shared_ptr<Ingridient> ingridient) override = 0;
        virtual void serve() override = 0;

        /* Getter */
        int getNumber() const override;

        /* Setter */
        void setNumber(int number) override;
    private:
        int _number;

};

#endif /* !APIZZA_HPP_ */
