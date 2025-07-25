/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** FantasiaClass
*/


#include "../../../common/APizza.hpp"

#ifndef FANTASIA_HPP_
#define FANTASIA_HPP_

class FantasiaClass : public APizza {
    public:
        FantasiaClass(int number);
        ~FantasiaClass() override;

        void cook(int cookTime) override;
        std::shared_ptr<Ingridient> prepare(int number, std::shared_ptr<Ingridient> ingridient) override;
        void serve() override;

    protected:
    private:
};

#endif /* !FANTASIA_HPP_ */
