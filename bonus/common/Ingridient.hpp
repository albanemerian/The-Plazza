/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Ingridient
*/

#include <vector>
#include <string>
#include <sstream>
#include <map>

#ifndef INGRIDIENT_HPP_
#define INGRIDIENT_HPP_

enum IngridientType
{
    DOUGH = 0,
    TOMATO = 1,
    CHEESE = 2,
    HAM = 3,
    MUSHROOM = 4,
    STEAK = 5,
    EGGPLANT = 6,
    GOAT_CHEESE = 7,
    CHEF_LOVE = 8,
    EGG = 9,
    BACON = 10,
    BASIL = 11,
    PEPPER = 12
};

struct ingStat {
    IngridientType type;
    int quantity;
};

class Ingridient {
    public:
        Ingridient();
        ~Ingridient() = default;
        std::vector<ingStat> fridgeStatus();

        /* Getter */
        int getDough() const;
        int getTomato() const;
        int getCheese() const;
        int getHam() const;
        int getMushroom() const;
        int getSteak() const;
        int getEggplant() const;
        int getGoatCheese() const;
        int getChefLove() const;
        int getEgg() const;
        int getBacon() const;
        int getBasil() const;
        int getPepper() const;

        /* Setter */
        void setDough(int dough);
        void setTomato(int tomato);
        void setCheese(int cheese);
        void setHam(int ham);
        void setMushroom(int mushroom);
        void setSteak(int steak);
        void setEggplant(int eggplant);
        void setGoatCheese(int goatCheese);
        void setChefLove(int chefLove);
        void setEgg(int egg);
        void setBacon(int bacon);
        void setBasil(int basil);
        void setPepper(int pepper);

        /* Packing/Unpacking methods */
        std::string packIngredients() const;
        static std::map<IngridientType, int> unpackIngredients(const std::string& packedData);
        std::shared_ptr<Ingridient> operator=(const std::vector<ingStat> &ingStat);
    private:
        int _dough;
        int _tomato;
        int _cheese;
        int _ham;
        int _mushroom;
        int _steak;
        int _eggplant;
        int _goatCheese;
        int _chefLove;
        int _egg;
        int _bacon;
        int _basil;
        int _pepper;
        std::vector<ingStat> _ingridient;
};

#endif /* !INGRIDIENT_HPP_ */
