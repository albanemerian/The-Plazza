# Process and Interfaces

This documentation describes the working process, key elements, and interfaces of the Plazza project, as well as how they interact with each other.

## System Architecture

The Plazza project follows a multi-process architecture where communication between processes is managed through IPC (Inter-Process Communication). The system is divided into several components:

1. **Reception**: The main process that handles user input and distributes orders
2. **Kitchens**: Separate processes that prepare pizzas
3. **Cooks**: Worker threads inside each kitchen
4. **IPC System**: Communication layer between reception and kitchens


```
                    ┌───────────┐     ┌───────────┐    ┌────────────┐
                    │  IProcess │-----│ForkProcess│----│ process.so │
                    └───────────┘     └───────────┘    └────────────┘

```

## Key Interfaces

### IIPCCommunication

This interface defines methods for inter-process communication:

```cpp
class IIPCCommunication {
public:
    virtual ~IIPCCommunication() = default;
    virtual void sendMessage(const std::string &message) = 0;
    virtual std::string receiveMessage() = 0;
    virtual bool isConnected() = 0;
    virtual void closeConnection() = 0;
};
```

### IPizza

The interface that defines pizza properties and behaviors:

```cpp
class IPizza {
public:
    virtual ~IPizza() = default;
    virtual PizzaType getType() const = 0;
    virtual PizzaSize getSize() const = 0;
    virtual std::vector<Ingredient> getIngredients() const = 0;
    virtual int getCookingTime() const = 0;
};
```

### IKitchen

Interface for kitchen management:

```cpp
class IKitchen {
public:
    virtual ~IKitchen() = default;
    virtual bool canAcceptOrder(const Order &order) = 0;
    virtual void processOrder(const Order &order) = 0;
    virtual KitchenStatus getStatus() const = 0;
    virtual std::vector<Cook> getAvailableCooks() const = 0;
    virtual int getStock(Ingredient ingredient) const = 0;
};
```

## Process Flow

1. The Reception initializes and waits for user input
2. When orders are received, Reception analyzes them and distributes to available Kitchens
3. If no Kitchen can accept an order, a new Kitchen process is spawned
4. Kitchens manage their own Cooks (threads) to prepare pizzas
5. When a pizza is ready, the Kitchen notifies Reception
6. Reception updates the order status and displays to the user

## Plugin System

The Plazza project implements a plugin system for extensibility. This allows adding new pizza types, ingredients, or kitchen behaviors without modifying the core code.

### Plugin Interface

```cpp
class IPlugin {
public:
    virtual ~IPlugin() = default;
    virtual void initialize() = 0;
    virtual std::string getName() const = 0;
    virtual std::string getVersion() const = 0;
    virtual void shutdown() = 0;
};
```

### Plugin Manager

The Plugin Manager is responsible for:
1. Loading plugins at runtime
2. Registering plugin functionality
3. Managing plugin lifecycle
4. Providing access to plugin features


## Detailed Component Interactions

### Reception and Kitchen Communication

Communication between Reception and Kitchens uses a message-based protocol through the IPC system:

1. **Handshake**: When a Kitchen starts, it establishes a connection with Reception
2. **Order Assignment**: Reception sends order details to Kitchen
3. **Status Updates**: Kitchen regularly reports its status to Reception
4. **Order Completion**: Kitchen notifies when orders are complete
5. **Kitchen Management**: Reception can send commands to adjust Kitchen behavior

### Thread Management in Kitchens

Each Kitchen manages multiple Cook threads:

1. Cook threads access shared resources (ingredients) through mutex protection
2. Work distribution uses a task queue system
3. Thread pooling optimizes resource usage
4. Cooks can be in different states: Idle, Preparing, or Cooking

## Error Handling

The system implements robust error handling:

1. Communication errors between processes are detected and handled
2. If a Kitchen process crashes, Reception can spawn a replacement
3. Order failures are reported back to Reception
4. Resource exhaustion is detected and reported

## Configuration and Extensibility

The Plazza system is configurable through:

1. Command-line arguments for initial parameters
2. Configuration files for detailed settings
3. Plugin system for extending functionality
4. Dynamic resource allocation based on system load

This architecture ensures the system can handle varying loads efficiently while maintaining reliability and performance.
