# Plazza Project Architecture

## System Overview

The Plazza project implements a multi-process pizza restaurant simulation with the following key architectural patterns:

- **Multi-Process Architecture**: Reception and Kitchens run as separate processes
- **Multi-Threading**: Each Kitchen manages multiple Cook threads
- **Plugin System**: Dynamic loading of recipe implementations
- **IPC Communication**: Unix Domain Sockets for process communication
- **Factory Pattern**: Dynamic creation of recipes and food items

## High-Level Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────────────────────┐
│                                PLAZZA SYSTEM                                    │
├─────────────────────────────────────────────────────────────────────────────────┤
│                                                                                 │
│  ┌─────────────┐    ┌──────────────────────────────────────────────────────┐    │
│  │    main     │────┤                  Plazza                              │    │
│  │   (entry)   │    │  - parseCmd()                                        │    │
│  └─────────────┘    │  - orderingLoop()                                    │    │
│                     │  - Configuration parameters                          │    │
│                     └──────────────────┬───────────────────────────────────┘    │
│                                        │                                        │
│                     ┌──────────────────▼───────────────────────────────────┐    │
│                     │                Reception                             │    │
│                     │  - Order processing & validation                     │    │
│                     │  - Kitchen management & lifecycle                    │    │
│                     │  - User interface (command shell)                    │    │
│                     │  - IPC coordination                                  │    │
│                     └──────────────────┬───────────────────────────────────┘    │
│                                        │                                        │
│                          ┌─────────────┼─────────────┐                          │
│                          │             │             │                          │
│              ┌───────────▼───┐  ┌──────▼──────┐  ┌───▼──────────┐               │
│              │   Kitchen 1   │  │  Kitchen 2  │  │  Kitchen N   │               │
│              │  (Process)    │  │ (Process)   │  │  (Process)   │               │
│              └───────────────┘  └─────────────┘  └──────────────┘               │
│                                                                                 │
└─────────────────────────────────────────────────────────────────────────────────┘
```

## Process Architecture

```
┌─────────────────┐                    ┌─────────────────┐
│   Main Process  │                    │ Kitchen Process │
│                 │                    │                 │
│ ┌─────────────┐ │    Unix Domain     │ ┌─────────────┐ │
│ │ Reception   │ │◄──── Sockets ────► │ │ Kitchen     │ │
│ │             │ │                    │ │             │ │
│ │ - Orders    │ │                    │ │ - Cooks     │ │
│ │ - Kitchens  │ │                    │ │ - Orders    │ │
│ │ - IPC       │ │                    │ │ - Recipes   │ │
│ └─────────────┘ │                    │ │ - IPC       │ │
│                 │                    │ └─────────────┘ │
│                 │                    │                 │
└─────────────────┘                    └─────────────────┘
         │                                       │
         │                                       │
         ▼                                       ▼
┌─────────────────┐                    ┌─────────────────┐
│  User Interface │                    │  Cook Threads   │
│                 │                    │                 │
│ - Command Line  │                    │ ┌─────────────┐ │
│ - Order Input   │                    │ │   Cook 1    │ │
│ - Status Output │                    │ │ - Recipes   │ │
└─────────────────┘                    │ │ - Plugins   │ │
                                       │ └─────────────┘ │
                                       │ ┌─────────────┐ │
                                       │ │   Cook N    │ │
                                       │ │ - Recipes   │ │
                                       │ │ - Plugins   │ │
                                       │ └─────────────┘ │
                                       └─────────────────┘
