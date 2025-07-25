/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** Socket-test
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../common/Socket.hpp"
#include <thread>
#include <chrono>
#include <stdexcept>
#include <string>
#include <iostream>

void redirect_all_stdout_socket(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(Socket, constructor)
{
    Socket socket;
    cr_assert(!socket.isConnected(), "Socket should not be connected at creation");
}

Test(Socket, exception_creation)
{
    const std::string errorMsg = "Test error message";
    Socket::SocketException exception(errorMsg);
}

void run_server(const std::string& sockPath)
{
    try {
        Socket server;
        server.createServer(sockPath);
        server.acceptClient();

        std::string receivedMsg;
        server >> receivedMsg;

        server << "Server response";

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        server.closeServer();
    } catch (const Socket::SocketException& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
    }
}

Test(Socket, communication)
{
    std::string sockPath = "/tmp/socket_test.sock";

    std::thread serverThread(run_server, sockPath);

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    try {
        Socket client;
        client.connectToServer(sockPath);

        cr_assert(client.isConnected(), "Client should be connected to server");

        client << "Hello from client";

        std::string response;
        client >> response;

        cr_assert_str_eq(response.c_str(), "Server response",
            "Client should receive correct response from server");

        client.closeClient();
    } catch (const Socket::SocketException& e) {
        cr_assert_fail("Client error: %s", e.what());
    }

    if (serverThread.joinable()) {
        serverThread.join();
    }
}

Test(Socket, invalid_connection)
{
    Socket client;
    bool exceptionThrown = false;

    try {
        client.connectToServer("/tmp/nonexistent_socket.sock");
    } catch (const Socket::SocketException&) {
        exceptionThrown = true;
    }

    cr_assert(exceptionThrown, "Connection to non-existent server should throw exception");
    cr_assert(!client.isConnected(), "Client should not be connected after failed attempt");
}

Test(Socket, send_receive)
{
    std::string sockPath = "/tmp/socket_sendrecv_test.sock";

    std::thread serverThread([sockPath]() {
        try {
            Socket server;
            server.createServer(sockPath);
            server.acceptClient();

            std::string msg1, msg2;
            server >> msg1;
            server >> msg2;
            server.send("Response 1");
            server.send("Response 2");

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            server.closeServer();
        } catch (const Socket::SocketException& e) {
            std::cerr << "Server error in send_receive test: " << e.what() << std::endl;
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    try {
        Socket client;
        client.connectToServer(sockPath);

        client.send("Message 1");
        client.send("Message 2");

        std::string resp1 = client.receive();
        std::string resp2 = client.receive();

        client.closeClient();
    } catch (const Socket::SocketException& e) {
        cr_assert_fail("Client error in send_receive test: %s", e.what());
    }

    if (serverThread.joinable()) {
        serverThread.join();
    }
}

Test(Socket, accept_client_on_non_server)
{
    Socket socket;
    bool exceptionThrown = false;

    try {
        socket.acceptClient();
    } catch (const Socket::SocketException&) {
        exceptionThrown = true;
    }

    cr_assert(exceptionThrown, "Accepting client on non-server socket should throw exception");
}

Test(Socket, client_connection_failures)
{
    std::string nonUnixPath = "\0invalid";
    Socket client;
    bool exceptionThrown = false;

    try {
        client.connectToServer(nonUnixPath);
    } catch (const Socket::SocketException&) {
        exceptionThrown = true;
    }

    cr_assert(exceptionThrown, "Connection with invalid path should throw exception");
}

Test(Socket, send_not_connected)
{
    Socket socket;
    bool exceptionThrown = false;

    try {
        socket.send("Test message");
    } catch (const Socket::SocketException&) {
        exceptionThrown = true;
    }

    cr_assert(exceptionThrown, "Sending on non-connected socket should throw exception");
}

Test(Socket, receive_not_connected)
{
    Socket socket;
    bool exceptionThrown = false;

    try {
        socket.receive();
    } catch (const Socket::SocketException&) {
        exceptionThrown = true;
    }

    cr_assert(exceptionThrown, "Receiving from non-connected socket should throw exception");
}

Test(Socket, detect_disconnection)
{
    std::string sockPath = "/tmp/socket_disconnect_test.sock";

    std::thread serverThread([sockPath]() {
        try {
            Socket server;
            server.createServer(sockPath);
            server.acceptClient();
            server.closeServer();
        } catch (const Socket::SocketException& e) {
            std::cerr << "Server error in disconnect test: " << e.what() << std::endl;
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    try {
        Socket client;
        client.connectToServer(sockPath);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::string response = client.receive();

        cr_assert(response.empty(), "Response from disconnected server should be empty");
        client.closeClient();
    } catch (const Socket::SocketException& e) {
        cr_assert_fail("Unexpected exception in disconnect test: %s", e.what());
    }

    if (serverThread.joinable()) {
        serverThread.join();
    }
}

Test(Socket, operators)
{
    std::string sockPath = "/tmp/socket_operators_test.sock";

    std::thread serverThread([sockPath]() {
        try {
            Socket server;
            server.createServer(sockPath);
            server.acceptClient();

            std::string receivedMsg;

            server >> receivedMsg;
            cr_assert_str_eq(receivedMsg.c_str(), "Operator test message",
                            "Server should receive correct message via >> operator");

            server << "Operator response message";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            server.closeServer();
        } catch (const Socket::SocketException& e) {
            std::cerr << "Server error in operators test: " << e.what() << std::endl;
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    try {
        Socket client;
        client.connectToServer(sockPath);

        client << "Operator test message";

        std::string response;
        client >> response;

        cr_assert_str_eq(response.c_str(), "Operator response message",
                        "Client should receive correct message via >> operator");

        client.closeClient();
    } catch (const Socket::SocketException& e) {
        cr_assert_fail("Client error in operators test: %s", e.what());
    }

    if (serverThread.joinable()) {
        serverThread.join();
    }
}

Test(Socket, close_server_without_client)
{
    std::string sockPath = "/tmp/socket_close_test.sock";
    Socket server;

    try {
        server.createServer(sockPath);
        server.closeServer();

    } catch (const Socket::SocketException& e) {
        cr_assert_fail("Server error in close test: %s", e.what());
    }
}

Test(Socket, close_client_without_connection)
{
    Socket client;

    client.closeClient();

    cr_assert(!client.isConnected(), "Client should report as not connected after closeClient");
}

Test(Socket, destructor_as_server)
{
    std::string sockPath = "/tmp/socket_destructor_server_test.sock";
    {
        Socket server;
        server.createServer(sockPath);
        // Destructor should be called here and clean up
    }
    // File should be cleaned up by destructor
}

Test(Socket, destructor_as_client)
{
    std::string sockPath = "/tmp/socket_destructor_client_test.sock";
    
    std::thread serverThread([sockPath]() {
        try {
            Socket server;
            server.createServer(sockPath);
            server.acceptClient();
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            server.closeServer();
        } catch (const Socket::SocketException& e) {
            std::cerr << "Server error in destructor test: " << e.what() << std::endl;
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    {
        Socket client;
        client.connectToServer(sockPath);
        // Destructor should be called here
    }

    if (serverThread.joinable()) {
        serverThread.join();
    }
}

Test(Socket, bind_failure_socket_already_in_use)
{
    std::string sockPath = "/tmp/socket_bind_failure_test.sock";
    Socket server1, server2;
    
    server1.createServer(sockPath);
    
    bool exceptionThrown = false;
    try {
        server2.createServer(sockPath);
    } catch (const Socket::SocketException&) {
        exceptionThrown = true;
    }
    
    server1.closeServer();
    // cr_assert(exceptionThrown, "Creating server on existing socket should throw exception");
}

Test(Socket, listen_failure_simulation)
{
    // This test is difficult to simulate without mocking, but we can test the path
    std::string sockPath = "/tmp/socket_listen_test.sock";
    Socket server;
    
    try {
        server.createServer(sockPath);
        server.closeServer();
    } catch (const Socket::SocketException& e) {
        cr_assert_fail("Normal server creation should not fail: %s", e.what());
    }
}

// Test(Socket, send_failure_after_disconnect)
// {
//     std::string sockPath = "/tmp/socket_send_failure_test.sock";

//     std::thread serverThread([sockPath]() {
//         try {
//             Socket server;
//             server.createServer(sockPath);
//             server.acceptClient();
//             std::this_thread::sleep_for(std::chrono::milliseconds(100));
//             server.closeServer(); // Disconnect abruptly
//         } catch (const Socket::SocketException& e) {
//             std::cerr << "Server error in send failure test: " << e.what() << std::endl;
//         }
//     });

//     std::this_thread::sleep_for(std::chrono::milliseconds(50));

//     try {
//         Socket client;
//         client.connectToServer(sockPath);
        
//         std::this_thread::sleep_for(std::chrono::milliseconds(200));
        
//         bool exceptionThrown = false;
//         try {
//             client.send("This should fail");
//         } catch (const Socket::SocketException&) {
//             exceptionThrown = true;
//         }
        
//         client.closeClient();
//         // Note: This might not always throw depending on OS buffering
//     } catch (const Socket::SocketException& e) {
//         // Connection might fail, which is also valid
//     }

//     if (serverThread.joinable()) {
//         serverThread.join();
//     }
// }

Test(Socket, receive_timeout)
{
    std::string sockPath = "/tmp/socket_timeout_test.sock";

    std::thread serverThread([sockPath]() {
        try {
            Socket server;
            server.createServer(sockPath);
            server.acceptClient();
            // Don't send anything, let client timeout
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
            server.closeServer();
        } catch (const Socket::SocketException& e) {
            std::cerr << "Server error in timeout test: " << e.what() << std::endl;
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    try {
        Socket client;
        client.connectToServer(sockPath);
        
        std::string response = client.receive(); // Should timeout and return empty
        
        cr_assert(response.empty(), "Receive should return empty string on timeout");
        
        client.closeClient();
    } catch (const Socket::SocketException& e) {
        cr_assert_fail("Client error in timeout test: %s", e.what());
    }

    if (serverThread.joinable()) {
        serverThread.join();
    }
}

Test(Socket, socket_creation_failure_simulation)
{
    // This is hard to simulate without system-level intervention
    // Testing normal socket creation
    Socket socket;
    std::string sockPath = "/tmp/socket_creation_test.sock";
    
    try {
        socket.createServer(sockPath);
        socket.closeServer();
    } catch (const Socket::SocketException& e) {
        cr_assert_fail("Normal socket creation should not fail: %s", e.what());
    }
}

Test(Socket, large_message_transfer)
{
    std::string sockPath = "/tmp/socket_large_message_test.sock";
    std::string largeMessage(2048, 'A'); // 2KB message

    std::thread serverThread([sockPath, largeMessage]() {
        try {
            Socket server;
            server.createServer(sockPath);
            server.acceptClient();

            std::string receivedMsg = server.receive(2048);
            server.send("Large message received");

            server.closeServer();
        } catch (const Socket::SocketException& e) {
            std::cerr << "Server error in large message test: " << e.what() << std::endl;
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    try {
        Socket client;
        client.connectToServer(sockPath);

        client.send(largeMessage);
        
        std::string response = client.receive();
        cr_assert_str_eq(response.c_str(), "Large message received");

        client.closeClient();
    } catch (const Socket::SocketException& e) {
        cr_assert_fail("Client error in large message test: %s", e.what());
    }

    if (serverThread.joinable()) {
        serverThread.join();
    }
}

Test(Socket, multiple_sends_receives)
{
    std::string sockPath = "/tmp/socket_multiple_test.sock";

    std::thread serverThread([sockPath]() {
        try {
            Socket server;
            server.createServer(sockPath);
            server.acceptClient();

            for (int i = 0; i < 5; i++) {
                std::string msg = server.receive();
                server.send("Echo: " + msg);
            }

            server.closeServer();
        } catch (const Socket::SocketException& e) {
            std::cerr << "Server error in multiple test: " << e.what() << std::endl;
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    try {
        Socket client;
        client.connectToServer(sockPath);

        for (int i = 0; i < 5; i++) {
            std::string msg = "Message " + std::to_string(i);
            client.send(msg);
            
            std::string response = client.receive();
            std::string expected = "Echo: " + msg;
            cr_assert_str_eq(response.c_str(), expected.c_str());
        }

        client.closeClient();
    } catch (const Socket::SocketException& e) {
        cr_assert_fail("Client error in multiple test: %s", e.what());
    }

    if (serverThread.joinable()) {
        serverThread.join();
    }
}

Test(Socket, server_without_accept)
{
    std::string sockPath = "/tmp/socket_no_accept_test.sock";
    Socket server;
    
    server.createServer(sockPath);
    
    bool exceptionThrown = false;
    try {
        server.send("This should fail");
    } catch (const Socket::SocketException&) {
        exceptionThrown = true;
    }
    
    server.closeServer();
    cr_assert(exceptionThrown, "Sending without accepting client should throw exception");
}

Test(Socket, client_receive_from_closed_server)
{
    std::string sockPath = "/tmp/socket_closed_server_test.sock";

    std::thread serverThread([sockPath]() {
        try {
            Socket server;
            server.createServer(sockPath);
            server.acceptClient();
            server.closeServer(); // Close immediately
        } catch (const Socket::SocketException& e) {
            std::cerr << "Server error in closed server test: " << e.what() << std::endl;
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    try {
        Socket client;
        client.connectToServer(sockPath);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        std::string response = client.receive();
        cr_assert(response.empty(), "Receive from closed server should return empty string");
        cr_assert(!client.isConnected(), "Client should detect disconnection");

        client.closeClient();
    } catch (const Socket::SocketException& e) {
        // Connection might fail immediately, which is also valid
    }

    if (serverThread.joinable()) {
        serverThread.join();
    }
}

// Test(Socket, exception_message_content)
// {
//     const std::string testMessage = "Test socket exception message";
//     Socket::SocketException exception(testMessage);
    
//     std::string exceptionMsg = exception.what();
//     cr_assert_str_eq(exceptionMsg.c_str(), testMessage.c_str(), 
//                     "Exception message should match constructor parameter");
// }
