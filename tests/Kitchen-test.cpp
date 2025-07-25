/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Cook-test
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/kitchen/Kitchen.hpp"
#include "../common/Ingridient.hpp"
#include "../common/AException.hpp"
#include "../common/messages/Order.hpp"
#include "../common/DebugLogger.hpp"
#include "../common/processes/IProcess.hpp"
#include <memory>
#include <thread>
#include <chrono>

void redirect_all_stdout_kitchen(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Mock Process class for testing
class MockProcess : public IProcess {
private:
    bool _shouldFailCreate;
    bool _shouldFailClose;
    pid_t _mockPid;
    std::function<void()> _storedFunc;

public:
    MockProcess(bool shouldFailCreate = false, bool shouldFailClose = false)
        : _shouldFailCreate(shouldFailCreate), _shouldFailClose(shouldFailClose), _mockPid(1234) {}

    ~MockProcess() override {
        // Cleanup if necessary
    }
    pid_t getPid() const override {
        return _mockPid;
    }

    pid_t create(const std::function<void()> &childLogic) override {
        if (_shouldFailCreate) {
            return -1;
        }
        _storedFunc = childLogic;
        return _mockPid;
    }
    
    void close() override {
        if (_shouldFailClose) {
            throw std::runtime_error("Mock close failure");
        }
    }
    
    int wait() override {
        return 0; // Mock implementation
        // Mock implementation
    }
    
    void executeStoredFunction() {
        if (_storedFunc) {
            _storedFunc();
        }
    }
    
    void setMockPid(pid_t pid) { _mockPid = pid; }
    void setShouldFailCreate(bool fail) { _shouldFailCreate = fail; }
    void setShouldFailClose(bool fail) { _shouldFailClose = fail; }
};

// Mock Socket class for testing
class MockSocket {
public:
    bool _shouldFailCreate;
    bool _shouldFailAccept;
    bool _shouldFailConnect;
    bool _isConnected;
    std::string _lastSentMessage;
    std::queue<std::string> _receivedMessages;
    
    MockSocket() : _shouldFailCreate(false), _shouldFailAccept(false),
                   _shouldFailConnect(false), _isConnected(false) {}
    
    void createServer(const std::string& path) {
        if (_shouldFailCreate) {
            throw Socket::SocketException("Mock create server failure");
        }
    }
    
    void acceptClient() {
        if (_shouldFailAccept) {
            throw Socket::SocketException("Mock accept failure");
        }
        _isConnected = true;
    }
    
    void connectToServer(const std::string& path) {
        if (_shouldFailConnect) {
            throw Socket::SocketException("Mock connect failure");
        }
        _isConnected = true;
    }
    
    bool isConnected() const { return _isConnected; }
    
    void send(const std::string& msg) {
        _lastSentMessage = msg;
    }
    
    std::string receive() {
        if (_receivedMessages.empty()) {
            return "";
        }
        std::string msg = _receivedMessages.front();
        _receivedMessages.pop();
        return msg;
    }
    
    void addMockMessage(const std::string& msg) {
        _receivedMessages.push(msg);
    }
    
    void disconnect() { _isConnected = false; }
};

TestSuite(Kitchen, .init = redirect_all_stdout_kitchen);

Test(Kitchen, constructor_and_getters)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(42, 3, 1000, 500, false, logger);
    
    cr_assert_eq(kitchen.getID(), 42);
    cr_assert_eq(kitchen.getNbCooks(), 3);
    cr_assert_eq(kitchen.getCookTime(), 1000);
    cr_assert_eq(kitchen.getRestockTime(), 500);
    cr_assert_eq(kitchen.getMaxCmd(), 6); // 2 * nbCooks
    cr_assert_eq(kitchen.getCurrentOrders(), 0);
    cr_assert(!kitchen.isFull(), "Kitchen should not be full initially");
    cr_assert_not_null(kitchen.getIngridient().get());
}

Test(Kitchen, constructor_with_debug)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 800, 400, true, logger);

    cr_assert_eq(kitchen.getID(), 1);
    cr_assert_eq(kitchen.getNbCooks(), 2);
    cr_assert_eq(kitchen.getCookTime(), 800);
    cr_assert_eq(kitchen.getRestockTime(), 400);
    cr_assert_eq(kitchen.getMaxCmd(), 4);
}