```

### Process Lifecycle Management

#### Reception Process Flow
```
┌─────────────────────────────────────────────────────────────────────────┐
│                     RECEPTION PROCESS LIFECYCLE                         │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                         │
│  Start ──► Initialize ──► Create Kitchen ──► Monitor ──► Cleanup        │
│     │           │              │               │           │            │
│     ▼           ▼              ▼               ▼           ▼            │
│ ┌────────┐ ┌─────────┐    ┌──────────┐   ┌─────────┐ ┌─────────┐        │
│ │Command │ │Config   │    │fork() +  │   │Kitchen  │ │Terminate│        │
│ │Line    │ │Parse    │    │execve()  │   │Status   │ │All      │        │
│ │Args    │ │         │    │Kitchen   │   │Check    │ │Kitchens │        │
│ └────────┘ └─────────┘    └──────────┘   └─────────┘ └─────────┘        │
│                                                                         │
│  User Input Loop:                                                       │
│  ┌─────────────────────────────────────────────────────────────────┐    │
│  │                                                                 │    │
│  │  Read Command ──► Parse Order ──► Select Kitchen ──► Send IPC   │    │
│  │       │               │               │                │        │    │
│  │       ▼               ▼               ▼                ▼        │    │
│  │  ┌─────────┐    ┌──────────┐    ┌──────────┐    ┌─────────┐     │    │
│  │  │"quit"   │    │Validate  │    │Load      │    │0x01     │     │    │
│  │  │"status" │    │Syntax    │    │Balance   │    │Message  │     │    │
│  │  │Orders   │    │& Recipe  │    │Check     │    │Format   │     │    │
│  │  └─────────┘    └──────────┘    └──────────┘    └─────────┘     │    │
│  │                                                                 │    │
│  └─────────────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────────────┘
```

#### Kitchen Process Flow
```
┌─────────────────────────────────────────────────────────────────────────┐
│                      KITCHEN PROCESS LIFECYCLE                          │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                         │
│  Fork ──► Initialize ──► Create Cooks ──► Process Orders ──► Terminate  │
│    │          │              │               │                 │        │
│    ▼          ▼              ▼               ▼                 ▼        │
│ ┌──────┐ ┌─────────┐    ┌──────────┐   ┌──────────┐    ┌──────────┐     │
│ │execve│ │Socket   │    │Thread    │   │Order     │    │Resource  │     │
│ │Args  │ │Setup    │    │Pool      │   │Queue     │    │Cleanup   │     │
│ │Parse │ │IPC      │    │Creation  │   │Process   │    │Thread    │     │
│ └──────┘ └─────────┘    └──────────┘   └──────────┘    └──────────┘     │
│                                                                         │
│  Cook Management:                                                       │
│  ┌─────────────────────────────────────────────────────────────────┐    │
│  │                                                                 │    │
│  │  Spawn Cook ──► Assign Order ──► Monitor Progress ──► Collect   │    │
│  │      │              │               │                   │       │    │
│  │      ▼              ▼               ▼                   ▼       │    │
│  │ ┌─────────┐    ┌──────────┐    ┌──────────┐    ┌──────────┐     │    │
│  │ │std::    │    │Order     │    │Recipe    │    │Send 0x02 │     │    │
│  │ │thread   │    │Assignment│    │Execution │    │Complete  │     │    │
│  │ │Create   │    │Queue     │    │Plugin    │    │Message   │     │    │
│  │ └─────────┘    └──────────┘    └──────────┘    └──────────┘     │    │
│  │                                                                 │    │
│  └─────────────────────────────────────────────────────────────────┘    │
│                                                                         │
│  Auto-Termination Logic:                                                │
│  ┌─────────────────────────────────────────────────────────────────┐    │
│  │                                                                 │    │
│  │  Check Timer ──► No Orders? ──► Send 0x03 ──► Exit Process      │    │
│  │      │              │              │             │              │    │
│  │      ▼              ▼              ▼             ▼              │    │
│  │ ┌─────────┐    ┌──────────┐    ┌────────┐   ┌─────────┐         │    │
│  │ │5 Second │    │Empty     │    │Self    │   │exit(0)  │         │    │
│  │ │Interval │    │Queue +   │    │Destruct│   │         │         │    │
│  │ │         │    │No Active │    │Message │   │         │         │    │
│  │ └─────────┘    └──────────┘    └────────┘   └─────────┘         │    │
│  │                                                                 │    │
│  └─────────────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────────────┘
```

### Inter-Process Communication (IPC) Architecture

```
┌─────────────────────────────────────────────────────────────────────────┐
│                         IPC MESSAGE SYSTEM                              │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                         │
│   Reception Process                     Kitchen Process                 │
│  ┌─────────────────┐                   ┌─────────────────┐              │
│  │                 │                   │                 │              │
│  │  Order Queue    │    Message 0x01   │  Order Handler  │              │
│  │ ┌─────────────┐ │ ──────────────────►│ ┌─────────────┐│              │
│  │ │Regina S 2   │ │ type|size|number; │ │Parse & Queue│ │              │
│  │ │Margherita M │ │                   │ │to Cook      │ │              │
│  │ │American L 1 │ │                   │ └─────────────┘ │              │
│  │ └─────────────┘ │                   │                 │              │
│  │                 │                   │                 │              │
│  │  Status Monitor │    Message 0x02   │  Completion     │              │
│  │ ┌─────────────┐ │ ◄──────────────────│ ┌─────────────┐│              │
│  │ │Order #1234  │ │ id|status;        │ │Notify Done   ││              │
│  │ │✓ Completed  │ │                   │ │Send Update  │ │              │
│  │ └─────────────┘ │                   │ └─────────────┘ │              │
│  │                 │                   │                 │              │
│  │ Kitchen Manager │    Message 0x03   │  Self-Destruct  │              │
│  │ ┌─────────────┐ │ ◄──────────────────│ ┌─────────────┐│              │
│  │ │Remove from  │ │ id;               │ │Idle Timeout │ │              │
│  │ │Active List  │ │                   │ │Cleanup      │ │              │
│  │ └─────────────┘ │                   │ └─────────────┘ │              │
│  │                 │                   │                 │              │
│  │Resource Monitor │    Message 0x04   │  Stock Alert    │              │
│  │ ┌─────────────┐ │ ◄──────────────────│ ┌─────────────┐│              │
│  │ │Restock      │ │ id|ingredients;   │ │Low Stock    │ │              │
│  │ │Notification │ │                   │ │Detection    │ │              │
│  │ └─────────────┘ │                   │ └─────────────┘ │              │
│  └─────────────────┘                   └─────────────────┘              │
│                                                                         │
└─────────────────────────────────────────────────────────────────────────┘
```

### Message Protocol Specification

#### Message Types and Formats

```
┌─────────────────────────────────────────────────────────────────────────┐
│                        MESSAGE PROTOCOL DETAILS                         │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                         │
│  0x01 - ORDER MESSAGE (Reception → Kitchen)                             │
│  ┌─────────────────────────────────────────────────────────────────┐    │
│  │  Header  │  Type   │  Size   │  Quantity │  Terminator          │    │
│  │  ─────── │  ────── │  ────── │  ──────── │  ─────────           │    │
│  │   0x01   │   int   │  char   │    int    │      ;               │    │
│  │          │         │         │           │                      │    │
│  │  Example: 0x01|1|S|2;                                           │    │
│  │           ^^^ ─ ─ ─                                             │    │
│  │           │   │ │ │                                             │    │
│  │           │   │ │ └── Quantity: 2 pizzas                        │    │
│  │           │   │ └──── Size: 1 (Small)                           │    │
│  │           │   └────── Type: 1 (Regina)                          │    │
│  │           └────────── Message Type: Order                       │    │
│  └─────────────────────────────────────────────────────────────────┘    │
│                                                                         │
│  0x02 - COMPLETION MESSAGE (Kitchen → Reception)                        │
│  ┌─────────────────────────────────────────────────────────────────┐    │
│  │  Header  │  Order ID │  Status    │  Terminator                 │    │
│  │  ─────── │  ──────── │  ────────  │  ─────────                  │    │
│  │   0x02   │    int    │   string   │      ;                      │    │
│  │          │           │            │                             │    │
│  │  Example: 0x02|1234|COMPLETED;                                  │    │
│  │           ^^^ ──── ─────────                                    │    │
│  │           │    │      │                                         │    │
│  │           │    │      └── Status: Order finished                │    │
│  │           │    └───────── Order ID: 1234                        │    │
│  │           └───────────── Message Type: Completion               │    │
│  └─────────────────────────────────────────────────────────────────┘    │
│                                                                         │
│  0x03 - TERMINATION MESSAGE (Kitchen → Reception)                       │
│  ┌─────────────────────────────────────────────────────────────────┐    │
│  │  Header  │  Kitchen ID │  Terminator                            │    │
│  │  ─────── │  ─────────  │  ─────────                             │    │
│  │   0x03   │     int     │      ;                                 │    │
│  │          │             │                                        │    │
│  │  Example: 0x03|5;                                               │    │
│  │           ^^^ ─                                                 │    │
│  │           │   │                                                 │    │
│  │           │   └── Kitchen ID: 5 shutting down                   │    │
│  │           └────── Message Type: Termination                     │    │
│  └─────────────────────────────────────────────────────────────────┘    │
│                                                                         │
│  0x04 - STATUS MESSAGE (Kitchen → Reception)                            │
│  ┌─────────────────────────────────────────────────────────────────┐    │
│  │  Header  │  Kitchen ID │  Ingredient Data │  Terminator         │    │
│  │  ─────── │  ─────────  │  ──────────────  │  ─────────          │    │
│  │   0x04   │     int     │     packed       │      ;              │    │
│  │          │             │     string       │                     │    │
│  │  Example: 0x04|3|1:2,2:0,3:5;                                   │    │
│  │           ^^^ ─ ───────                                         │    │
│  │           │   │    │                                            │    │
│  │           │   │    └── Ingredients: Dough:2, Tomato:0, Cheese:5 │    │
│  │           │   └─────── Kitchen ID: 3                            │    │
│  │           └─────────── Message Type: Status                     │    │
│  └─────────────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────────────┘
```

#### Socket Communication Flow

```
┌─────────────────────────────────────────────────────────────────────────┐
│                    UNIX DOMAIN SOCKET COMMUNICATION                     │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                         │
│  Socket Creation & Binding:                                             │
│  ┌─────────────────────────────────────────────────────────────────┐    │
│  │                                                                 │    │
│  │  Reception Process                Kitchen Process               │    │
│  │  ┌───────────────┐               ┌───────────────┐              │    │
│  │  │1. socket()    │               │1. socket()    │              │    │
│  │  │2. bind()      │               │2. connect()   │              │    │
│  │  │3. listen()    │    Handshake  │3. Protocol    │              │    │
│  │  │4. accept()    │ ◄────────────►│   Negotiate   │              │    │
│  │  │5. Protocol    │               │4. Establish   │              │    │
│  │  │   Establish   │               │   Channel     │              │    │
│  │  └───────────────┘               └───────────────┘              │    │
│  │                                                                 │    │
│  └─────────────────────────────────────────────────────────────────┘    │
│                                                                         │
│  Message Exchange Pattern:                                              │
│  ┌─────────────────────────────────────────────────────────────────┐    │
│  │                                                                 │    │
│  │    Step 1: Order Submission                                     │    │
│  │    ┌─────────────┐            ┌─────────────┐                   │    │
│  │    │ Reception   │ ───0x01───►│  Kitchen    │                   │    │
│  │    │ "regina S 2"│            │ Parse Order │                   │    │
│  │    └─────────────┘            └─────────────┘                   │    │
│  │                                                                 │    │
│  │    Step 2: Order Processing                                     │    │
│  │    ┌─────────────┐            ┌─────────────┐                   │    │
│  │    │   Waiting   │            │Cook Threads │                   │    │
│  │    │ for Update  │            │Execute Recipe│                  │    │
│  │    └─────────────┘            └─────────────┘                   │    │
│  │                                                                 │    │
│  │    Step 3: Completion Notification                              │    │
│  │    ┌─────────────┐            ┌─────────────┐                   │    │
│  │    │ Reception   │ ◄──0x02────│  Kitchen    │                   │    │
│  │    │Update Status│            │ Order Done  │                   │    │
│  │    └─────────────┘            └─────────────┘                   │    │
│  │                                                                 │    │
│  │    Step 4: Kitchen Lifecycle                                    │    │
│  │    ┌─────────────┐            ┌─────────────┐                   │    │
│  │    │ Reception   │ ◄──0x03────│  Kitchen    │                   │    │
│  │    │Remove from  │            │Auto-Shutdown│                   │    │
│  │    │Active List  │            │   (Idle)    │                   │    │
│  │    └─────────────┘            └─────────────┘                   │    │
│  │                                                                 │    │
│  └─────────────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────────────┘
```

### Process Synchronization

```
┌─────────────────────────────────────────────────────────────────────────┐
│                     PROCESS SYNCHRONIZATION PATTERNS                    │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                         │
│  Kitchen Load Balancing:                                                │
│  ┌─────────────────────────────────────────────────────────────────┐    │
│  │                                                                 │    │
│  │  New Order ──► Check Kitchens ──► Select Best ──► Send Order    │    │
│  │      │              │                │              │           │    │
│  │      ▼              ▼                ▼              ▼           │    │
│  │ ┌─────────┐    ┌──────────┐    ┌─────────┐    ┌─────────┐       │    │
│  │ │Order    │    │Capacity  │    │Lowest   │    │0x01     │       │    │
│  │ │Received │    │Check     │    │Load     │    │Message  │       │    │
│  │ │         │    │Active    │    │Score    │    │Send     │       │    │
│  │ │         │    │Cooks     │    │         │    │         │       │    │
│  │ └─────────┘    └──────────┘    └─────────┘    └─────────┘       │    │
│  │                                                                 │    │
│  │  Load Calculation:                                              │    │
│  │  Load Score = (Active Orders / Total Cooks) * 100               │    │
│  │                                                                 │    │
│  └─────────────────────────────────────────────────────────────────┘    │
│                                                                         │
│  Auto-Scaling Mechanism:                                                │
│  ┌─────────────────────────────────────────────────────────────────┐    │
│  │                                                                 │    │
│  │  Monitor Load ──► Threshold? ──► Create Kitchen ──► Update List │    │
│  │       │              │              │                 │         │    │
│  │       ▼              ▼              ▼                 ▼         │    │
│  │ ┌─────────┐    ┌──────────┐    ┌──────────┐    ┌──────────┐     │    │
│  │ │Every    │    │All       │    │fork() +  │    │Add to    │     │    │
│  │ │Order    │    │Kitchens  │    │execve()  │    │Active    │     │    │
│  │ │Check    │    │> 80%     │    │New       │    │Kitchen   │     │    │
│  │ │         │    │Capacity  │    │Process   │    │List      │     │    │
│  │ └─────────┘    └──────────┘    └──────────┘    └──────────┘     │    │
│  │                                                                 │    │
│  │  Auto-Cleanup:                                                  │    │
│  │  Kitchen Idle > 5s ──► Send 0x03 ──► Remove from List           │    │
│  │                                                                 │    │
│  └─────────────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────────────┘
```

## Component Dependencies

### Core Dependencies

```
                    ┌─────────────┐
                    │   Plazza    │
                    └─────┬───────┘
                          │
                          ▼
                    ┌─────────────┐
                    │  Reception  │
                    └─────┬───────┘
                          │
                ┌─────────┼─────────┐
                │         │         │
                ▼         ▼         ▼
        ┌─────────────┐ ┌─────────────┐ ┌─────────────┐
        │   Kitchen   │ │    Utils    │ │ CommandParser│
        └─────┬───────┘ └─────────────┘ └─────────────┘
              │
              ▼
        ┌─────────────┐
        │    Cooks    │
        └─────┬───────┘
              │
              ▼
        ┌─────────────┐
        │   Recipes   │
        │  (Plugins)  │
        └─────────────┘
