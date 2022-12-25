#pragma once

#include <iostream>
#include <iomanip>

#include "ArrayPool.hpp"
#include "MemoryOwnerFactory.hpp"
#include "TCPClient.hpp"

int main(int argc, char* argv[])
{
    auto pool = std::make_shared<ArrayPool::ArrayPool<char>>();
    ArrayPool::MemoryOwnerFactory<char> mof(std::move(pool));
    FakeCamTCPClient::TCPClient client(
        "10.0.0.2",
        "4823",
        mof
    );

    while (!client.Connect())
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    auto responce = mof.rentMemory(100);
    std::string command;
    int commandSize = 0;
    while (true)
    {
        std::cout << "Enter command:" << std::endl;
        std::getline(std::cin, command);
        client.sendCommand(command.data(), command.size(), responce);
        std::cout << responce.data();
    }

    return EXIT_SUCCESS;
}