/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** APasta
*/

#ifndef APASTA_HPP_
#define APASTA_HPP_

#include "IRecipe.hpp"


class APasta : public IRecipe {
    public:
        APasta(int number);
        virtual ~APasta() override = default;
        virtual void cook(int cookTime) override = 0;
        virtual std::shared_ptr<Ingridient> prepare(int number, std::shared_ptr<Ingridient> ingridient) override = 0;
        virtual void serve() override = 0;

        /* Getter */
        int getNumber() const override;

        /* Setter */
        void setNumber(int number) override;

    private:
        int _size;
        int _number;
};

#endif /* !APASTA_HPP_ */
