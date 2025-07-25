# Plugins

The plugin system of the Plazza project allows for dynamically loading and managing external modules (processes, recipes, etc.) at runtime. This is based on the `DLLoader` library and specific entry points exposed by each plugin.

## Core Architecture

The DLLoader is built on this hierarchy system:

```
                    ┌───────────┐     ┌───────────┐    ┌────────────┐
                    │  ILoader  │-----│DLLoader   │----│dlloader.so │
                    └───────────┘     └───────────┘    └────────────┘

```

## DLLoader Library

The `DLLoader` class manages the dynamic loading of shared libraries (`.so`) and access to their symbols via `dlopen`, `dlsym`, and `dlclose`.

### Main Components

- **`ILoader` Interface**
  Defines the contract for any dynamic loader:
  - `Open(const char *path, int flag)`: Opens a shared library.
  - `Symbol(const char *symbolName)`: Retrieves a symbol (function or variable).
  - `Close()`: Closes the library.
  - `Error()`: Returns the last error encountered.
  - `getHandler() const`: Returns the library handler.
  - **Class Details**:

- **`DLLoader` Class**
  Implements `ILoader` with a template interface for typed symbol retrieval.
  - `Open`: Loads the library.
  - `Symbol`: Retrieves a symbol and casts it.
  - `Close`: Unloads the library.
  - `Error`: Gets the last error message.

### Workflow Example

1. `DLLoader` dynamically loads a library (e.g.: `plazza_process.so`).
2. `Open` loads the library and stores the handler.
3. `Symbol` retrieves the plugin entry functions.
4. `Close` unloads the library.

## Plugin Entry Points

Each plugin exposes entry points as `extern "C"` to ensure C compatibility (no name mangling) and to allow easy retrieval of symbols.

### Process Plugins


```cpp
extern "C" {
    std::shared_ptr<IProcess> createProcess();
    std::string getType();
}
```

- **createProcess**: Creates a process instance
- **getType**: Returns the process type

### Recipe Plugins

Recipes (pizzas (Americana, Regina...), pasta (carbonara, ...)):

```cpp
extern "C" {
    std::shared_ptr<IRecipe>createFood(int number) {
        return std::make_shared<AmericanaClass>(number);
    }
    std::string getType() {
        return "Americana";
    }
}
```

- **createFood**: Creates a recipe instance
- **getType**: Returns the food type

## Summary

- **Process, Recipe** are all dynamic plugins, each with their specific entry points.
- The objective of the plugin system is to facilitate the addition of new elements.
