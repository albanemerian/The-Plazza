# Errors

## Error Handling System

The Plazza project implements a custom exception handling mechanism using an object-oriented approach:

### IException Interface

The base interface `IException` inherits from the standard C++ `std::exception` and defines the following methods:

- `what()`: Returns the error message as a C-string (inherited from `std::exception`)
- `getType()`: Returns the type of exception as a string
- `getMessage()`: Returns the raw error message as a string
- `getFormattedMessage()`: Returns a formatted error message, typically with color codes for terminal display

### AException Abstract Class

An abstract implementation of the `IException` interface that:

- Stores the error type and message
- Implements the required methods from the interface
- Formats error messages with terminal colors (red) for better visibility

### Usage

Custom exceptions can be created by inheriting from `AException`:

```cpp
class MyCustomException : public AException {
public:
    MyCustomException(const std::string& message)
    : AException("CustomError", message) {}
};
```

Exceptions can be thrown and caught as follows:

```cpp
try {
    throw MyCustomException("Something went wrong!");
} catch (const IException& e) {
    std::cerr << e.getFormattedMessage() << std::endl;
    // Or simply:
    // std::cerr << e.what() << std::endl;
}
```

The formatted output will appear as: `[CustomError] Something went wrong!` (with "CustomError" in red).