```

### Recipe System Dependencies

```
                    ┌─────────────┐
                    │   IRecipe   │
                    │ (Interface) │
                    └─────┬───────┘
                          │
              ┌───────────┼───────────┐
              │           │           │
              ▼           ▼           ▼
        ┌─────────┐ ┌─────────┐ ┌─────────┐
        │ APizza  │ │ APasta  │ │ABurger  │
        │(Abstract)│ │(Abstract)│ │(Abstract)│
        └─────┬───┘ └─────┬───┘ └─────┬───┘
              │           │           │
    ┌─────────┼─────┐     │           │
    │         │     │     │           │
    ▼         ▼     ▼     ▼           ▼
┌────────┐┌────────┐┌────────┐┌─────────┐┌─────────┐
│ Regina ││Margarita││Americana││Carbonara││ Classic │
└────────┘└────────┘└────────┘└─────────┘└─────────┘
┌────────┐                   ┌─────────┐┌─────────┐
│Fantasia│                   │  Pesto  ││ Cheese  │
└────────┘                   └─────────┘└─────────┘
                             ┌─────────┐
                             │Bolognese│
                             └─────────┘
```

## IPC Communication Flow

```
Reception Process                    Kitchen Process
┌─────────────────┐                 ┌─────────────────┐
│                 │   0x01|type|    │                 │
│   Order Input   │────size|num;───►│ Order Processing│
│                 │                 │                 │
│                 │   0x02|id|      │                 │
│ Status Update   │◄────status;─────│ Order Complete  │
│                 │                 │                 │
│                 │   0x03|id;      │                 │
│Kitchen Monitor  │◄─────────────────│ Self-Terminate  │
│                 │                 │                 │
│                 │   0x04|id|      │                 │
│Ingredient Alert │◄────status;─────│ Low Ingredients │
└─────────────────┘                 └─────────────────┘
```

## Plugin System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    Plugin System                            │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌─────────────┐    ┌─────────────┐    ┌─────────────┐      │
│  │ DLLoader<T> │    │ loadPlugin  │    │ Plugin .so  │      │
│  │             │    │             │    │             │      │
│  │ - Open()    │───►│ - Symbol()  │───►│extern "C"   │      │
│  │ - Symbol()  │    │ - Create()  │    │createFood() │      │
│  │ - Close()   │    │             │    │getType()    │      │
│  └─────────────┘    └─────────────┘    └─────────────┘      │
│                                                             │
│  Plugin Libraries:                                          │
│  ┌─────────────────────────────────────────────────────────┤
│  │ ./plugins/plazza_regina.so                              │
│  │ ./plugins/plazza_margherita.so                          │
│  │ ./plugins/plazza_american.so                            │
│  │ ./plugins/plazza_fantasia.so                            │
│  │ ./plugins/plazza_carbonara.so                           │
│  │ ./plugins/plazza_pesto.so                               │
│  └─────────────────────────────────────────────────────────┘
└─────────────────────────────────────────────────────────────┘
```

