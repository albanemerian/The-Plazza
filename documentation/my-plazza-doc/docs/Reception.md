# Reception

The Reception component is the central orchestrator of the Plazza system, responsible for handling customer orders and coordinating kitchens to fulfill those orders.

## Core Responsibilities

- **Order Processing**: Receives, validates and distributes pizza orders
- **Kitchen Management**: Creates, monitors, and destroys kitchen instances
- **Resource Allocation**: Efficiently distributes orders across available kitchens
- **System Monitoring**: Tracks the status of all kitchens and orders

## Architecture

The Reception class maintains a collection of Kitchen instances and communicates with them through socket connections. Each kitchen runs as a separate process to ensure parallelism and isolation.

### Components

1. **Main Loop (`orderingLoop`)**: 
   - Continuously reads user input from the console
   - Processes commands like orders, status requests, and exit command
   - Delegates order processing to appropriate methods

2. **Order Processing (`processOrders`, `checkCommand`)**: 
   - Validates order syntax using regex patterns
   - Parses orders into structured data
   - Distributes orders among kitchens based on their capacity
   - Creates new kitchens when necessary

3. **Kitchen Management**:
   - `createKitchen`: Initializes a new kitchen process with specified parameters
   - `destroyKitchen`: Terminates a kitchen when it's no longer needed
   - `monitorKitchens`: Continuously checks the status of all kitchens

4. **Inter-Process Communication (`interMessaege`)**:
   - Handles messages from kitchens (order completion, ingredient status)
   - Updates kitchen status based on received information
   - Reacts to kitchen closure notifications

## Order Flow

1. User enters an order in the format: `PizzaType Size x{Quantity};`
2. Reception validates the order format and pizza specifications
3. Reception determines how to distribute the order among kitchens:
   - Try to use existing kitchens first
   - Create new kitchens if necessary to handle order volume
4. Orders are sent to kitchens via socket connections
5. Kitchens process orders and send completion messages back to Reception
6. Reception updates its internal state based on these messages

## Implementation Details

- **Thread Safety**: Uses mutex locks to protect shared resources
- **Error Handling**: Implements custom exceptions for various error scenarios
- **Modularity**: Separates concerns like command parsing, kitchen monitoring, and order processing
- **Dynamic Scaling**: Automatically creates and destroys kitchens based on demand

## Kitchen Communication Protocol

Kitchens and Reception communicate using a simple message protocol:

- **0x01**: Order message (Reception → Kitchen)
- **0x02**: Order completion message (Kitchen → Reception)
- **0x03**: Kitchen closure notification (Kitchen → Reception)
- **0x04**: Low ingredients warning (Kitchen → Reception)