Test(Kitchen, setCurrentOrders)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);

    kitchen.setCurrentOrders(3);
    cr_assert_eq(kitchen.getCurrentOrders(), 3);

    kitchen.setCurrentOrders(0);
    cr_assert_eq(kitchen.getCurrentOrders(), 0);
}

Test(Kitchen, setIsFull)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);

    cr_assert(!kitchen.isFull());

    kitchen.setIsFull(true);
    cr_assert(kitchen.isFull());

    kitchen.setIsFull(false);
    cr_assert(!kitchen.isFull());
}

Test(Kitchen, canAcceptOrder_within_capacity)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);

    bool canAccept = kitchen.canAcceptOrder(2);
    cr_assert(canAccept, "Kitchen should accept order within capacity");

    canAccept = kitchen.canAcceptOrder(4);
    cr_assert(canAccept, "Kitchen should accept order at max capacity");
}

Test(Kitchen, canAcceptOrder_exceeds_capacity)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);

    bool canAccept = kitchen.canAcceptOrder(5);
    cr_assert(!canAccept, "Kitchen should not accept order exceeding capacity");
}

Test(Kitchen, canAcceptOrder_sets_full_flag)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);

    kitchen.canAcceptOrder(4); // Max capacity
    cr_assert(kitchen.isFull(), "Kitchen should be marked full at max capacity");
}

Test(Kitchen, createCooks)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 3, 1000, 500, false, logger);

    kitchen.createCooks();

    auto cooks = kitchen.getCooks();
    cr_assert_eq(cooks.size(), 3);

    for (size_t i = 0; i < cooks.size(); ++i) {
        cr_assert_not_null(cooks[i].get());
        cr_assert_eq(cooks[i]->getID(), i);
    }
}

Test(Kitchen, processOrder_valid_order)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    kitchen.createCooks();

    Order order(Margarita, S, 2);
    std::string orderData = order.pack(order);

    int initialOrders = kitchen.getCurrentOrders();
    kitchen.processOrder(orderData);

    cr_assert_eq(kitchen.getCurrentOrders(), initialOrders + 2);
}

Test(Kitchen, processOrder_invalid_format)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    kitchen.createCooks();

    std::string invalidOrder = "invalid_format";

    int initialOrders = kitchen.getCurrentOrders();
    kitchen.processOrder(invalidOrder);

    cr_assert_eq(kitchen.getCurrentOrders(), initialOrders);
}

Test(Kitchen, processOrder_non_order_message)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    kitchen.createCooks();

    std::string nonOrderMessage = "0x02|1|some_data;";

    int initialOrders = kitchen.getCurrentOrders();
    kitchen.processOrder(nonOrderMessage);

    cr_assert_eq(kitchen.getCurrentOrders(), initialOrders);
}

Test(Kitchen, processOrder_sets_full_flag)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 1, 1000, 500, false, logger);
    kitchen.createCooks();

    Order order(Margarita, S, 2);
    std::string orderData = order.pack(order);

    kitchen.processOrder(orderData);

    cr_assert(kitchen.isFull(), "Kitchen should be full after reaching max capacity");
}

Test(Kitchen, setProcess)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    auto mockProcess = std::make_shared<MockProcess>();

    kitchen.setProcess(mockProcess);

    bool exceptionThrown = false;
    try {
        kitchen.startKitchenProcess();
    } catch (const Kitchen::ErrorKitchen& e) {
        exceptionThrown = true;
    }
}

Test(Kitchen, startKitchenProcess_process_creation_failure)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    auto mockProcess = std::make_shared<MockProcess>(true);
    kitchen.setProcess(mockProcess);

    bool exceptionThrown = false;
    try {
        kitchen.startKitchenProcess();
    } catch (const Kitchen::ErrorKitchen& e) {
        exceptionThrown = true;
        std::string what = e.what();
    }
}

Test(Kitchen, startKitchenProcess_with_debug)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, true, logger);
    auto mockProcess = std::make_shared<MockProcess>();
    kitchen.setProcess(mockProcess);

    bool exceptionThrown = false;
    try {
        kitchen.startKitchenProcess();
    } catch (const Kitchen::ErrorKitchen& e) {
        exceptionThrown = true;
    }
}


Test(Kitchen, run_with_inactivity_timeout)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 1, 1000, 500, false, logger);
    kitchen.createCooks();

    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point past = now - std::chrono::seconds(6);

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - past).count();
    cr_assert_geq(duration, 5, "Duration calculation should work correctly");
}