## Data Flow Architecture

```
User Input → Reception → Kitchen Selection → Cook Assignment → Plugin Loading → Recipe Execution

┌──────────┐    ┌──────────┐    ┌──────────┐    ┌──────────┐    ┌──────────┐    ┌──────────┐
│   User   │───►│Reception │───►│ Kitchen  │───►│  Cook    │───►│ Plugin   │───►│ Recipe   │
│ Command  │    │ Parser   │    │Selection │    │Assignment│    │ Loader   │    │Execution │
└──────────┘    └──────────┘    └──────────┘    └──────────┘    └──────────┘    └──────────┘
      │              │               │               │               │               │
      │              │               │               │               │               │
      ▼              ▼               ▼               ▼               ▼               ▼
┌──────────┐    ┌──────────┐    ┌──────────┐    ┌──────────┐    ┌──────────┐    ┌──────────┐
│   Order  │    │   Order  │    │   IPC    │    │ Thread   │    │  .so     │    │ Cooking  │
│Validation│    │ Queue    │    │ Message  │    │ Pool     │    │ Library  │    │Simulation│
└──────────┘    └──────────┘    └──────────┘    └──────────┘    └──────────┘    └──────────┘
```

## Thread Architecture

```
Main Process                          Kitchen Process
┌─────────────────┐                  ┌─────────────────┐
│ Reception       │                  │ Kitchen Main    │
│ (Main Thread)   │                  │ (Main Thread)   │
│                 │                  │                 │
│ - Order Input   │                  │ - IPC Handler   │
│ - Kitchen Mgmt  │                  │ - Cook Manager  │
│ - IPC Coord     │                  │ - Lifecycle     │
└─────────────────┘                  └─────┬───────────┘
                                           │
                                           │ Spawns
                                           │
                              ┌────────────┼────────────┐
                              │            │            │
                              ▼            ▼            ▼
                    ┌─────────────┐ ┌─────────────┐ ┌─────────────┐
                    │   Cook 1    │ │   Cook 2    │ │   Cook N    │
                    │  (Thread)   │ │  (Thread)   │ │  (Thread)   │
                    │             │ │             │ │             │
                    │ - Recipes   │ │ - Recipes   │ │ - Recipes   │
                    │ - Plugins   │ │ - Plugins   │ │ - Plugins   │
                    │ - Orders    │ │ - Orders    │ │ - Orders    │
                    └─────────────┘ └─────────────┘ └─────────────┘
```

