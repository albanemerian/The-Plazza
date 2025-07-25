# Inter-Process Communication (IPC) in Plazza

## Unix Domain Sockets (UDS)

For the Plazza project, we chose Unix Domain Sockets (UDS) as our primary Inter-Process Communication mechanism. UDS provides an efficient way for processes running on the same system to communicate with each other, without the overhead of network protocols.

### Why Unix Domain Sockets?

- **Performance**: UDS is optimized for local communication and performs better than TCP/IP for processes on the same machine.
- **Established API**: Uses the familiar socket API, making it easier to implement.
- **Reliable Communication**: Provides reliable, bidirectional communication channels.
- **Flexibility**: Supports both message-based and stream-based communication.
- **Security**: Leverages filesystem permissions for access control.

### Message structure

Here the messages formating follow the logic of a TCP binairie format, the messages are formatted like this :
-> Starts : 0x0n = n beeing the type of the message
-> Ends : ;
Here is the list of messages used :

#### Reception sending the order to a Kitchen

```
0x01|type|size|number;
```

    - type = enum recipy type
    - size = enum recipy type
    - number = number ordered

#### Kitchen sending the status of it's "fridge"

```
0x02|id|ing:quant|ing:quant|...;
```

    - id = Id if the kitchen sending
    - ing = ingridient type enum
    - quant = quantity of ingridient

#### Kitchen sending the fact that it has been inactive for 5 sec

```
0x03|id;
```

    - id = Id of the kitchen

#### Kitchen sending status after refilling a unit

```
0x04|id|ing:quant|ing:quant|...;
```

    - id = Id if the kitchen sending
    - ing = ingridient type enum
    - quant = quantity of ingridient

### Implementation Details

In our implementation, the kitchen processes communicate with the reception using UDS:

1. Each kitchen creates a socket at initialization using a unique path:
```cpp
std::string socketPath = "/tmp/plazza_kitchen_" + std::to_string(_ID) + ".sock";
```

2. The socket is created and configured to accept connections:
```cpp
_socket.createServer(socketPath);
_socket.acceptClient();
```

3. The reception connects to each kitchen using the established socket path.

## Data Serialization

To transmit data between processes, we implemented a custom protocol with message formats for different types of communication.

### Message Protocol

Messages follow this pattern: `Code|Data1|Data2|...;`

- **Order messages** (from Reception to Kitchen): `0x01|PizzaType|Size|Number;`
- **Completion messages** (from Kitchen to Reception): `0x02|KitchenID|IngredientData;`
- **Status messages** (for kitchen inactivity): `0x03|KitchenID;`

### Packing and Unpacking Data

#### Order Serialization

Orders are packed using the Order class:

```cpp
static std::string pack(const Order &order) {
    std::ostringstream oss;
    oss << "0x01|" << static_cast<int>(order.type) << "|"
        << static_cast<int>(order.size) << "|"
        << order.number << ";";
    return oss.str();
}
```

And unpacked using regex pattern matching:

```cpp
static Order unpack(const std::string &data) {
    Order order = {Nothing, Zero, 0};
    std::regex pattern("0x01\\|(\\d+)\\|(\\d+)\\|(\\d+);");
    std::smatch match;
    if (std::regex_match(data, match, pattern)) {
        order.type = static_cast<PizzaType>(std::stoi(match[1].str()));
        order.size = static_cast<Size>(std::stoi(match[2].str()));
        order.number = std::stoi(match[3].str());
    }
    return order;
}
```

#### Ingredient Serialization

Ingredients are serialized as key-value pairs with types and quantities:

```cpp
std::string packIngredients() const {
    std::ostringstream packed;

    if (_dough > 0) packed << DOUGH << ":" << _dough << "|";
    if (_tomato > 0) packed << TOMATO << ":" << _tomato << "|";
    if (_cheese > 0) packed << CHEESE << ":" << _cheese << "|";
    // ... other ingredients
    
    std::string result = packed.str();
    if (!result.empty() && result.back() == '|') {
        result.pop_back();
    }
    return result;
}
```

And deserialized using string tokenization:

```cpp
std::map<IngridientType, int> unpackIngredients(const std::string& packedData) {
    std::map<IngridientType, int> result;
    std::istringstream stream(packedData);
    std::string token;

    while (std::getline(stream, token, '|')) {
        size_t colonPos = token.find(':');
        if (colonPos != std::string::npos) {
            int type = std::stoi(token.substr(0, colonPos));
            int qty = std::stoi(token.substr(colonPos + 1));
            result[static_cast<IngridientType>(type)] = qty;
        }
    }
    return result;
}
```

## Socket Communication Flow

1. Reception sends orders to kitchens using the format: `0x01|type|size|number;`
2. Kitchen processes the order and updates ingredients
3. Kitchen sends a completion message: `0x02|kitchenID|ingredientData;`
4. If a kitchen is inactive for too long, it sends: `0x03|kitchenID;`

This IPC approach allows our Plazza system to maintain efficient communication between the reception process and multiple kitchen processes, with clear message formats for orders, status updates, and ingredient tracking.