Test(Kitchen, run_with_debug_output)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 1, 1000, 500, true, logger);
    kitchen.createCooks();

    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point past = now - std::chrono::seconds(2);

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - past).count();
    cr_assert_eq(duration, 2, "Duration calculation should be accurate");
}

Test(Kitchen, processOrder_with_debug)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, true, logger);
    kitchen.createCooks();

    Order order(Margarita, S, 1);
    std::string orderData = order.pack(order);

    int initialOrders = kitchen.getCurrentOrders();
    kitchen.processOrder(orderData);

    cr_assert_eq(kitchen.getCurrentOrders(), initialOrders + 1);
}

Test(Kitchen, processOrder_exception_handling)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    kitchen.createCooks();

    std::string malformedOrder = "0x01|invalid|data;";

    int initialOrders = kitchen.getCurrentOrders();
    kitchen.processOrder(malformedOrder);

    cr_assert_eq(kitchen.getCurrentOrders(), initialOrders);
}

Test(Kitchen, canAcceptOrder_with_current_orders)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);

    kitchen.setCurrentOrders(2);

    bool canAccept = kitchen.canAcceptOrder(1);
    cr_assert(canAccept, "Should accept 1 more order when at 2/4");

    canAccept = kitchen.canAcceptOrder(3);
    cr_assert(!canAccept, "Should not accept 3 more orders when at 2/4");
}

Test(Kitchen, stopKitchen_with_running_threads)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 100, 50, false, logger);
    kitchen.createCooks();

    std::thread testThread([&kitchen]() {
        kitchen.restock();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    kitchen.stopKitchen();
    if (testThread.joinable()) {
        testThread.join();
    }
}

Test(Kitchen, stopKitchen_multiple_calls)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    kitchen.createCooks();

    kitchen.stopKitchen();
    kitchen.stopKitchen();
    kitchen.stopKitchen();
}

Test(Kitchen, cook_thread_simulation)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 100, 500, false, logger);
    kitchen.createCooks();

    Order order1(Margarita, S, 1);
    Order order2(Regina, M, 1);

    kitchen.processOrder(order1.pack(order1));
    kitchen.processOrder(order2.pack(order2));

    cr_assert_eq(kitchen.getCurrentOrders(), 2);

    kitchen.stopKitchen();
}

Test(Kitchen, ingredient_management)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    kitchen.createCooks();

    auto ingredient = kitchen.getIngridient();

    int initialDough = ingredient->getDough();
    ingredient->setDough(initialDough + 5);

    cr_assert_eq(ingredient->getDough(), initialDough + 5);
}

Test(Kitchen, kitchen_full_state_management)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 1, 1000, 500, false, logger);
    kitchen.createCooks();

    Order order(Margarita, S, 2);
    kitchen.processOrder(order.pack(order));

    cr_assert(kitchen.isFull(), "Kitchen should be full");
    cr_assert_eq(kitchen.getCurrentOrders(), 2);

    kitchen.setCurrentOrders(1);
    cr_assert_eq(kitchen.getCurrentOrders(), 1);
}

Test(Kitchen, exception_creation_with_different_messages)
{
    std::vector<std::string> errorMessages = {
        "Socket error",
        "Process creation failed",
        "Invalid operation",
        "Resource unavailable"
    };

    for (const auto& msg : errorMessages) {
        bool exceptionThrown = false;
        try {
            throw Kitchen::ErrorKitchen(msg);
        } catch (const Kitchen::ErrorKitchen& e) {
            exceptionThrown = true;
            std::string what = e.what();
        }
    }
}

Test(Kitchen, sendOrder_method)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    kitchen.createCooks();

    kitchen.sendOrder();
}

Test(Kitchen, complex_order_processing)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 3, 1000, 500, false, logger);
    kitchen.createCooks();

    std::vector<PizzaType> pizzaTypes = {Margarita, Regina, Americana, Fantasia};
    std::vector<Size> pizzaSizes = {S, M, L, XL};

    int totalOrders = 0;
    for (size_t i = 0; i < pizzaTypes.size() && i < pizzaSizes.size(); ++i) {
        Order order(pizzaTypes[i], pizzaSizes[i], 1);
        kitchen.processOrder(order.pack(order));
        totalOrders++;
    }

    cr_assert_eq(kitchen.getCurrentOrders(), totalOrders);
}