## File System Dependencies

```
Project Root
├── src/
│   ├── Plazza.hpp/.cpp          # Main application class
│   ├── main.cpp                 # Entry point
│   ├── reception/
│   │   ├── Reception.hpp/.cpp   # Order management & kitchen coordination
│   │   └── CommandParser.*      # Order parsing logic
│   ├── kitchen/
│   │   └── Kitchen.hpp/.cpp     # Process management & cook coordination
│   ├── cooks/
│   │   └── Cooks.hpp/.cpp       # Recipe execution & plugin loading
│   ├── recipes/
│   │   ├── pizza/               # Pizza implementations
│   │   └── pasta/               # Pasta implementations
│   └── utils/
│       └── Utils.hpp/.cpp       # Helper functions
├── common/
│   ├── IRecipe.hpp              # Recipe interface
│   ├── APizza.hpp/.cpp          # Pizza base class
│   ├── APasta.hpp/.cpp          # Pasta base class
│   ├── AException.hpp           # Exception handling
│   ├── Ingridient.hpp/.cpp      # Ingredient management
│   └── messages/
│       └── Order.hpp/.cpp       # Order serialization
├── plugins/                     # Dynamic libraries (.so files)
│   ├── plazza_regina.so
│   ├── plazza_margherita.so
│   ├── plazza_american.so
│   └── ...
└── documentation/               # Project documentation
```

