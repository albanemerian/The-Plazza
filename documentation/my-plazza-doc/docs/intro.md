# The Plazza - Implementation Tutorial

## Introduction

Welcome to the Plazza implementation tutorial. This guide will help you understand how to extend the Plazza system by adding new food types, whether extending existing categories (pizzas/pastas) or creating entirely new food categories like burgers.

The Plazza system is designed to be modular and extensible, allowing for easy addition of new food types without significant changes to the core system.
Here you will find how to implement a new element in a recepy type like adding a pizza type, and how you can implement an entire new category.

To add any new element to the project please do so in the `Bonus` folder, it is intended for easier implementation of new elements.

## System Architecture Overview

Before diving into implementation, let's review the key components:

- **Reception**: The central coordinator that receives orders and distributes them to kitchens
- **Kitchen**: Processing unit that manages cooks and ingredients
- **Cook**: Worker that prepares food based on recipes
- **Recipe**: Abstract representation of how to prepare a specific food item
- **Ingredient**: Resources required to prepare food items

## Extern Building Pattern

The Plazza system uses an extern building pattern for food types. This pattern:

1. Provides a standardized way to instantiate different food classes
2. Separates class implementation from the construction logic
3. Makes extending the system with new food types more straightforward

For each food category, we define:
- A factory function using the `extern` keyword
- Registration of the function in a factory map
- Common interface for all builders

## Adding New Food Items

### 1. Adding a New Pizza Type

Let's add a "Hawaiian" pizza with ham and pineapple:
You will first of all add the pizza in the RecipyType enum.
Then you will need to create the class of your new recipy.
With those two step you will have implemented a new element,
no need to modify any parsing or error handling.

#### Step 1: Create the Pizza Class Header

```cpp
// filepath: /src/recipes/pizza/Hawaiian.hpp
/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** HawaiianClass
*/

#include "../../../common/APizza.hpp"

#ifndef HAWAIIAN_HPP_
#define HAWAIIAN_HPP_

class HawaiianClass : public APizza {
    public:
        HawaiianClass(int number);
        ~HawaiianClass() override;

        /* Method */
        void cook(int cookTime) override;
        std::shared_ptr<Ingridient> prepare(int number, std::shared_ptr<Ingridient> ingridient) override;
        void serve() override;

    protected:
    private:
};


#endif /* !HAWAIIAN_HPP_ */
```

#### Step 2: Implement the Pizza Class

```cpp
// filepath: /src/recipes/pizza/Hawaiian.cpp
/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Hawaiian
*/

#include <chrono>
#include <thread>
#include <memory>

#include "Hawaiian.hpp"

HawaiianClass::HawaiianClass(int number) :
    APizza(number) {
    // Constructor logic for HawaiianClass pizza
}

HawaiianClass::~HawaiianClass() {
}

/* Method */
void HawaiianClass::cook(int cookTime) {
    /* Cook time = 2 * multiplier */
    int baseCookTime = 2;
    double waitTime = baseCookTime * cookTime;
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>
        (waitTime * 1000)));
}

std::shared_ptr<Ingridient> HawaiianClass::prepare(int number,
    std::shared_ptr<Ingridient> ingridient) {
    /* dough, tomato, gruyere, ham, pineapple (using pepper as substitute) */
    for (int i = 0; i < number; i++) {
        ingridient->setDough(ingridient->getDough() - 1);
        ingridient->setTomato(ingridient->getTomato() - 1);
        ingridient->setCheese(ingridient->getCheese() - 1);
        ingridient->setHam(ingridient->getHam() - 1);
        ingridient->setPepper(ingridient->getPepper() - 1); // Using pepper as pineapple
    }
    return ingridient;
}

void HawaiianClass::serve() {
    // Serving logic for Hawaiian pizza
}

extern "C" {
    std::shared_ptr<IRecipe>createFood(int number) {
        return std::make_shared<HawaiinClass>(number);
    }
    std::string getType() {
        return "Hawaiian";
    }
}
```

#### Step 3: Update the PizzaType Enum

```cpp
// In common/APizza.hpp
enum PizzaType
{
    Nothing = 0,
    Regina = 1,
    Margarita = 2,
    Americana = 4,
    Fantasia = 8,
    Hawaiian = 16  // Add the new pizza type
};
```

### 2. Adding an Entirely New Food Category: Burgers

Creating a new category requires more extensive changes:
You will first of all add the pizza type in the enum.
Before creating the new classes recipies, you will need to implement
a new abstract class of the type of element you're adding.
With those two step you will have implemented a new element,
no need to modify any parsing or error handling.


#### Step 1: Create the Base Interface Class

```cpp
// filepath: /common/ABurger.hpp
/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** ABurger
*/

#ifndef ABURGER_HPP_
#define ABURGER_HPP_

#include "IRecipe.hpp"

enum BurgerType
{
    Classic = 1,
    Cheese = 2,
    Veggie = 3,
    BBQ = 4
};

class ABurger : public IRecipe {
    public:
        ABurger(int number);
        virtual ~ABurger() override = default;
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

// Define the builder function type for burgers
typedef std::shared_ptr<ABurger> (*BurgerBuilderFunc)(int);

#endif /* !ABURGER_HPP_ */
```

#### Step 2: Implement the Base Interface Class

```cpp
// filepath: /common/ABurger.cpp
/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** ABurger
*/

#include "ABurger.hpp"

ABurger::ABurger(int number) : _number(number) {
}

int ABurger::getNumber() const {
    return this->_number;
}

void ABurger::setNumber(int number) {
    this->_number = number;
}
```

#### Step 3: Create a Factory for Burgers

