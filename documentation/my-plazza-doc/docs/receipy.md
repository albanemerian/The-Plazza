# Food Recipes System

The Recipes component provides standardized implementations for different food items that can be prepared in the Plazza system. It uses a hierarchical class structure to enforce consistent behavior while allowing for specialized preparation methods.

## Core Architecture

The recipe system is built as a hierarchical inheritance model:

```
                    ┌───────────┐
                    │  IRecipe  │
                    └─────┬─────┘
                          │
              ┌───────────┴───────────┐
              │                       │
        ┌─────▼─────┐           ┌────▼─────┐
        │   APizza  │           │  APasta  │
        └─────┬─────┘           └────┬─────┘
              │                       │
    ┌─────────┼─────────┐      ┌─────┴──────────────┐
    │         │         │      │          │         │
┌───▼───┐ ┌───▼───┐ ┌───▼───┐  │          │         │
│Regina │ │Marga- │ │Ameri- │  │       ┌──▼──┐  ┌───▼───┐
└───────┘ │ rita  │ │cana   │  │       │Pesto│  │Carbon-│
          └───────┘ └───────┘  │       └─────┘  │ara    │
             │                 │                └───────┘
         ┌───▼────┐        ┌────▼─────┐
         │Fantasia│        │Arrabiata │
         └────────┘        |          │
                           └──────────┘
```

## Interface and Abstract Classes

### IRecipe

The `IRecipe` interface defines the contract that all food recipes must implement:

```cpp
class IRecipe {
public:
    virtual ~IRecipe() = default;
    virtual void cook(int cookTime) = 0;
    virtual std::shared_ptr<Ingridient> prepare(int number, std::shared_ptr<Ingridient> ingridient) = 0;
    virtual void serve() = 0;

    virtual int getNumber() const = 0;
    virtual void setNumber(int number) = 0;
};
```

Key methods:
- `cook`: Simulates the cooking process with a given time multiplier
- `prepare`: Consumes ingredients from inventory and prepares the dish
- `serve`: Finalizes preparation and makes the food ready for service

### APizza

The `APizza` abstract class implements common behavior for all pizza types:

```cpp
class APizza : public IRecipe {
public:
    APizza(int number);
    // ...existing code...
private:
    int _number;
};
```

### APasta

The `APasta` abstract class implements common behavior for all pasta types:

```cpp
class APasta : public IRecipe {
public:
    APasta(int number);
    // ...existing code...
private:
    int _size;
    int _number;
};
```

## Pizza Types

Each pizza has specific:
- Ingredient requirements
- Cooking time (multiplied by cookTime parameter)
- Preparation steps

| Type | Base Time | Ingredients |
|------|-----------|-------------|
| Regina | 2 | dough, tomato, cheese, ham, mushrooms |
| Margarita | 1 | dough, tomato, cheese |
| Americana | 2 | dough, tomato, cheese, steak |
| Fantasia | 4 | dough, tomato, eggplant, cheese, chef love |

### Example: Regina Pizza

```cpp
void ReginaClass::cook(int cookTime) {
    /* Cook time = 2 * multiplier */
    int baseCookTime = 2;
    double waitTime = baseCookTime * cookTime;
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>
        (waitTime * 1000)));
}

std::shared_ptr<Ingridient> ReginaClass::prepare(int number,
    std::shared_ptr<Ingridient> ingridient) {
    /* dough, tomato, gruyere, ham, mushrooms */
    for (int i = 0; i < number; i++) {
        ingridient->setDough(ingridient->getDough() - 1);
        ingridient->setTomato(ingridient->getTomato() - 1);
        ingridient->setCheese(ingridient->getCheese() - 1);
        ingridient->setHam(ingridient->getHam() - 1);
        ingridient->setMushroom(ingridient->getMushroom() - 1);
        ingridient->fridgeStatus();
    }
    return ingridient;
}
```

## Pasta Types

Each pasta type has specific:
- Ingredient requirements
- Cooking time (multiplied by cookTime parameter)
- Preparation steps

| Type | Base Time | Ingredients |
|------|-----------|-------------|
| Carbonara | 1 | dough, egg, bacon |
| Pesto | 1 | dough, basil, cheese |
| Bolognese | 2 | dough, tomato, steak, cheese |
| Arrabiata | 2 | dough, tomato, pepper, steak |
| Paffo | 4 | dough, egg, cheese, goat cheese, basil, mushroom, chef love |
| Lasagna | 5 | dough, tomato, cheese, steak, eggplant |

### Example: Carbonara Pasta

```cpp
void Carbonara::cook(int cookTime) {
    int baseCookTime = 1;
    double waitTime = baseCookTime * cookTime;
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>
        (waitTime * 1000)));
}

std::shared_ptr<Ingridient> Carbonara::prepare(int number,
    std::shared_ptr<Ingridient> ingridient) {
    for (int i = 0; i < number; i++) {
        ingridient->setDough(ingridient->getDough() - 1);
        ingridient->setEgg(ingridient->getEgg() - 1);
        ingridient->setBacon(ingridient->getBacon() - 1);
    }
    return ingridient;
}
```

## Cooking Process Overview

1. The `Cook` receives an order for a specific food type
2. The appropriate recipe class is instantiated (e.g., `ReginaClass` for a Regina pizza)
3. The `prepare` method is called, consuming ingredients from inventory
4. The `cook` method is called, simulating the cooking time by pausing execution
5. The `serve` method is called to complete the preparation

## Ingredient Management

Each recipe's `prepare` method:
1. Takes a shared pointer to the current `Ingredient` inventory
2. Deducts the required ingredients for the specified number of dishes
3. Returns the updated inventory with consumed ingredients

This allows for ingredient tracking and enforcement of ingredient requirements.

## Dependency Diagram

```
┌─────────────────┐     ┌───────────────┐
│     Kitchen     │     │     Order     │
└────────┬────────┘     └───────┬───────┘
         │                      │
         │                      │
         │                      │
         ▼                      ▼
┌─────────────────┐     ┌───────────────┐
│      Cook       │◄────┤    IRecipe    │
└────────┬────────┘     └───────┬───────┘
         │                      │
         │                      │
         ▼                      │
┌─────────────────┐             │
│   Ingredient    │◄────────────┘
└─────────────────┘
```

## Code Integration

The recipe system integrates with the Cook component of the kitchen, which:

1. Receives an order from the kitchen
2. Creates the appropriate recipe instance based on the order type
3. Executes the recipe's methods in sequence (prepare, cook, serve)
4. Updates the kitchen's ingredient inventory
5. Reports completion back to the kitchen

This design allows for easy expansion with new recipes by simply adding new implementations of `APizza` or `APasta` without changing the core cooking logic.