## Key Design Patterns

### 1. Factory Pattern
- Dynamic creation of recipes based on type
- Plugin loading system for extensibility

### 2. Observer Pattern
- Kitchen status monitoring
- Order completion notifications

### 3. Command Pattern
- Order processing and validation
- User command interpretation

### 4. Strategy Pattern
- Different cooking strategies per recipe
- IPC communication strategies

### 5. Singleton Pattern
- Resource managers (ingredients, plugins)
- Configuration management

## Communication Protocols

### IPC Message Format
```
Message Type | Data Structure | Terminator
0x01        | type|size|num  | ;          (Order)
0x02        | id|status      | ;          (Completion)
0x03        | id             | ;          (Termination)
0x04        | id|ingredients | ;          (Status)
```

### Plugin Interface
```cpp
extern "C" {
    std::shared_ptr<IRecipe> createFood(int number);
    std::string getType();
}
```

## Scalability Considerations

- **Process Isolation**: Each kitchen runs independently
- **Thread Pool**: Configurable number of cooks per kitchen
- **Dynamic Scaling**: Kitchens created/destroyed based on demand
- **Plugin Architecture**: Easy addition of new recipe types
- **Configurable Parameters**: Cook time, restock time, number of cooks

This architecture ensures the system is modular, scalable, and maintainable while providing clear separation of concerns and efficient resource utilization.