```cpp
// filepath: /src/factory/BurgerFactory.hpp
/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** BurgerFactory
*/

#ifndef BURGERFACTORY_HPP_
#define BURGERFACTORY_HPP_

#include <map>
#include <memory>
#include <dlfcn.h>

#include "../../common/ABurger.hpp"

class BurgerFactory {
    public:
        BurgerFactory();
        ~BurgerFactory();

        std::shared_ptr<ABurger> createBurger(BurgerType type, int number);

    private:
        BurgerBuilderFunc loadBurgerBuilder(const std::string& libraryPath, const std::string& funcName);
        std::map<BurgerType, BurgerBuilderFunc> _builderMap;
        std::vector<void*> _dlHandles;
};

#endif /* !BURGERFACTORY_HPP_ */
```

#### Step 4: Create a Specific Burger Type

```cpp
// filepath: /src/recipes/burger/ClassicBurger.hpp
/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** ClassicBurger
*/

#include "../../../common/ABurger.hpp"

#ifndef CLASSICBURGER_HPP_
#define CLASSICBURGER_HPP_

class ClassicBurger : public ABurger {
    public:
        ClassicBurger(int number);
        ~ClassicBurger() override;

        void cook(int cookTime) override;
        std::shared_ptr<Ingridient> prepare(int number, std::shared_ptr<Ingridient> ingridient) override;
        void serve() override;
    protected:
    private:
};

// Extern builder function for factory pattern
extern "C" std::shared_ptr<ABurger> buildClassicBurger(int number);

#endif /* !CLASSICBURGER_HPP_ */
```

#### Step 5: Implement the Burger Class

```cpp
// filepath: /src/recipes/burger/ClassicBurger.cpp
/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** ClassicBurger
*/

#include <chrono>
#include <thread>
#include <memory>

#include "ClassicBurger.hpp"

ClassicBurger::ClassicBurger(int number) :
    ABurger(number) {
    // Constructor logic
}

ClassicBurger::~ClassicBurger() {
}

/* Method */
void ClassicBurger::cook(int cookTime) {
    /* Cook time = 3 * multiplier */
    int baseCookTime = 3;
    double waitTime = baseCookTime * cookTime;
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>
        (waitTime * 1000)));
}

std::shared_ptr<Ingridient> ClassicBurger::prepare(int number,
    std::shared_ptr<Ingridient> ingridient) {
    /* dough (bun), tomato, cheese, steak */
    for (int i = 0; i < number; i++) {
        ingridient->setDough(ingridient->getDough() - 1);
        ingridient->setTomato(ingridient->getTomato() - 1);
        ingridient->setCheese(ingridient->getCheese() - 1);
        ingridient->setSteak(ingridient->getSteak() - 1);
    }
    return ingridient;
}

void ClassicBurger::serve() {
    // Serving logic for Classic burger
}

// Implement the extern factory function
extern "C" std::shared_ptr<ABurger> buildClassicBurger(int number) {
    return std::make_shared<ClassicBurger>(number);
}
```

#### Step 7: Update the Build System

Create a Makefile rule to compile the new burger classes as shared libraries:

```makefile
# Burger shared libraries
burgers: ${OBJ_BURGER}
	@mkdir -p ./lib
	${CC} -shared -o ./lib/classicburger.so ${OBJ_DIR}/recipes/burger/ClassicBurger.o ${CXXFLAGS}
	${CC} -shared -o ./lib/cheeseburger.so ${OBJ_DIR}/recipes/burger/CheeseBurger.o ${CXXFLAGS}
	${CC} -shared -o ./lib/veggieburger.so ${OBJ_DIR}/recipes/burger/VeggieBurger.o ${CXXFLAGS}
	${CC} -shared -o ./lib/bbqburger.so ${OBJ_DIR}/recipes/burger/BBQBurger.o ${CXXFLAGS}
```

## Using the Extern Building Pattern

### Why Use the Extern Pattern?

1. **Modularity**: Each food type can be compiled into a separate shared library
2. **Runtime Loading**: New food types can be added without recompiling the entire program
3. **Clean Interfaces**: Each food type follows a consistent interface pattern
4. **Dynamic Extension**: The system can be extended with new food types by simply adding new libraries

### How the Extern Pattern Works

1. Each food class defines an `extern "C"` function that acts as a factory method
2. The function name follows a pattern: `build<FoodName>`
3. These functions are compiled into shared libraries (`.so` files)
4. The factory classes dynamically load these libraries at runtime
5. The builder functions are stored in a map for easy lookup by type

### Best Practices for Using the Extern Pattern

1. **Consistent Naming**: Keep function names consistent (`build<ClassName>`)
2. **Error Handling**: Always check for errors when loading libraries
3. **Library Management**: Close library handles when they are no longer needed
4. **Library Path Management**: Use relative or configurable paths for loading libraries
5. **Builder Signature**: Maintain consistent function signatures across all factories
6. **Enum Types**: Always add new food types to the appropriate enum

## Example Workflow with the Extern Pattern

1. User orders a new food type: `pizza hawaiian XL x2;`
2. Reception parses the order and identifies the pizza type (Hawaiian)
3. Kitchen uses the PizzaFactory to create the pizza
4. PizzaFactory looks up the builder function for Hawaiian pizza
5. If found, it calls the function to create a HawaiianClass instance
6. The pizza is prepared, cooked, and served
7. Order completion is reported back to Reception

## Conclusion

The Plazza system's extern building pattern makes it highly extensible. By creating new shared libraries with builder functions, you can add new food types easily without modifying the core system.

This approach allows for clean separation of concerns and runtime flexibility. Whether adding a new pizza variant or an entirely new food category, the pattern provides a standardized way to integrate new components into the system.