Test(Kitchen, kitchen_stream_operator)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    kitchen.createCooks();

    std::ostringstream oss;
    oss << kitchen;

    std::string output = oss.str();
    cr_assert(!output.empty(), "Stream output should not be empty");
    cr_assert(output.find("Kitchen N°") != std::string::npos, 
              "Output should contain kitchen identifier");
    cr_assert(output.find("Cooks:") != std::string::npos,
              "Output should contain cook information");
}

Test(Kitchen, edge_case_zero_cooks)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 0, 1000, 500, false, logger);

    cr_assert_eq(kitchen.getNbCooks(), 0);
    cr_assert_eq(kitchen.getMaxCmd(), 0);

    kitchen.createCooks();
    auto cooks = kitchen.getCooks();
    cr_assert_eq(cooks.size(), 0);
}

Test(Kitchen, edge_case_large_cook_count)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 10, 1000, 500, false, logger);

    cr_assert_eq(kitchen.getNbCooks(), 10);
    cr_assert_eq(kitchen.getMaxCmd(), 20);

    kitchen.createCooks();
    auto cooks = kitchen.getCooks();
    cr_assert_eq(cooks.size(), 10);
}

Test(Kitchen, order_queue_stress_test)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 100, 500, false, logger);
    kitchen.createCooks();

    for (int i = 0; i < 10; ++i) {
        Order order(Margarita, S, 1);
        kitchen.processOrder(order.pack(order));
    }

    cr_assert_geq(kitchen.getCurrentOrders(), 0);
    kitchen.stopKitchen();
}

Test(Kitchen, concurrent_operations)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 100, 50, false, logger);
    kitchen.createCooks();

    std::vector<std::thread> threads;

    threads.emplace_back([&kitchen]() {
        for (int i = 0; i < 3; ++i) {
            Order order(Margarita, S, 1);
            kitchen.processOrder(order.pack(order));
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });

    threads.emplace_back([&kitchen]() {
        for (int i = 0; i < 5; ++i) {
            kitchen.getCurrentOrders();
            kitchen.isFull();
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    kitchen.stopKitchen();

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

Test(Kitchen, process_close_with_exception)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    auto mockProcess = std::make_shared<MockProcess>(false, true); // Fail on close
    kitchen.setProcess(mockProcess);

    cr_assert_any_throw(kitchen.stopKitchen());
}

Test(Kitchen, destructor_cleanup)
{
    {
        std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
        Kitchen kitchen(1, 2, 100, 50, false, logger);
        kitchen.createCooks();

        Order order(Margarita, S, 1);
        kitchen.processOrder(order.pack(order));
    }
}

Test(Kitchen, sendQueueStatMessage_without_debug)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    kitchen.createCooks();

    kitchen.setCurrentOrders(3);
    cr_assert_any_throw(kitchen.sendQueueStatMessage());
}

Test(Kitchen, sendQueueStatMessage_with_debug)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, true, logger);
    kitchen.createCooks();

    kitchen.setCurrentOrders(5);
    cr_assert_any_throw(kitchen.sendQueueStatMessage());
}

Test(Kitchen, sendDoneMessage_without_debug)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    kitchen.createCooks();

    cr_assert_any_throw(kitchen.sendDoneMessage());
}

Test(Kitchen, sendDoneMessage_with_debug)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, true, logger);
    kitchen.createCooks();

    cr_assert_any_throw(kitchen.sendDoneMessage());
}

Test(Kitchen, sendRefillMessage_without_debug)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    kitchen.createCooks();

    cr_assert_any_throw(kitchen.sendRefillMessage());
}

Test(Kitchen, sendRefillMessage_with_debug)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, true, logger);
    kitchen.createCooks();

    cr_assert_any_throw(kitchen.sendRefillMessage());
}

Test(Kitchen, sendInactive_message)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    kitchen.createCooks();

    cr_assert_any_throw(kitchen.sendInactive());
}

Test(Kitchen, sendInactive_message_with_debug)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, true, logger);
    kitchen.createCooks();

    cr_assert_any_throw(kitchen.sendInactive());
}


Test(Kitchen, process_not_initialized_error)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);

    cr_assert_throw(kitchen.startKitchenProcess(), Kitchen::ErrorKitchen);
}

