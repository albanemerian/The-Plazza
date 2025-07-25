/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** IReceipy
*/


#include <memory>
#include "Ingridient.hpp"

#ifndef IRECIPE_HPP_
#define IRECIPE_HPP_

enum Size
{
    Zero = 0,
    S = 1,
    M = 2,
    L = 4,
    XL = 8,
    XXL = 16
};

enum RecipyType
{
    /* Pizza types*/
    Nothing = 0,
    Regina = 1,
    Margarita = 2,
    Americana = 4,
    Fantasia = 8,

    /* Pasta Types */
    Carbonara = 10,
    Pesto = 12,
    Bolognese = 14,
    Arrabiata = 16,
    Paffo = 18,
    Lasagna = 20
};


class IRecipe {
    public:

        virtual ~IRecipe() = default;
        virtual void cook(int cookTime) = 0;
        virtual std::shared_ptr<Ingridient> prepare(int number, std::shared_ptr<Ingridient> ingridient) = 0;
        virtual void serve() = 0;

        /* Getter */
        virtual int getNumber() const = 0;
        /* Setter */
        virtual void setNumber(int number) = 0;

};

#endif /* !IRECEIPY_HPP_ */
