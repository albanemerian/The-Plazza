# Kitchen

The Kitchen component represents a food preparation facility within the Plazza system. Each Kitchen operates as an independent process and is responsible for processing pizza orders.

## Core Responsibilities

- **Order Processing**: Receives and processes pizza orders from Reception
- **Resource Management**: Manages cooks and ingredients
- **Cooking Simulation**: Simulates the preparation of pizzas with realistic timing
- **Lifecycle Management**: Self-terminates when inactive to conserve system resources

## Architecture

The Kitchen class operates as a separate process, communicating with Reception through socket connections. Each Kitchen manages multiple cooks and maintains its own ingredient inventory.

### Components

1. **Process Management**:
   - `startKitchenProcess`: Forks a new process for the kitchen
   - `startKitchen`: Initializes kitchen resources in the child process
   - `stopKitchen`: Gracefully terminates the kitchen process

2. **Cook Management**:
   - `createCooks`: Creates the specified number of cook threads
   - Each cook runs in its own thread to process orders in parallel

3. **Order Handling**:
   - `processOrder`: Validates and queues incoming orders
   - `canAcceptOrder`: Checks if the kitchen can accept an order based on current capacity
   - Orders are processed by available cooks using a thread-safe queue

4. **Resource Management**:
   - `restock`: Periodically replenishes ingredients
   - Tracks ingredient levels and reports shortages to Reception

## Lifecycle of a Kitchen

1. **Creation**: Reception creates a kitchen with parameters (ID, number of cooks, cook time, restock time)
2. **Initialization**: Kitchen forks into a new process and initializes its resources
3. **Socket Setup**: Kitchen creates a server socket for communication with Reception
4. **Operation**: Kitchen processes orders, manages cooks, and periodically restocks ingredients
5. **Inactivity Monitoring**: Kitchen tracks its last activity time
6. **Self-Termination**: After 5 seconds of inactivity with no pending orders, the kitchen self-terminates

## Implementation Details

- **Multi-Threading**: Each cook operates in its own thread to allow parallel processing
- **Thread Safety**: Uses mutex locks and condition variables to ensure thread-safe operations
- **IPC**: Communicates with Reception through Unix domain sockets
- **Error Handling**: Robust error handling for kitchen operations
- **Resource Management**: Auto-scaling system where kitchens self-terminate when idle

## Communication Protocol

The Kitchen communicates with Reception using a simple message protocol:

- **0x01**: Incoming order from Reception
  - Format: `0x01|pizza_type|pizza_size|quantity;`

- **0x02**: Order completion notification to Reception
  - Format: `0x02|kitchen_id|ingredient_status;`

- **0x03**: Kitchen closure notification to Reception
  - Format: `0x03|kitchen_id;`

- **0x04**: Low ingredients warning to Reception
  - Format: `0x04|kitchen_id;`

## Cook Operation

Each cook thread:
1. Waits for an order using a condition variable
2. Claims an order from the shared queue
3. Processes the order (simulating cooking time)
4. Updates ingredient inventory
5. Notifies Reception of order completion
6. Returns to waiting state

## Ingredients Management

- Each kitchen maintains its own ingredient inventory
- Ingredients are consumed when preparing pizzas
- Stock is replenished periodically based on the restockTime parameter
- Kitchen tracks ingredient levels and sends warnings when running low

## Résumé

- **Primitives, lumières et modes graphiques** sont tous des plugins dynamiques, chacun avec ses points d'entrée spécifiques.
- Le système permet d'ajouter de nouveaux types d'objets sans recompiler le raytracer.
- Les factories du raytracer chargent dynamiquement ces plugins et utilisent leurs points d'entrée pour instancier les objets nécessaires à la scène.