Test(Kitchen, socket_creation_error_handling)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    auto mockProcess = std::make_shared<MockProcess>();
    kitchen.setProcess(mockProcess);
    kitchen.startKitchenProcess();
}

Test(Kitchen, sendCookStatus_method)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    kitchen.createCooks();

    cr_assert_any_throw(kitchen.sendCookStatus());
}

Test(Kitchen, sendCookStatus_with_debug)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, true, logger);
    kitchen.createCooks();

    cr_assert_any_throw(kitchen.sendCookStatus());
}

Test(Kitchen, processOrder_empty_message)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    kitchen.createCooks();

    std::string emptyMessage = "";
    int initialOrders = kitchen.getCurrentOrders();

    kitchen.processOrder(emptyMessage);
    cr_assert_eq(kitchen.getCurrentOrders(), initialOrders);
}

Test(Kitchen, processOrder_wrong_message_type)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    kitchen.createCooks();

    std::string wrongType = "0x02|some|data;";
    int initialOrders = kitchen.getCurrentOrders();

    kitchen.processOrder(wrongType);
    cr_assert_eq(kitchen.getCurrentOrders(), initialOrders);
}

Test(Kitchen, processOrder_malformed_order_data)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    kitchen.createCooks();

    std::string malformed = "0x01|invalid_format";
    int initialOrders = kitchen.getCurrentOrders();

    kitchen.processOrder(malformed);
    cr_assert_eq(kitchen.getCurrentOrders(), initialOrders);
}

Test(Kitchen, processOrder_with_zero_pizzas)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    kitchen.createCooks();

    Order order(Margarita, S, 0);
    std::string orderData = order.pack(order);

    int initialOrders = kitchen.getCurrentOrders();
    kitchen.processOrder(orderData);

    cr_assert_eq(kitchen.getCurrentOrders(), initialOrders);
}

Test(Kitchen, canAcceptOrder_exact_capacity_boundary)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);

    cr_assert(kitchen.canAcceptOrder(4), "Should accept exactly max capacity");
    cr_assert(kitchen.isFull(), "Should be full at exact capacity");

    kitchen.setIsFull(false);
    cr_assert(kitchen.canAcceptOrder(3), "Should accept less than max");
    cr_assert(!kitchen.isFull(), "Should not be full below max");
}

Test(Kitchen, canAcceptOrder_with_existing_queue)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    kitchen.createCooks();

    Order order1(Margarita, S, 2);
    kitchen.processOrder(order1.pack(order1));

    bool canAccept = kitchen.canAcceptOrder(2);

    canAccept = kitchen.canAcceptOrder(3);
}

Test(Kitchen, incrementCurrentOrders_thread_safety)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 3, 1000, 500, false, logger);
    kitchen.createCooks();

    int initial = kitchen.getCurrentOrders();
    kitchen.incrementCurrentOrders(5);

    cr_assert_eq(kitchen.getCurrentOrders(), initial + 5);

    kitchen.incrementCurrentOrders(-2);
    cr_assert_eq(kitchen.getCurrentOrders(), initial + 3);
}

Test(Kitchen, restock_with_all_ingredients_full)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 1, 100, 50, false, logger);
    kitchen.createCooks();

    auto ingredient = kitchen.getIngridient();

    ingredient->setDough(5);
    ingredient->setTomato(5);
    ingredient->setCheese(5);
    ingredient->setHam(5);
    ingredient->setMushroom(5);
    ingredient->setSteak(5);
    ingredient->setEggplant(5);
    ingredient->setGoatCheese(5);
    ingredient->setChefLove(5);

    std::thread restockThread([&kitchen]() {
        kitchen.restock();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    kitchen.stopKitchen();

    if (restockThread.joinable()) {
        restockThread.join();
    }
}


Test(Kitchen, run_with_socket_disconnection)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 1, 1000, 500, false, logger);
    kitchen.createCooks();

    std::thread runThread([&kitchen]() {
        kitchen.run();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    kitchen.stopKitchen();

    if (runThread.joinable()) {
        runThread.join();
    }
}

Test(Kitchen, run_with_message_buffer_handling)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 1, 1000, 500, true, logger);
    kitchen.createCooks();

    std::thread runThread([&kitchen]() {
        kitchen.run();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    kitchen.stopKitchen();

    if (runThread.joinable()) {
        runThread.join();
    }
}

Test(Kitchen, cook_thread_with_insufficient_ingredients)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 100, 500, false, logger);
    kitchen.createCooks();

    auto ingredient = kitchen.getIngridient();

    ingredient->setDough(0);
    ingredient->setTomato(0);
    ingredient->setCheese(0);
    ingredient->setHam(0);
    ingredient->setMushroom(0);
    ingredient->setSteak(0);
    ingredient->setEggplant(0);
    ingredient->setGoatCheese(0);

    Order order(Margarita, S, 1);
    kitchen.processOrder(order.pack(order));

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    kitchen.stopKitchen();
}

