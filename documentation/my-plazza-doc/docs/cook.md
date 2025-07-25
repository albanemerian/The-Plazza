# Cooks

The Cooks component represents individual workers within a Kitchen who are responsible for preparing pizzas. Each Cook operates in its own thread and handles pizza orders assigned by the Kitchen.

## Core Responsibilities

- **Order Processing**: Prepares, cooks, and serves pizzas according to orders
- **Ingredient Management**: Verifies and consumes ingredients for pizza preparation
- **Pizza Creation**: Creates specific pizza types based on order specifications
- **Process Simulation**: Simulates realistic cooking times for each pizza

## Architecture

Each Cook operates as an independent entity within a Kitchen, running in its own thread to allow for parallel pizza preparation.

### Components

1. **Order Processing (`startOrder`)**:
   - Takes order data as input
   - Verifies ingredient availability
   - Creates appropriate pizza type
   - Prepares, cooks, and serves the pizza
   - Updates ingredient inventory

2. **Ingredient Management**:
   - `hasEnoughIngredients`: Checks if sufficient ingredients are available for an order
   - Consumes ingredients during pizza preparation
   - Reports shortages when ingredients are insufficient

3. **Status Management**:
   - Maintains busy/idle status
   - Maintains restocking status
   - Thread-safe status updates using mutex locks

## Cook Workflow

1. **Initialization**: Cook is created with a reference to shared ingredients, ID, cook time, and restock time
2. **Waiting**: Cook waits for an order to be assigned by the Kitchen
3. **Order Processing**:
   - Verifies ingredient availability
   - Creates appropriate pizza based on type (Regina, Margarita, Americana, Fantasia)
   - Prepares the pizza (consumes ingredients)
   - Simulates cooking time
   - Serves the pizza
4. **Status Update**: Updates status to idle after completing the order
5. **Repeat**: Returns to waiting state for next order

## Implementation Details

- **Thread Safety**: Uses mutex locks to protect status changes
- **Error Handling**: Custom exceptions for ingredient shortages and invalid orders
- **Pizza Type Handling**: Dynamic creation of different pizza types based on order specifications
- **Time Simulation**: Realistic cooking time simulation based on kitchen parameters

## Pizza Types and Ingredients

Cooks can prepare four types of pizzas, each requiring specific ingredients:

1. **Regina**:
   - Dough
   - Tomato
   - Cheese (Gruyere)
   - Ham
   - Mushrooms

2. **Margarita**:
   - Dough
   - Tomato
   - Cheese (Gruyere)

3. **Americana**:
   - Dough
   - Tomato
   - Cheese (Gruyere)
   - Steak

4. **Fantasia**:
   - Dough
   - Tomato
   - Eggplant
   - Goat Cheese
   - Chef Love (special ingredient)

## Ingredient Verification

Before preparing a pizza, the Cook verifies that all required ingredients are available in sufficient quantity. If ingredients are insufficient, an error is thrown and the order is rejected.

## Cooking Process

The cooking process includes three main phases:

1. **Preparation**: Gather and use ingredients for the pizza
2. **Cooking**: Simulate cooking time based on the kitchen's cook time parameter
3. **Serving**: Mark the pizza as ready for delivery

## Error Handling

The Cook class includes its own exception class (`ErrorCooks`) for handling cook-specific errors such as:
- Insufficient ingredients
- Unknown pizza types
- General cooking errors