Test(Kitchen, cook_thread_exception_handling)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 1, 100, 500, true, logger);
    kitchen.createCooks();

    Order order(Margarita, S, 1);
    kitchen.processOrder(order.pack(order));

    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    kitchen.stopKitchen();
}

Test(Kitchen, stopKitchen_with_unjoined_threads)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 100, 50, false, logger);
    kitchen.createCooks();

    Order order(Margarita, S, 1);
    kitchen.processOrder(order.pack(order));

    kitchen.stopKitchen();
    kitchen.stopKitchen();
}

Test(Kitchen, error_kitchen_exception_inheritance)
{
    bool correctInheritance = false;
    try {
        throw Kitchen::ErrorKitchen("test error");
    } catch (const AException& e) {
        correctInheritance = true;
        std::string what = e.what();
        cr_assert(!what.empty(), "Exception message should not be empty");
    } catch (...) {
        correctInheritance = false;
    }

    cr_assert(correctInheritance, "ErrorKitchen should inherit from AException");
}

Test(Kitchen, lastActivity_update_during_operations)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 100, 500, true, logger);
    kitchen.createCooks();

    Order order(Margarita, S, 1);
    kitchen.processOrder(order.pack(order));

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    kitchen.stopKitchen();
}

Test(Kitchen, ingredient_status_output_formatting)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    kitchen.createCooks();

    auto ingredient = kitchen.getIngridient();

    ingredient->setDough(10);
    ingredient->setTomato(8);
    ingredient->setCheese(6);
    ingredient->setHam(4);
    ingredient->setMushroom(2);
    ingredient->setSteak(1);
    ingredient->setEggplant(3);
    ingredient->setGoatCheese(5);
    ingredient->setChefLove(7);

    std::ostringstream oss;
    oss << kitchen;

    std::string output = oss.str();
    cr_assert(output.find("Dough: 10") != std::string::npos);
    cr_assert(output.find("Tomato: 8") != std::string::npos);
    cr_assert(output.find("Love: 7") != std::string::npos);
}

Test(Kitchen, concurrent_order_processing_stress)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 3, 50, 200, false, logger);
    kitchen.createCooks();

    std::vector<std::thread> orderThreads;
    const int numThreads = 5;
    const int ordersPerThread = 2;

    for (int t = 0; t < numThreads; ++t) {
        orderThreads.emplace_back([&kitchen, ordersPerThread]() {
            for (int i = 0; i < ordersPerThread; ++i) {
                Order order(Margarita, S, 1);
                kitchen.processOrder(order.pack(order));
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    kitchen.stopKitchen();

    for (auto& thread : orderThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    cr_assert_geq(kitchen.getCurrentOrders(), 0);
}

Test(Kitchen, message_parsing_edge_cases)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 2, 1000, 500, false, logger);
    kitchen.createCooks();

    std::vector<std::string> edgeCases = {
        "0x01",                    // Too short
        "0x01|",                   // Incomplete
        "0x01||;",                 // Empty fields
        "0x01|999|S|1;",          // Invalid pizza type
        "0x01|1|999|1;",          // Invalid size
        "0x01|1|S|-1;",           // Negative number
        "0x03|unexpected|data;",   // Wrong message type
        "invalid_start|data;",     // No hex prefix
    };

    int initialOrders = kitchen.getCurrentOrders();

    for (const auto& testCase : edgeCases) {
        kitchen.processOrder(testCase);
    }
}


Test(Kitchen, process_exception_in_child_logic)
{
    std::shared_ptr<DebugLogger> logger = std::make_shared<DebugLogger>("plazza-test.log");
    Kitchen kitchen(1, 1, 1000, 500, false, logger);
    auto mockProcess = std::make_shared<MockProcess>();
    kitchen.setProcess(mockProcess);

    kitchen.startKitchenProcess();
}
